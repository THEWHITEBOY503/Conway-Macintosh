# syntax = docker/dockerfile:1

# image args
ARG IMAGE=ghcr.io/autc04/retro68
ARG IMAGE_TAG=latest

##### Project Builder #####
FROM ${IMAGE}:${IMAGE_TAG}

# constants
ENV SCRIPT_NAME="build.sh"
ARG DATA_DIR="/data"
ENV SRC_DIR="${DATA_DIR}/src"
ENV BUILD_DIR="${DATA_DIR}/build"

COPY --chmod=555 ./${SCRIPT_NAME} /bin

WORKDIR ${DATA}

VOLUME "${SRC_DIR}"
VOLUME "${BUILD_DIR}"

ENTRYPOINT [ "sh", "-c" ]
CMD ["/bin/${SCRIPT_NAME} \"${SRC_DIR}\" \"${BUILD_DIR}\" "]
