@ECHO OFF

SET BUILD_DIR=build
SET CODE_DIR=../code
SET SOURCE_FILES=%CODE_DIR%/helpful_builder.c %CODE_DIR%/file_system.c %CODE_DIR%/file_system.h %CODE_DIR%/parse.c %CODE_DIR%/parse.h
SET COMPILER_FLAGS=-g -O0 -std=c17 -target x86_64-pc-windows-msvc -x c -o helpful_builder.exe -fuse-ld=lld

IF "%1%"=="c" (
    rmdir /s /q %BUILD_DIR%
) ELSE IF "%1%"=="r" (
    .\build\helpful_builder.exe .\misc\test.cfg
) ELSE IF "%1%"=="br" (
    build
    build r
) ELSE (
    rmdir /s /q %BUILD_DIR%
    mkdir %BUILD_DIR%
    pushd %BUILD_DIR%
    clang %COMPILER_FLAGS% %SOURCE_FILES%
    popd
)
