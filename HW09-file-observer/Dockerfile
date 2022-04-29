FROM centos:7

WORKDIR /app

RUN yum -y install epel-release && \
    yum -y update && \
    yum -y clean all

RUN set -x \
    && yum -y install \
        make \
        gcc-c++ \
        openssl-devel \
        valgrind \
        cmake3 \
    && cd /app
