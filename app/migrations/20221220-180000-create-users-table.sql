CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    portalAddress TEXT,
    userName TEXT NOT NULL,
    password TEXT NOT NULL,
    service TEXT NOT NULL,
    busy INTEGER DEFAULT 0
);

