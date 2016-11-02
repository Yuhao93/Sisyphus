@echo off
rm -rf out
mkdir out
rm -rf gen
mkdir gen
for %%P in (proto/*.proto) do protoc -I=proto --cpp_out=gen proto/%%P
g++ --std=gnu++11 -Iinclude -Iwindows/include -Igen -o out/sisyphus_win.exe src/*.cc windows/src/*.cc gen/*.cc -lpthread -lprotobuf
@echo on
