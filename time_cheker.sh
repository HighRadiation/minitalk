#!/bin/sh

RAND=$(python3 - << 'EOF'
import random, string
print("".join(random.choice(string.ascii_letters) for _ in range(100)))
EOF
)

time ./client "$1" "$RAND"



