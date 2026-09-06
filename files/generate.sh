#!/bin/bash

ROOT="."

find "$ROOT" -type d -print0 | while IFS= read -r -d '' dir; do
    echo "Generating: $dir/index.html"

    (
        cd "$dir" || exit 1

        tree -H '' \
            --noreport \
            --dirsfirst \
            -T "$(basename "$(pwd)")" \
            -s \
            -D \
            --charset utf-8 \
            -I "index.html" \
            -o index.html
    )
done


