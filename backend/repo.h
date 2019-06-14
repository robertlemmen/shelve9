#ifndef REPO_H
#define REPO_H

#include "config.h"

class repository {
    public:
        explicit repository(config &conf);
        virtual ~repository();
};

#endif /* REPO_H */
