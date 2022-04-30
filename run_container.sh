#/bin/sh

CONTAINER_NAME=otus_hw:latest

if [[ "$(docker images -q $CONTAINER_NAME 2> /dev/null)" == "" ]]; then
    docker build -t $CONTAINER_NAME .
fi

docker run --rm -it --name otus_hw -w /app -v $(pwd):/app $CONTAINER_NAME /bin/bash
