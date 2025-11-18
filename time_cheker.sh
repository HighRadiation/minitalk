#!/bin/sh
time ./client "$1" "$(python3 -c 'import random,string; print("".join(random.choice(string.ascii_letters) for _ in range(100)))')"



