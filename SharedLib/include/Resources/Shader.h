#pragma once

#include "Graphics/Graphics.h"

GLint DROP_CreateShaderProgram(const char* vSource, const char* fSource);
void  DROP_DestroyShaderProgram(GLint shaderProgram);