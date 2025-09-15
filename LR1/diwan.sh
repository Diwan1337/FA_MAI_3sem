#!/usr/bin/env bash
set -e

# Настройки
MAKE=make
CFLAGS="-std=c99 -Wall -Wextra -O2"

usage() {
    echo "Usage:"
    echo "  ./diwan.sh build [taskN|all]   # собрать всё или конкретную задачу"
    echo "  ./diwan.sh run taskN [args...] # запустить задачу с аргументами"
    echo "  ./diwan.sh test                # запустить тесты (если test/*.in есть)"
    echo "  ./diwan.sh clean               # очистить сборку"
}

if [ $# -lt 1 ]; then
    usage
    exit 1
fi

cmd=$1; shift

case "$cmd" in
    build)
        target=${1:-all}
        echo ">>> Building $target..."
        if ! $MAKE $target CFLAGS="$CFLAGS" ; then
            echo "✗ Build failed"
            exit 1
        fi
        echo "✓ Build success"
        ;;
    run)
        if [ $# -lt 1 ]; then usage; exit 1; fi
        bin=$1; shift
        if [ ! -x "$bin" ]; then
            echo "Binary $bin not found, trying to build..."
            $MAKE $bin CFLAGS="$CFLAGS"
        fi
        echo ">>> Running $bin $@"
        ./$bin "$@"
        ;;
        test)
        echo ">>> Running tests..."
        if [ -x ./run_tests.sh ]; then
            echo ">>> Found run_tests.sh — using it"
            ./run_tests.sh
            exit $?
        fi
        for f in test/*.in; do
            [ -e "$f" ] || { echo "No tests in test/"; exit 0; }
            base=$(basename "$f" .in)
            bin=$(echo $base | cut -d_ -f1) # task1_xxx.in → task1
            echo "--- $base ---"
            if [ ! -x "$bin" ]; then
                echo "Building $bin..."
                $MAKE $bin CFLAGS="$CFLAGS"
            fi
            ./$bin < "$f" > "test/$base.out" || echo "✗ runtime error"
            if [ -f "test/$base.expected" ]; then
                diff -u "test/$base.expected" "test/$base.out" || echo "✗ mismatch"
            fi
        done
        ;;
    clean)
        $MAKE clean
        ;;
    *)
        usage
        exit 1
        ;;
esac
