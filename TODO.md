# shelve9 Todo List

- add a database backing to the hello world service
- make a more complete plan here, visual designs, database schema, API

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


## Small Stuff

- improve reading of git hash/tags inside backend makefile, e.g. by stealing
  from keiryaku
- set up a travis job for building
- build with clang as well
