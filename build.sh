#!/bin/sh

# TODO: only set "universal" env var if user has the files
# TODO: explain that the dev needs to install docker

docker compose run --rm --build retro68
