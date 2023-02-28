clear
rm -rf build/*
cmake -B build -DDEBUG=1
make -C build