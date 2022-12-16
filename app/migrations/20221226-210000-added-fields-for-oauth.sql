DROP TABLE users;

CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    portalAddress TEXT NOT NULL,
    userName TEXT NOT NULL,
    password TEXT,
    service TEXT NOT NULL,
    busy INTEGER DEFAULT 0,
    token TEXT,
    tokenTimeReceipt TEXT,
    tokenLifetime TEXT
);
