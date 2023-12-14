#!/bin/bash

# Connect to localhost on port 6667 using nc
nc localhost 6667 << EOF
PASS password1
USER lou2 0 * lou2
NICK lou2
JOIN #test
TOPIC #test