#!/usr/bin/env bash

set -eu

mkdir -p obj
mkdir -p bin

LINK_FLAGS=""
COMPILER_FLAGS="-g"

for FILE in *.c; do
  gcc $COMPILER_FLAGS $FILE -o ./obj/$FILE.o -c;
done

gcc -o bin/main $LINK_FLAGS obj/*.o
