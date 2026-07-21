#!/bin/bash
# tests/run.sh
make || exit 1
./assembler tests/ps
for ext in am ob ent ext; do
  if diff -q "tests/ps.$ext" "tests/expected/ps.$ext" > /dev/null 2>&1; then
    echo "OK    $ext"
  else
    echo "FAIL  $ext"
    diff "tests/ps.$ext" "tests/expected/ps.$ext" | head -10
  fi
done
