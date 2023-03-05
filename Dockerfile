FROM ubuntu:20.04

RUN apt-get update && apt-get install -y gcc libpthread-stubs0-dev make cmake

COPY . /server/

WORKDIR /server/

RUN make build

EXPOSE 80

CMD ./build/hl-web-server ./conf/httpd_docker.conf
