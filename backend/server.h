#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#include "config.h"

class server_impl;

class server {
    public:
        explicit server(boost::asio::io_service &ios, config &conf);
        virtual ~server();

        void start();
        void stop();

    private:
        server_impl *pimpl_;
};

#endif /* SERVER_H */
