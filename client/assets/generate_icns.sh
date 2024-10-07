#!/bin/zsh
set -e

INPUT=$1
OUTPUT=$2
if [ -z "$INPUT" ] || [ -z "$OUTPUT" ]; then
  echo "Usage: $0 <input> <output>"
  exit 1
fi

echo "Generating ICNS file from $INPUT to $OUTPUT"

mkdir temp.iconset

sips -z 16 16 "$INPUT" --out temp.iconset/icon_16x16.png
sips -z 32 32 "$INPUT" --out temp.iconset/icon_16x16@2x.png
sips -z 32 32 "$INPUT" --out temp.iconset/icon_32x32.png
sips -z 64 64 "$INPUT" --out temp.iconset/icon_32x32@2x.png
sips -z 128 128 "$INPUT" --out temp.iconset/icon_128x128.png
sips -z 256 256 "$INPUT" --out temp.iconset/icon_128x128@2x.png
sips -z 256 256 "$INPUT" --out temp.iconset/icon_256x256.png
sips -z 512 512 "$INPUT" --out temp.iconset/icon_256x256@2x.png
sips -z 512 512 "$INPUT" --out temp.iconset/icon_512x512.png
sips -z 1024 1024 "$INPUT" --out temp.iconset/icon_512x512@2x.png

iconutil -c icns temp.iconset -o "$OUTPUT"
rm -r temp.iconset
