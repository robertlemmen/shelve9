#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <pion/http/server.hpp>

#include "config.h"

class server_impl;

class server {
    public:
        using handler_type = std::function<bool(const pion::http::request_ptr&, const pion::tcp::connection_ptr&)>;

        explicit server(boost::asio::io_service &ios, config &conf);
        virtual ~server();

        void start();
        void stop();

        void add_handler(const std::string &path, handler_type handler);

    private:
        // XXX undo the pimpl!
        server_impl *pimpl_;
};

#endif /* SERVER_H */
