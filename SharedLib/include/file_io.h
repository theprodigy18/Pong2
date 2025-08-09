#pragma once

// This function allocate the buffer, so free is needed.
static inline bool ReadFileFromPath(const char* path, char** pBuffer)
{
    FILE* file = NULL;
    if (fopen_s(&file, path, "rb") != 0 || !file)
    {
        LOG_ERROR("Failed to open file '%s'", path);
        return false;
    }

    fseek(file, 0, SEEK_END);
    i64 fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileSize == 0)
    {
        LOG_ERROR("File '%s' is empty", path);
        fclose(file);
        return false;
    }

    char* buffer = ALLOC(char, fileSize + 1);
    fread(buffer, fileSize, 1, file);
    fclose(file);
    buffer[fileSize] = '\0';

    *pBuffer = buffer;

    return true;
}