rm -rf node/gen
mkdir node\gen

SET protopath=
for %%P in (proto/*.proto) do call SET "protopath=%%protopath%% proto/%%P"

protoc -I=proto --js_out=import_style=commonjs,binary:node/gen  %protopath%