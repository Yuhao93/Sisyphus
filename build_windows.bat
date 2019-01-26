@echo off
rm -rf out
mkdir out
rm -rf gen
mkdir gen
rm -rf android/app/src/main/java/sisyphus
mkdir android\app\src\main\java
for %%P in (proto/*.proto) do protoc -I=proto --cpp_out=gen proto/%%P
for %%P in (proto/*.proto) do protoc -I=proto --java_out=android/app/src/main/java proto/%%P
for %%P in (proto/*.proto) do protoc -I=proto --js_out=library=protos,binary:web/gen proto/%%P
java -jar "C:\Program Files (x86)\Google\Closure\closure-compiler.jar" --dependency_mode=STRICT --js="web\gen\protos.js" --js="web\proto\**.js" --js="web\closure\**.js" --js="!web\closure\**_test.js" --js="!web\proto\**_test.js" --js_output_file web/gen/protos.gen.js --entry_point web\gen\protos.js
g++ --std=gnu++11 -Iinclude -Iwindows/include -Igen -o out/sisyphus_win.exe src/*.cc windows/src/*.cc gen/*.cc -lprotobuf
@echo on
