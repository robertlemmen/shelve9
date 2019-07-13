#include "auth_svc.h"

#include "server.h"

auth_svc::auth_svc(server &srv, config &conf) :
    srv_(srv) {

    srv_.add_handler("/auth/v1", [this](const pion::http::request_ptr &request, const pion::tcp::connection_ptr &conn) -> bool {
        // XXX routing function goes here
        return false;
    });
}

auth_svc::~auth_svc() {
}
