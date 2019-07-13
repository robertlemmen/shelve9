#ifndef AUTH_SVC_H
#define AUTH_SVC_H

#include "config.h"

class server;

class auth_svc {
    public:
        explicit auth_svc(server &srv, config &conf);
        virtual ~auth_svc();

    private:
        server &srv_;

};

#endif /* AUTH_SVC_H */
