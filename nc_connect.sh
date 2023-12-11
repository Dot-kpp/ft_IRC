#!/bin/bash

# Connect to localhost on port 6667 using nc
nc localhost 6667 <<EOF
PASS password1
USER cs 0 * cs
NICK cs
JOIN #test
MODE #test +i
INVITE lou #test
