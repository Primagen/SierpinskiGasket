
///
/// See OpenGL_common.hpp for Header
///

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void init_cstdlib_rand()
{
    srand(static_cast<unsigned>(time(NULL)));
}

// Use for textures:
bool power_of_two(unsigned x)
{
    return (x & (x - 1)) == 0;
}

void show_GLSL_and_OpenGL_version()
{
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor   = glGetString( GL_VENDOR );
    const GLubyte *version  = glGetString( GL_VERSION );
    const GLubyte *glslVersion =
    glGetString( GL_SHADING_LANGUAGE_VERSION );
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version (string) : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);
}

void show_OpenGL_extensions()
{
    GLint nExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
    for( int i = 0; i < nExtensions; i++ )
        printf("%s\n", glGetStringi( GL_EXTENSIONS, i ) );

    //Example: Checking for extension availability at runtime
    if (!GLEW_ARB_vertex_program)
    {
        std::cout <<  "ARB_vertex_program is missing!" << std::endl;
    }
}

/**
 * Warning: Return value c_str() should not be used as a temporary... duh!
 */
std::string loadShaderAsString(const std::string& filename)
{
    std::string shaderString;
    std::ifstream f(filename);
    std::string line = "";
    while(getline(f, line))
        shaderString += "\n" + line;
    std::cout << shaderString << std::endl;
    return shaderString;
}

/**
 * Calls glUseProgram if status is good
 */
void verify_program(GLint program_handle)
{
    GLint status;
    glGetProgramiv(program_handle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cout << "Failed to link shader program!" << std::endl;
        GLint logLength;
        glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            char* log = (char*)malloc(logLength); // I'm sorry
            GLsizei written;
            glGetProgramInfoLog(program_handle, logLength, &written, log);
            std::cout << "Program log: " << log << std::endl;
            free(log);
        }
    }
    else
    {
        glUseProgram(program_handle);
    }
}
void verify_shader(GLint shader_handle)
{
    GLint result;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) // can be put into separate function?
    {
        std::cout << "Shader compilation failed!" << std::endl;
        GLint logLength;
        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            char* log = new char[logLength];
            GLsizei written;
            glGetShaderInfoLog(shader_handle, logLength, &written, log);
            std::cout << "Shader log:\n" << log;
            delete log;
        }
    }
}

GLuint initShader(const char* vertex_file, const char* fragment_file)
{
    GLuint vertShader = loadVertexShader(vertex_file);
    GLuint fragShader = loadFragmentShader(fragment_file);
    
    GLuint programHandle = glCreateProgram();
    if (programHandle == 0)
    { std::cout << "Error creating program object" << std::endl; }
    
    // Attach shaders to the program output:
    glAttachShader(programHandle, vertShader);
    glAttachShader(programHandle, fragShader);
    
    // Link
    glLinkProgram(programHandle);
    
    return programHandle;
}

/**
 * Loads and compiles the shader file.
 * Returns the GLuint handle to the fragment shader
 */
GLuint loadFragmentShader(const char* shader_file)
{
    //Create the shader object:
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragShader == 0)
        std::cout << "Error creating vertex shader." << std::endl;

    //Copy the source code into the shader object:
    std::string shaderString = loadShaderAsString(shader_file);
    const GLchar* shaderCode = shaderString.c_str();
    glShaderSource(fragShader, 1, &shaderCode, NULL);

    //Compiler the shader:
    glCompileShader(fragShader);

    //Verify compilation status:
    verify_shader(fragShader);

    return fragShader; // not sure if this is what I should be returning
}

/**
 * Loads and compiles the shader file.
 * Returns the GLint handle to the vertex shader
 */
GLuint loadVertexShader(const char* shader_file)
{
    //Create the shader object:
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertShader == 0)
        std::cout << "Error creating vertex shader." << std::endl;

    //Copy the source code into the shader object:
    std::string shaderString = loadShaderAsString(shader_file);
    const GLchar* shaderCode = shaderString.c_str();
    glShaderSource(vertShader, 1, &shaderCode, NULL);

    //Compile the shader:
    glCompileShader(vertShader);

    //Verify compilation status:
    verify_shader(vertShader);

    return vertShader; // not sure if this is what I should be returning
}

GLFWwindow* init_GLFW_window_and_GLEW()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create Window or its openGL context." << std::endl;
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize glew after window context creation: */
    GLenum err = glewInit();
    if ( err != GLEW_OK ) {
        std::cout << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
    }

    return window;
}
