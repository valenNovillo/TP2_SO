#!/bin/bash

docker start test
docker exec -it test make clean -C/root/
docker exec -it test make -C/root/
docker stop test