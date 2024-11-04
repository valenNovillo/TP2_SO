#!/bin/bash

docker start SistOp
docker exec -it SistOp make clean -C/root/Toolchain
docker exec -it SistOp make clean -C/root/
docker exec -it SistOp make -C/root/Toolchain

if [[ "$1" = "buddy" ]]; then
    docker exec -it SistOp make buddy -C/root/
else
    docker exec -it SistOp make -C/root/
fi

docker stop SistOp
