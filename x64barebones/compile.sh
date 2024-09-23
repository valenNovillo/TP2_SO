#!/bin/bash

docker start SistOp
docker exec -it SistOp make clean -C/root/Toolchain
docker exec -it SistOp make clean -C/root/
docker exec -it SistOp make -C/root/Toolchain
docker exec -it SistOp make -C/root/
docker stop SistOp

