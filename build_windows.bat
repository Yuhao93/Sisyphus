@echo off
rm -rf out
mkdir out
rm -rf gen
mkdir gen

SET protopath=
for %%P in (proto/*.proto) do call SET "protopath=%%protopath%% proto/%%P"

protoc -I=proto --cpp_out=gen %protopath%
protoc -I=proto --js_out=library=protos,binary:web/shared/gen  %protopath%
java -jar "C:\Program Files (x86)\Google\Closure\closure-compiler.jar" --dependency_mode=STRICT --js="web\shared\gen\protos.js" --js="web\shared\proto\**.js" --js="web\shared\closure\**.js" --js="!web\shared\closure\**_test.js" --js="!web\shared\proto\**_test.js" --js_output_file web/shared/gen/protos.gen.js --entry_point web\shared\gen\protos.js
g++ --std=gnu++11 -Iinclude -Iwindows/include -Igen -o out/sisyphus_win.exe src/*.cc windows/src/*.cc gen/*.cc -lprotobuf
@echo on
