#include "utils/gl_utils.h"
#include "utils/Logger.h"

void check_gl_error(const char* file, int line) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (err) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                               error = "UNKNOWN_ERROR"; break;
        }
        Logger::error("OpenGL error: " + error + " at " + file + ":" + std::to_string(line));
    }
}
