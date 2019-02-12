#include "config.h"

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <yaml-cpp/yaml.h>

using namespace std;
namespace po = boost::program_options;

bool config::setup(int ac, char **av) {
    // set up default values
    set_defaults();

    // create option parsers
    const po::positional_options_description pos_cmds;
    po::options_description cmds("Command-line options");
    cmds.add_options()
        // commands
        ("help,?", "print this message")
        ("version,V", "print program version")
        // basic options to be read before config file
        ("config-file,c", po::value<string>(), "set alternative config file [config.yaml]")
        ("port", po::value<int>(), "set port to listen on [12345]")
        // XXX more cmdline config options
    ;

    po::variables_map vm_cmdline;
    try {
        po::store(po::parse_command_line(ac, av, cmds), vm_cmdline);
        po::notify(vm_cmdline);
    }
    catch(po::error& e) {
        cerr << e.what() << endl;
        BOOST_LOG_TRIVIAL(error) << "could not parse cmdline arguments: " << e.what();
        return false;
    }

    // check if basic command is called
    if (vm_cmdline.count("help")) {
        cout << "This is STORED, please see the provided documentation for detailed instructions." << endl << endl;
        cout << cmds << endl;
        return false;
    }
    if (vm_cmdline.count("version")) {
        cout << "STORED " << VERSION << endl << endl;
        return false;
    }

    // determine config file filename, the rest of the cmdline args will be read after the config
    // file to allow overriding
    if (vm_cmdline.count("config-file")) {
        // XXX does not need to be class member, can discard after this method
        cfg_filename_ = vm_cmdline["config-file"].as<string>();
    }

    if (!read_config()) {
        return false;
    }

    // XXX read other cmdline args and override config from file
    if (vm_cmdline.count("port")) {
        server_port_ = vm_cmdline["port"].as<int>();
    }

    if (!verify_config()) {
        return false;
    }

    return true;
}

void config::dump_config() {
    BOOST_LOG_TRIVIAL(info) << "configuration:";
    BOOST_LOG_TRIVIAL(info) << "  server:";
    BOOST_LOG_TRIVIAL(info) << "    .port: " << server_port_;
    // XXX
}

void config::set_defaults() {
    cfg_filename_ = "config.yaml";
    server_port_ = 12345;
    // XXX
}

// XXX this needs to be cleaned up quite a bit, refactored and broken down
bool config::read_config() {
    try {
        YAML::Node cfg_root = YAML::LoadFile(cfg_filename_);
        if (! cfg_root.IsMap()) {
            // XXX better error handling?
            BOOST_LOG_TRIVIAL(error) << "config root is not a map";
            return false;
        }
        bool result = true;
        for (auto it = cfg_root.begin(); it != cfg_root.end(); ++it) {
            string item = it->first.as<string>();
            if (item == "server") {
                YAML::Node srv_cfg = it->second;
                if (! srv_cfg.IsMap()) {
                    BOOST_LOG_TRIVIAL(error) << "config for 'server' is not a map";
                    result = false;
                }
                else {
                    for (auto it = srv_cfg.begin(); it != srv_cfg.end(); ++it) {
                        string key = it->first.as<string>();
                        if (key == "port") {
                            if (it->second.IsScalar()) {
                                try {
                                    int value = it->second.as<int>();
                                    if ((value >= 1) && (value <= 65535)) {
                                        server_port_ = value;
                                    }
                                    else {
                                        BOOST_LOG_TRIVIAL(error) << "config item server.port is not within the valid range [1;65535]";
                                        result = false;
                                    }
                                }
                                catch (YAML::TypedBadConversion<int> &tc) {
                                    BOOST_LOG_TRIVIAL(error) << "config item server.port is not a number";
                                    result = false;
                                }
                            }
                            else {
                                BOOST_LOG_TRIVIAL(error) << "config item server.port is not a scalar value";
                                result = false;
                            }
                        }
                        else {
                            BOOST_LOG_TRIVIAL(error) << "unexpected config item '" << item << "' within 'server' section";
                            result = false;
                        }
                    }
                }
            }
            else {
                BOOST_LOG_TRIVIAL(error) << "unexpected config item '" << item << "'";
                result = false;
            }
        }
        return result;
    }
    catch (YAML::BadFile &e) {
        BOOST_LOG_TRIVIAL(error) << "Could not read config file '" << cfg_filename_ << "': " << e.msg;
        return false;
    }
    catch (YAML::ParserException &e) {
        BOOST_LOG_TRIVIAL(error) << "Could not parse config file: " << e.msg << " at line " << e.mark.line << ", column " << e.mark.column;
        return false;
    }

    return true;
}

bool config::verify_config() {
    // XXX
    return true;
}
