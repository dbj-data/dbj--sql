-- use sha1 extension 
-- use dbj_hash.db table
INSERT INTO unique_strings 
VALUES ( sha1("ABC"), "ABC");