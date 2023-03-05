
.PHONY: 
# for local
	all \
	build \
	clean \
	func-test \ 
	
# for docker
	docker-build \
	docker-run \
	docker-run-log \
	docker-stop \
	docker-clean \

# stress test
	stress-test \

# nginx comparison
	nginx-build \
	nginx-run \
	nginx-stop \
	nginx-clean \

# nginx all in one
	nginx-stress \

# linters
	style \
	clang-tidy \
	cppcheck

all: 
	make clean && \
	make build

build:
	cmake -B build \
	-DDEBUG=0 \
	-DNO_INFO=0 \
	-DNO_ERR=0 \
	-DTIME_IT=0 \
	&& \
	make -C build

clean:
	rm -rf -f build/

docker-build:
	docker build -t hl-server_img .

docker-run:
	docker rm -f hl-server || true && \
	docker run -d --name hl-server -p 80:80 hl-server_img

docker-run-log:
	docker rm -f hl-server || true && \
	docker run --name hl-server -p 80:80 hl-server_img

docker-stop:
	docker stop hl-server || true

docker-clean:
	docker rm -f hl-server || true \
	docker rmi -f hl-server_img || true

nginx-build:
	make nginx-clean && \
	docker build -t hl-nginx_img ./tests/stress/nginx/

nginx-run:
	docker run -d --name hl-nginx -p 80:80 hl-nginx_img

nginx-stop:
	docker stop hl-nginx || true

nginx-clean:
	make nginx-stop && \
	docker rm -f hl-nginx && \
	docker rmi -f hl-nginx_img

nginx-stress: 
	make nginx-build && \
	make nginx-run && \
	sleep 2s && clear && \
	make stress-test && \
	make nginx-clean

stress-test:
	ab -n 20000 -c 100 http://127.0.0.1:80/

func-test:
	python3 ./tests/func/httptest.py

style:
	find -name "*.c" -o -name "*.h" | egrep -v "build|conf|tests" | xargs clang-format -i -style=Google

clang-tidy:
	find -name "*.c" -o -name "*.h" | egrep -v "build|conf|tests|http_close_safe_deb.c" | xargs clang-tidy -p ./build/compile_commands.json

cppcheck:
	find -name "*.c" -o -name "*.h" | egrep -v "build|conf|tests|http_close_safe_deb.c" | xargs cppcheck --enable=all --suppress=missingInclude --suppress=unusedFunction