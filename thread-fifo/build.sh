#!/usr/bin/env bash

set -eu

mkdir -p obj
mkdir -p bin

LINK_FLAGS="-lpthread -lrt"

for FILE in *.c; do
  gcc $FILE -o ./obj/$FILE.o -c;
done

gcc -o bin/main $LINK_FLAGS obj/*.o
