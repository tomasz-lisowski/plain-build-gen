@ECHO OFF

SET BUILD_DIR=build
SET CODE_DIR=../code
SET SOURCE_FILES=%CODE_DIR%/helpful_builder.c
SET OBJ_FILES=helpful_builder.o
SET COMPILER_FLAGS=-g -c -std=c17 -target x86_64-pc-windows-msvc

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
    lld-link /debug %OBJ_FILES% libcmt.lib kernel32.lib ntdll.lib
    popd
)
