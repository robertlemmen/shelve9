#ifndef API_SVC_H
#define API_SVC_H

#include "config.h"

class server;

class api_svc {
    public:
        explicit api_svc(server &srv, config &conf);
        virtual ~api_svc();

    private:
        server &srv_;

};

#endif /* API_SVC_H */
