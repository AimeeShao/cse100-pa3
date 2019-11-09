#!/usr/bin/env bash
echo "Building reference solution..."

if [ ! -d "build_refsol" ]; then
    meson build_refsol -Ddefault_library=static -Db_coverage=false -Doptimization=3 -Dwarning_level=1
fi

ninja -C build_refsol
