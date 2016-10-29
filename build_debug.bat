mkdir out
g++ --std=gnu++11 -Iinclude -Idebug/include -o out/sisyphus_deb.exe src/*.cc debug/src/*.cc -lpthread
