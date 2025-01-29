#!/bin/bash
# Скрипт создает релизную версию программы
folder="$(dirname "$0")"
for file in "$folder"/*.c
do
    filename=$(basename "$file")
    out_file="$folder/${filename/.c/.o}"
    gcc -std=c99 -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion -o2 -c "$file" -o "$out_file"
done
help=$(find "$folder" -name "*.o")
help="${help/'\n'/' '}"
gcc -std=c99 -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion $help -o2 -o "$folder"/app.exe