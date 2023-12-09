#!/bin/bash

# Connect to localhost on port 6667 using nc
nc localhost 6667 <<EOF
PASS password1
USER cs 0 * cs
NICK cs
JOIN #tests
MODE #tests +k password123
MODE #tests +l 2