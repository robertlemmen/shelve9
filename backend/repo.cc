#include "repo.h"

#include <pqxx/pqxx>
#include <iostream>

repository::repository(config &conf) {
    // XXX putting some crap in here just to see if we have all we need
    pqxx::connection c;
    pqxx::work w(c);
    pqxx::row r = w.exec1("SELECT 1");
    w.commit();
    std::cout << r[0].as<int>() << std::endl;
}

repository::~repository() {
}
