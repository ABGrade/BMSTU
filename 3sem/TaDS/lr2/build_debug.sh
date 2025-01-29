#!/bin/bash
# Скрипт создает отладочную версию программы
folder="$(dirname "$0")"
for file in "$folder"/*.c
do
    filename=$(basename "$file")
    out_file="$folder/${filename/.c/.o}"
    gcc -std=c99 -g -c "$file" -o "$out_file"
done
help=$(find "$folder" -name "*.o")
help="${help/'\n'/' '}"
gcc -std=c99  $help -g -o "$folder"/app.exe
