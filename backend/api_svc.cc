#include "api_svc.h"

#include "server.h"

api_svc::api_svc(server &srv, config &conf) :
    srv_(srv) {

    srv_.add_handler("/api/v1", [this](const pion::http::request_ptr &request, const pion::tcp::connection_ptr &conn) -> bool {
        // XXX routing function goes here
        return false;
    });
}

api_svc::~api_svc() {
}
