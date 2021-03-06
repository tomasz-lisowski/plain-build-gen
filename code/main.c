#include <windows.h>

#include <stdio.h>
#include <inttypes.h>

#include "file_system.h"
#include "parse.h"
#include "build_script_gen.h"

int main(int argc, char *argv[argc + 1])
{
    if (argc != 2)
    {
        fprintf(stderr, "Please specify the path to a config file.\n");
        return EXIT_FAILURE;
    }

    HANDLE cfg_handle = get_config_file_handle(argv[1]);
    uint64_t const cfg_file_size = get_file_size(cfg_handle);
    uint64_t const cfg_raw_size = cfg_file_size + 1; /* "+1" to make space for '\0' */

    char *cfg_raw_buffer = malloc(cfg_raw_size);
    uint64_t cfg_bytes_read = 0;
    read_file_into_buffer(cfg_handle, cfg_raw_buffer, cfg_file_size, &cfg_bytes_read);
    cfg_raw_buffer[cfg_raw_size - 1] = '\0'; /* Make buffer null terminated (not necessary) */

    string_t cfg_raw;
    cfg_raw.s = cfg_raw_buffer;
    cfg_raw.len = cfg_raw_size;
    config_t cfg_parsed = parse_cfg_raw(cfg_raw);

    string_t build_script = gen_build_script(&cfg_parsed);
    char *build_script_path = malloc(cfg_parsed.output_path.arr->len + 1);
    memcpy(build_script_path, cfg_parsed.output_path.arr->s, cfg_parsed.output_path.arr->len);
    build_script_path[cfg_parsed.output_path.arr->len] = '\0';

    save_build_script(build_script_path, build_script);
    printf("Build script saved to: %s\n", build_script_path);

    return EXIT_SUCCESS;
}
