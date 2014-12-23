
#ifndef OPENGL_HANDY_FUNCTIONS_HPP
#define OPENGL_HANDY_FUNCTIONS_HPP

///
/// GLEW + GLFW + GLM functions
/// 
/// Also contains functions that can be used
/// in multiple programs
///

//GLEW 1.11.0
//#define GLEW_STATIC
#include <GL/glew.h> // included before OpenGL and GLFW

//GLFW 3.0.4
#include <GLFW/glfw3.h>

//GLM 0.9.6.0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//C++
#include <string>


/**
 * glew32.dll, and
 * glfw3.dll should be in executable's directory,
 * compile statically if you wish, it was just easier to do it dynamically
 */

// Move somewhere else?
void init_cstdlib_rand();
 
// Use for textures:
bool power_of_two(unsigned x);

void show_GLSL_and_OpenGL_version();
void show_OpenGL_extensions();

/**
 * Warning: Return value c_str() should not be used as a temporary... duh!
 */
std::string loadShaderAsString(const std::string& filename);

/**
 * Calls glUseProgram if status is good
 */
void verify_program(GLint program_handle);

//
void verify_shader(GLint shader_handle);


GLuint initShader(const char* vertex_file, const char* fragment_file);

/**
 * Loads and compiles the shader file.
 * Returns the GLint handle to the fragment shader
 */
GLuint loadFragmentShader(const char* shader_file);

/**
 * Loads and compiles the shader file.
 * Returns the GLint handle to the vertex shader
 */
GLuint loadVertexShader(const char* shader_file);

GLFWwindow* init_GLFW_window_and_GLEW();

#include "OpenGL_common.inl"

#endif // OPENGL_HANDY_FUNCTIONS_HPP
