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
- libpqxx
- libpion5-dev, which is a bit awkward to get. You could of course build it from
  source, I managed to install the debian stretch version on debian buster as
  well without much trouble. I guess it will have TLS problems, but we do not
  necessarily need our backend service to speak TLS, it probably ends up sitting
  behind some reverse proxy anyway

With all that installed, shelve9 is just a 'make' away! Running it is as easy as
staring it in the directory with the samepl config.yaml, it will serve requests
on port 12345 with that config and serve a response at /api/test

## API Design

GET /api/v1/domain?locator=<loc>&count=<c>
    returns a list of domains. locator and count are optional, no locator means
    start at the beginning, no count means server-picked count. sample response:
    { "domains": [{
            "id": "test"
        }
      ],
      "locator": "293847"
    }

GET /api/v1/domain/<dom>
    returns a single domain
    XXX
    XXX with shelves and books!

POST /api/v1/domain
    request creation of a domain
    XXX

GET /api/v1/domain/<dom>/shelf
    get a list of shelves for this domain
    XXX

GET /api/v1/domain/<dom>/shelf/<sh>
    get details about this shelf
    XXX

GET /api/v1/domain/<dom>/shelf/<sh>/book
    get books for this shelf
    XXX

GET /api/v1/domain/<dom>/shelf/<sh>/book/<bk>
    get details about an individual book
    XXX
    XXX with comments

XXX create/delete shelves/books
XXX create comments
XXX patch some parts, including reordering

## Authentication and Authorization

Shelve9 uses a form of bastardized JWT/Session/Cookie mechanism to authenticate
users. Whenever the server responds with 401 Unauthorized to a non-login
request, the client needs to re-authenticate. This could e.g. be happening
because the token has expired. The end-point to authenticate is:

POST /auth/v1/web-login with a body like {"username" : "test", "password": "secret"}

This will return 401 if the credentials are incorrect, in which case of course
re-trying does not help. If the credentials are correct, the server will repond
with 200 OK and place the authentication token into a http-only cookie.

The actual content API endpoints above will check that cookie and token, and
only accept requests with a valid token, and return 401 otherwise.

This approach works well for web clients, but is awkward for other backends. If
necessary a different login endpoint can be added for that later.

