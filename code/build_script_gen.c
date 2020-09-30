#include "build_script_gen.h"

// Returns "buffer" with the "str" appeneded to it
static string_t append_str_to_buffer(string_t const str, string_t buffer)
{
    uint64_t buffer_len_after = buffer.len + str.len;
    buffer.s = realloc(buffer.s, buffer_len_after * sizeof(char));
    memcpy(buffer.s + buffer.len, str.s, str.len);
    buffer.len = buffer_len_after;
    return buffer;
}

string_t gen_build_script(config_t const *cfg_parsed)
{
    string_t build_script = {.len = 0, .s = 0};
    string_t const delimeter = {.len = 1, .s = " "};
    string_t const new_line = {.len = 2, .s = "\r\n"};
    string_t const include_path_prefix = {.len = 2, .s = "-I"};

    if (cfg_parsed->compiler.size != 1)
    {
        fprintf(stderr, "Expected 1 but got %llu elements in the \"compiler\" section.\n", cfg_parsed->compiler.size);
        exit(EXIT_FAILURE);
    }
    // Compiler
    build_script = append_str_to_buffer(cfg_parsed->compiler.arr[0], build_script);

    // Compiler flags
    for (uint64_t i = 0; i < cfg_parsed->compiler_flags.size; i++)
    {
        build_script = append_str_to_buffer(delimeter, build_script);
        build_script = append_str_to_buffer(cfg_parsed->compiler_flags.arr[i], build_script);
    }

    // Include paths
    for (uint64_t i = 0; i < cfg_parsed->include_paths.size; i++)
    {
        build_script = append_str_to_buffer(delimeter, build_script);
        build_script = append_str_to_buffer(include_path_prefix, build_script);
        build_script = append_str_to_buffer(cfg_parsed->include_paths.arr[i], build_script);
    }

    // Source files
    for (uint64_t i = 0; i < cfg_parsed->source_files.size; i++)
    {
        build_script = append_str_to_buffer(delimeter, build_script);
        build_script = append_str_to_buffer(cfg_parsed->source_files.arr[i], build_script);
    }

    // Start new line
    build_script = append_str_to_buffer(new_line, build_script);

    if (cfg_parsed->compiler.size > 1)
    {
        fprintf(stderr, "Expected 1 or 0 but got %llu elements in the \"linker\" section.\n", cfg_parsed->linker.size);
        exit(EXIT_FAILURE);
    }
    if (cfg_parsed->compiler.size == 1)
    {
        // Linker
        build_script = append_str_to_buffer(cfg_parsed->linker.arr[0], build_script);

        // Linker flags
        for (uint64_t i = 0; i < cfg_parsed->linker_flags.size; i++)
        {
            build_script = append_str_to_buffer(delimeter, build_script);
            build_script = append_str_to_buffer(cfg_parsed->linker_flags.arr[i], build_script);
        }

        // Object files
        for (uint64_t i = 0; i < cfg_parsed->object_files.size; i++)
        {
            build_script = append_str_to_buffer(delimeter, build_script);
            build_script = append_str_to_buffer(cfg_parsed->object_files.arr[i], build_script);
        }
    }

    // Start new line
    build_script = append_str_to_buffer(new_line, build_script);

    // Null terminate build script string
    build_script.s = realloc(build_script.s, ++build_script.len);
    build_script.s[build_script.len - 1] = '\0';

    return build_script;
}