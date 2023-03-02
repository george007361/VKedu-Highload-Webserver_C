clear
rm -rf build/*
cmake -B build -DDEBUG=0 -DNO_INFO=0 -DNO_ERR=0 -DTIME_IT=1
make -C build