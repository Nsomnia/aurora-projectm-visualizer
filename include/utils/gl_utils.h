#pragma once

#include <QOpenGLFunctions_3_3_Core>

void check_gl_error(const char* file, int line);

// Macro to automatically pass file and line number
#define CHECK_GL_ERROR() check_gl_error(__FILE__, __LINE__)
