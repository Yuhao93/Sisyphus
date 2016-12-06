rm -rf out && mkdir -p out && \
rm -rf gen && mkdir -p gen && \
protoc -I=proto --cpp_out=gen proto/*.proto && \
<<<<<<< HEAD
g++ -o out/sisyphus -std=c++0x src/*.cc gen/*.cc -Ilinux/include -Iinclude -Igen -lwiringPi -lpthread -lprotobuf
=======
g++ -o out/sisyphus -std=c++0x src/*.cc gen/*.cc linux/src/*.cc -Iinclude -Igen -lwiringPi -lpthread -lprotobuf
>>>>>>> 7810e18c2708e4d4eb454b3db4c579eb96af0773
