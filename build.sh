rm -rf out && mkdir -p out && \
rm -rf gen && mkdir -p gen && \
protoc -I=proto --cpp_out=gen proto/*.proto && \
g++ -o out/sisyphus -std=c++0x src/*.cc gen/*.cc linux/src/*.cc -Ilinux/include -Iinclude -Igen -lwiringPi -lpthread -lprotobuf
