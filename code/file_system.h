#include <windows.h>

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "parse.h"

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

HANDLE get_config_file_handle(char const *path);

void read_file_into_buffer(HANDLE file,
                           char *buffer,
                           uint64_t num_bytes_to_read,
                           uint64_t *num_bytes_read);

uint64_t get_file_size(HANDLE file);

void save_build_script(char *path, string_t build_script);

#endif