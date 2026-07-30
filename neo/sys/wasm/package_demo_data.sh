#!/usr/bin/env bash
python3 $(dirname $(which emcc))/tools/file_packager.py demo00.data --preload $1/demo00.pk4@/usr/local/share/dhewm3/base/demo00.pk4 --preload ../base/glprogs@/usr/local/share/dhewm3/base/glprogs --js-output=demo00.js --use-preload-cache --no-heap-copy
