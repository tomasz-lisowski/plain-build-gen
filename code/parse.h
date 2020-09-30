#include <windows.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef PARSE_H
#define PARSE_H

typedef struct
{
    uint64_t len;
    char *s;
} string_t;

typedef struct
{
    string_t *arr;
    uint64_t size;
} parsed_section_t;

typedef struct
{
    parsed_section_t output_path;
    parsed_section_t source_files;
    parsed_section_t object_files;
    parsed_section_t include_paths;
    parsed_section_t compiler;
    parsed_section_t compiler_flags;
    parsed_section_t linker;
    parsed_section_t linker_flags;
} config_t;

config_t parse_cfg_raw(string_t cfr_raw);

#endif
