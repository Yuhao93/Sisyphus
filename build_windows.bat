mkdir out
g++ --std=gnu++11 -Iinclude -Iwindows/include -o out/sisyphus_win.exe src/*.cc windows/src/*.cc -lpthread
