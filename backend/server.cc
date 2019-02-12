#include "server.h"

#include <queue>
#include <boost/log/trivial.hpp>
#include <boost/tokenizer.hpp>
#include <pion/http/server.hpp>
#include <pion/http/response_writer.hpp>
#include <pion/http/types.hpp>
#include <json/json.h>

using namespace std;
using namespace boost::asio;
using namespace boost::posix_time;
using namespace pion;

/* a scheduler adapter so that pion can use our common boost io_scheduler rather
 * than using it's own thread pool */
class boost_ios_scheduler : public scheduler {
    public:
        boost_ios_scheduler(boost::asio::io_service &ios)
            : ios_(ios) {};

        virtual boost::asio::io_service& get_io_service(void) {
            return ios_;
        }
    private:
        boost::asio::io_service &ios_;
};

class server_impl {
    public:
        server_impl(io_service &ios, config &conf);
        ~server_impl();

        void start();
        void stop();
    private:
        int port_;
        ip::tcp::endpoint listen_ep_;
        boost_ios_scheduler sched_;
        http::server srv_;

        void test_handler(const http::request_ptr &req, const tcp::connection_ptr &conn);
        void default_handler(const http::request_ptr &req, const tcp::connection_ptr &conn);
        void error_handler(const http::request_ptr &req, const tcp::connection_ptr &conn,
            const string &msg);
};

void server_impl::default_handler(const http::request_ptr &req, const tcp::connection_ptr &conn) {
    BOOST_LOG_TRIVIAL(debug) << "fallback handler called!";
    http::response_writer_ptr writer(
        http::response_writer::create(
            conn,
            *req,
            boost::bind(&tcp::connection::finish, conn)));
    http::response& r = writer->get_response();
    r.set_status_code(http::types::RESPONSE_CODE_NOT_FOUND);
    r.set_status_message(http::types::RESPONSE_MESSAGE_NOT_FOUND);
    writer->send();
}

void server_impl::error_handler(const http::request_ptr &req, const tcp::connection_ptr &conn,
            const string &msg) {
    BOOST_LOG_TRIVIAL(debug) << "error handler: " << msg;
    http::response_writer_ptr writer(
        http::response_writer::create(
            conn,
            *req,
            boost::bind(&tcp::connection::finish, conn)));
    http::response& r = writer->get_response();
    r.set_status_code(http::types::RESPONSE_CODE_SERVER_ERROR);
    r.set_status_message(http::types::RESPONSE_MESSAGE_SERVER_ERROR);
    // XXX perhaps put msg into body?
    writer->send();
}

void server_impl::test_handler(const http::request_ptr &req, const tcp::connection_ptr &conn) {
    BOOST_LOG_TRIVIAL(debug) << "test handler " << req->get_method() << " " << req->get_resource();

    http::response_writer_ptr writer(http::response_writer::create(conn, *req,
                                                boost::bind(&tcp::connection::finish, conn)));

    Json::Value root;
    root["testresults"] = Json::Value(Json::arrayValue);
    ostringstream buffer;
    buffer << root;
    writer->get_response().add_header("Content-Type", "application/json");
    writer->write(buffer.str());
    writer->send();
    return;
}

server_impl::server_impl(io_service &ios, config &conf)
        : port_(conf.server_port())
        , listen_ep_(ip::tcp::v6(), port_)
        , sched_(ios)
        , srv_(sched_, listen_ep_) {

    srv_.add_resource("/api/test", boost::bind(&server_impl::test_handler, this, _1, _2));
    srv_.set_not_found_handler(boost::bind(&server_impl::default_handler, this, _1, _2));
    srv_.set_error_handler(boost::bind(&server_impl::error_handler, this, _1, _2, _3));
}

server_impl::~server_impl() {
    srv_.stop(true);
}

void server_impl::start() {
    BOOST_LOG_TRIVIAL(debug) << "HTTP server starting...";
    try {
        srv_.start();
        BOOST_LOG_TRIVIAL(info) << "Serving HTTP requests on port " << port_;
    }
    catch (boost::system::system_error &e) {
        BOOST_LOG_TRIVIAL(warning) << "Could not start server: " << e.what();
    }
}

void server_impl::stop() {
    BOOST_LOG_TRIVIAL(debug) << "HTTP server shutting down...";
    srv_.stop();
}

/* implementation of external module interface */

server::server(io_service &ios, config &conf) {
    pimpl_ = new server_impl(ios, conf);
}

server::~server() {
    delete pimpl_;
}

void server::start() {
    pimpl_->start();
}

void server::stop() {
    pimpl_->stop();
}
