#!/bin/sh
cd .. 
docker run --rm -v $(pwd):$(pwd) docker.slock.it/build-images/cmake:clang16  /bin/bash -c \
"cd $PWD; find src java -not -path \"src/third-party/*\" \\( -name \"*.c\" -o -name \"*.h\" -o -name \"*.java\" \\) | xargs clang-format -i"

cd scripts
