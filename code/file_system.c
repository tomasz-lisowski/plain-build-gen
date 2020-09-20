#include "file_system.h"

HANDLE get_config_file_handle(char const *path)
{
    HANDLE cfg_handle = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        0,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        0);
    if (cfg_handle == INVALID_HANDLE_VALUE ||
        GetLastError() == ERROR_FILE_NOT_FOUND)
    {
        fprintf(stderr, "Config file not found.\n");
        exit(EXIT_FAILURE);
    }
    return cfg_handle;
}

void read_file_into_buffer(HANDLE file,
                           char *buffer,
                           uint64_t num_bytes_to_read,
                           uint64_t *num_bytes_read)
{
    DWORD num_bytes_read_tmp = 0;
    BOOL success = ReadFile(
        file,
        (void *)buffer,
        num_bytes_to_read,
        &num_bytes_read_tmp,
        0);
    if (!success)
    {
        fprintf(stderr, "Failed to read the config file.\n");
        exit(EXIT_FAILURE);
    }
    *num_bytes_read = (uint64_t)num_bytes_read_tmp;
}

uint64_t get_file_size(HANDLE file)
{
    LARGE_INTEGER file_size;
    BOOL success = GetFileSizeEx(file, &file_size);
    if (!success)
    {
        fprintf(stderr, "Failed to get the config file size.\n");
        exit(EXIT_FAILURE);
    }
    return (uint64_t)file_size.QuadPart;
}
