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

string_t gen_build_script(config_t *cfg_parsed)
{
    string_t build_script = {.len = 0, .s = 0};
    string_t const delimeter = {.len = 1, .s = " "};

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

    // Source files
    for (uint64_t i = 0; i < cfg_parsed->source.size; i++)
    {
        build_script = append_str_to_buffer(delimeter, build_script);
        build_script = append_str_to_buffer(cfg_parsed->source.arr[i], build_script);
    }

    // Null terminate build script string
    build_script.s = realloc(build_script.s, ++build_script.len);
    build_script.s[build_script.len - 1] = '\0';

    return build_script;
}