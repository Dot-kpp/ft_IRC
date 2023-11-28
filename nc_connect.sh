#!/bin/bash

# Connect to localhost on port 6667 using nc
nc -C localhost 6667 <<EOF
PASS password1
USER lou 0 * lou
NICK lou
NAMES lou
exec </dev/tty
