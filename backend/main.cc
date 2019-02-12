#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

#include "config.h"
#include "server.h"

using namespace std;

static const int MIN_THREADS = 4;

int main(int argc, char **argv) {
    // set up logging
    boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
    boost::log::add_common_attributes();
    boost::log::add_console_log(
        cout,
        boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%",
        boost::log::keywords::auto_flush = true
    );

    // XXX set based on some config (-v or so), but how? circular deps...
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::debug
    );

    config cfg;
    if (!cfg.setup(argc, argv)) {
        return 1;
    }

    BOOST_LOG_TRIVIAL(info) << "SHELVE9 " << VERSION;

    cfg.dump_config();

    unsigned int cores = boost::thread::hardware_concurrency();
    if (cores < MIN_THREADS) {
        // sometimes this can't be determined, so we pick a safe value
        cores = MIN_THREADS;
    }
    BOOST_LOG_TRIVIAL(debug) << "running with " << cores << " threads...";
    boost::asio::io_service ios(cores);
    boost::asio::signal_set sigs(ios);

    // set up and main subcomponents
    server srv(ios, cfg);
  
    sigs.add(SIGINT);
    sigs.add(SIGTERM);
    sigs.add(SIGQUIT);
    sigs.async_wait(
        [&sigs, &srv](boost::system::error_code ec, int signo) {
            // XXX does this not need to look at ec and re-wait to deal with spurious wakeups?
            BOOST_LOG_TRIVIAL(debug) << "signal caught, shutting down...";
            // stop main subcomponents and clean up
            srv.stop();
            sigs.cancel();
        }
    );

    // set up thread pool
    boost::thread_group threadpool;
    for (unsigned int i = 0; i < (cores-1); ++i) {
        threadpool.create_thread(
            [&ios]() { 
                ios.run(); 
            }
        );
    }

    // start subcomponents
    srv.start();

    // main thread joins as well, this will block until no more work in ios
    ios.run();

    // wait for other threads to shut down
    threadpool.join_all();
    BOOST_LOG_TRIVIAL(info) << "clean shutdown";

    return 0;
}
