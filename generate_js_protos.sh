rm -rf node/gen
mkdir -p node/gen
protoc -I=proto --js_out=import_style=commonjs,binary:node/gen proto/*.proto
