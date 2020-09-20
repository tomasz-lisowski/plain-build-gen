#include <stdio.h>

#include "parse.h"

typedef struct
{
    string_t const output_path;
    string_t const source;
    string_t const include;
    string_t const comment;
    string_t const end;
    string_t const compiler;
    string_t const compiler_flags;
    string_t const linker;
    string_t const linker_flags;
} token_struct;

static token_struct const tokens = {
    .output_path = {.len = 13, .s = "[output_path]"},
    .source = {.len = 8, .s = "[source]"},
    .include = {.len = 9, .s = "[include]"},
    .comment = {.len = 2, .s = "//"},
    .end = {.len = 5, .s = "[end]"},
    .compiler = {.len = 10, .s = "[compiler]"},
    .compiler_flags = {.len = 16, .s = "[compiler_flags]"},
    .linker = {.len = 8, .s = "[linker]"},
    .linker_flags = {.len = 14, .s = "[linker_flags]"},
};

static string_t get_next_line(string_t cfg_raw, uint64_t i)
{
    string_t next_line = {.len = 0, .s = cfg_raw.s};
    for (; i < cfg_raw.len; ++i)
    {
        ++next_line.len;
        if (cfg_raw.s[i] == '\n')
        {
            break;
        }
    }
    return next_line;
}

static void print_str(string_t str)
{
    for (uint64_t i = 0; i < str.len; i++)
    {
        putc(str.s[i], stdout);
    }
}

/*
 Checks if the shorter string of length n matches exactly 
 the first n characters of the other string.
*/
static bool str_match_str(string_t str1, string_t str2)
{
    uint64_t shorter_len = str1.len <= str2.len ? str1.len : str2.len;
    for (uint64_t i = 0; i < shorter_len; i++)
    {
        if (str1.s[i] != str2.s[i])
        {
            return false;
        }
    }
    return true;
}

static bool line_is_section_header(string_t line)
{
    bool line_is_empty = line.len == 0;
    bool line_is_comment = str_match_str(line, tokens.comment);
    bool line_is_section_end = str_match_str(line, tokens.end);
    return !(line_is_empty || line_is_comment || line_is_section_end);
}

/*
 Returns the number of bytes parsed when line 
 is a section header and line length otherwise.
*/
static uint64_t parse_section(string_t const line, uint64_t const line_start, config_t *cfg_parsed)
{
    if (!line_is_section_header(line))
    {
        return line.len;
    }

    parsed_section_t *section;
    if (str_match_str(line, tokens.include))
    {
        section = &cfg_parsed->include;
    }
    else if (str_match_str(line, tokens.source))
    {
        section = &cfg_parsed->source;
    }
    else if (str_match_str(line, tokens.linker_flags))
    {
        return line.len;
    }
    else if (str_match_str(line, tokens.linker))
    {
        return line.len;
    }
    else if (str_match_str(line, tokens.compiler_flags))
    {
        return line.len;
    }
    else if (str_match_str(line, tokens.compiler))
    {
        return line.len;
    }
    else if (str_match_str(line, tokens.output_path))
    {
        section = &cfg_parsed->output_path;
    }
    else
    {
        fprintf(stderr, "Section contained in the file is not supported.\n");
        exit(EXIT_FAILURE);
    }

    // TODO: Parse section here
    return line.len;
}

config_t parse_cfg_raw(string_t cfg_raw)
{
    config_t cfg_parsed;
    uint64_t bytes_read = 0;
    string_t line;
    while (bytes_read < cfg_raw.len)
    {
        line = get_next_line(cfg_raw, bytes_read);
        bytes_read += parse_section(line, bytes_read, &cfg_parsed);
    }
    return cfg_parsed;
}
