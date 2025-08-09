#include "pch.h"
#include "Resources/Shader.h"

#include "file_io.h"

#define LOG_SIZE 512

#pragma region INTERNAL
static bool TestCompileShader(GLuint shader, const char* type)
{
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[LOG_SIZE];
        glGetShaderInfoLog(shader, LOG_SIZE, NULL, infoLog);
        LOG_ERROR("Failed to compile %s shader: %s", type, infoLog);
        return false;
    }

    return true;
}
static bool TestLinkProgram(GLint program)
{
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[LOG_SIZE];
        glGetProgramInfoLog(program, LOG_SIZE, NULL, infoLog);
        LOG_ERROR("Failed to link program: %s", infoLog);
        return false;
    }

    return true;
}
#pragma endregion

GLint DROP_CreateShaderProgram(const char* vPath, const char* fPath)
{
    ASSERT_MSG(vPath && fPath, "One or more shader paths are null.");

    char* vSource = NULL;
    if (!ReadFileFromPath(vPath, &vSource) || !vSource)
    {
        ASSERT_MSG(false, "Failed to read vertex shader.");
        return -1;
    }

    char* fSource = NULL;
    if (!ReadFileFromPath(fPath, &fSource) || !fSource)
    {
        ASSERT_MSG(false, "Failed to read fragment shader.");
        FREE(vSource);
        return -1;
    }

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShader, 1, (const GLchar**) &vSource, NULL);
    glShaderSource(fragShader, 1, (const GLchar**) &fSource, NULL);

    FREE(vSource);
    FREE(fSource);

    glCompileShader(vertShader);
    glCompileShader(fragShader);

    if (!TestCompileShader(vertShader, "vertex"))
    {
        ASSERT_MSG(false, "Failed to compile vertex shader.");
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        return -1;
    }

    if (!TestCompileShader(fragShader, "fragment"))
    {
        ASSERT_MSG(false, "Failed to compile fragment shader.");
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        return -1;
    }

    GLint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    if (!TestLinkProgram(program))
    {
        ASSERT_MSG(false, "Failed to link shader program.");
        glDeleteProgram(program);
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        return -1;
    }

    // Cleanup.
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}

void DROP_DestroyShaderProgram(GLint shaderProgram)
{
    if (shaderProgram != -1)
    {
        glDeleteProgram(shaderProgram);
    }
}