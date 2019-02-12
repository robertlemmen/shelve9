# How to contribute to shelve9

## Building the Backend

The shelve9 backend is written in C++, and will require a handful of build
dependencies to be installed:

- a C++ compiler that does C++-17. GCC 7.3 is known to work, but others should
  too
- make
- libbz2-dev
- libyaml-cpp-dev 
- libjsoncpp-dev
- openssl
- boost (1.62 is known to work)
- libpion5-dev, which is a bit awkward to get. You could of course build it from
  source, I managed to install the debian stretch version on debian buster as
  well without much trouble. I guess it will have TLS problems, but we do not
  necessarily need our backend service to speak TLS, it probably ends up sitting
  behind some reverse proxy anyway

With all that installed, shelve9 is just a 'make' away! Running it is as easy as
staring it in the directory with the samepl config.yaml, it will server requests
on port 12345 with that config and serve a response at /api/test
