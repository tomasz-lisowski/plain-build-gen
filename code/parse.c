#include <stdio.h>

#include "parse.h"

enum token
{
    OUTPUT_PATH = 0,
    SOURCE = 1,
    INCLUDE = 2,
    COMMENT = 3,
    END = 4,
    COMPILER = 5,
    COMPILER_FLAGS = 6,
    LINKER = 7,
    LINKER_FLAGS = 8,
    TOKEN_COUNT,
    TOKEN_UNKNOWN,
};

static string_t const tokens[TOKEN_COUNT] = {
    [OUTPUT_PATH] = {.len = 13, .s = "[output_path]"},
    [SOURCE] = {.len = 8, .s = "[source]"},
    [INCLUDE] = {.len = 9, .s = "[include]"},
    [COMMENT] = {.len = 2, .s = "//"},
    [END] = {.len = 5, .s = "[end]"},
    [COMPILER] = {.len = 10, .s = "[compiler]"},
    [COMPILER_FLAGS] = {.len = 16, .s = "[compiler_flags]"},
    [LINKER] = {.len = 8, .s = "[linker]"},
    [LINKER_FLAGS] = {.len = 14, .s = "[linker_flags]"},
};

static string_t get_next_line(string_t cfg_raw, uint64_t start_offset)
{
    string_t next_line = {.len = 0, .s = cfg_raw.s + start_offset};
    for (uint64_t i = 0; i < cfg_raw.len - start_offset; ++i)
    {
        ++next_line.len;
        if (next_line.s[i] == '\r' &&
            next_line.s[i + 1] == '\n')
        {
            ++next_line.len; // '\n'
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

static bool str_is_not_section_header(string_t str)
{
    bool is_empty = str.len == 0;
    bool is_comment = str_match_str(str, tokens[COMMENT]);
    bool is_end = str_match_str(str, tokens[END]);
    bool is_not_header = is_empty || is_end || is_comment;
    return is_not_header;
}

static enum token str_to_token(string_t line)
{
    for (int i = 0; i < TOKEN_COUNT; ++i)
    {
        if (str_match_str(line, tokens[i]))
        {
            return i;
        }
    }
    return TOKEN_UNKNOWN;
}

/*
 Returns the number of bytes parsed when line 
 is a section header and line length otherwise.
*/
static uint64_t parse_section(
    string_t const str_header,
    uint64_t const header_start,
    config_t *cfg_parsed,
    string_t cfg_raw)
{
    if (str_is_not_section_header(str_header))
    {
        return str_header.len;
    }

    parsed_section_t *section;
    switch (str_to_token(str_header))
    {
    case OUTPUT_PATH:
        section = &(cfg_parsed->output_path);
        break;
    case SOURCE:
        section = &(cfg_parsed->source);
        break;
    case INCLUDE:
        section = &(cfg_parsed->include);
        break;
    case COMPILER:
        section = &cfg_parsed->compiler;
        break;
    case COMPILER_FLAGS:
        section = &cfg_parsed->compiler_flags;
        break;
    case LINKER:
        section = &cfg_parsed->linker;
        break;
    case LINKER_FLAGS:
        section = &cfg_parsed->linker_flags;
        break;
    case TOKEN_UNKNOWN:
    default:
        return str_header.len;
    }

    string_t section_line;
    uint64_t section_bytes_read = str_header.len; // Already read the secton header
    section->arr = 0;
    section->size = 0;
    do
    {
        section_line = get_next_line(cfg_raw, header_start + section_bytes_read);
        section_bytes_read += section_line.len;
        if (str_match_str(section_line, tokens[COMMENT]))
        {
            continue;
        }
        else if (str_match_str(section_line, tokens[END]))
        {
            break;
        }

        // Store section entry
        ++section->size;
        section->arr = realloc(section->arr, section->size * sizeof(string_t));
        section->arr[section->size - 1].len = section_line.len - 2; // Ignore '\r\n' at end of line
        section->arr[section->size - 1].s = section_line.s;
    } while (header_start + section_bytes_read < cfg_raw.len);

    return section_bytes_read;
}

config_t parse_cfg_raw(string_t cfg_raw)
{
    config_t cfg_parsed;
    uint64_t bytes_read = 0;
    string_t line;
    while (bytes_read < cfg_raw.len)
    {
        line = get_next_line(cfg_raw, bytes_read);
        bytes_read += parse_section(line, bytes_read, &cfg_parsed, cfg_raw);
    }
    return cfg_parsed;
}
