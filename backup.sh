#!/bin/bash
# backup home dir or individual folders

args="$@"

echo "$args" | grep -i "foo" >/dev/null 2>&1 && echo "we got foo"
