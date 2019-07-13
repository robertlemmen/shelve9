CREATE TABLE site_user (
    id Serial PRIMARY KEY,
    email text NOT NULL UNIQUE,
    password_salt text,
    password_hash text,
    username text NOT NULL,
    created timestamp NOT NULL,
    last_login timestamp NOT NULL,
    doi_confirmation boolean NOT NULL,
    external_identified boolean NOT NULL
);

CREATE TABLE domain (
    id Serial PRIMARY KEY,
    created timestamp NOT NULL,
    name text UNIQUE NOT NULL,
    publicly_visible boolean NOT NULL,
    admin_user integer references site_user(id) NOT NULL,
    auto_access_email_pattern text
);

CREATE TABLE domain_access_code (
    code text PRIMARY KEY,
    expires timestamp NOT NULL,
    domain_id integer REFERENCES domain(id) NOT NULL,
    email_restrict_pattern text,
    allow_reuse boolean NOT NULL
);

CREATE TABLE domain_admin (
    admin_user integer references site_user(id) NOT NULL,
    domain_id integer REFERENCES domain(id) NOT NULL,
    PRIMARY KEY(admin_user, domain_id)
);


CREATE TABLE user_domain_link (
    user_id integer references site_user(id) NOT NULL,
    domain_id integer REFERENCES domain(id) NOT NULL,
    PRIMARY KEY(user_id, domain_id)
);

CREATE TABLE shelf (
    id Serial PRIMARY KEY,
    domain_id integer REFERENCES domain(id) NOT NULL,
    name text,
    created timestamp NOT NULL,
    last_modified timestamp NOT NULL
);

CREATE TABLE book (
    id Serial PRIMARY KEY,
    shelf_id integer REFERENCES shelf(id) NOT NULL,
    title text NOT NULL,
    author text NOT NULL,
    description text NOT NULL,
    book_user integer references site_user(id) NOT NULL,
    created timestamp NOT NULL,
    last_modified timestamp NOT NULL
);

CREATE TABLE book_comment (
    id Serial PRIMARY KEY,
    book_id integer REFERENCES book(id) NOT NULL,
    body_text text NOT NULL,
    comment_user integer references site_user(id) NOT NULL,
    created timestamp NOT NULL,
    last_modified timestamp NOT NULL
);
