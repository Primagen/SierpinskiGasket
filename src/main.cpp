
#include <OpenGL_common.hpp>

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>


typedef glm::vec2 point2;

// Specify one triangle:
// Modifies points array
void triangle(point2 points[], point2 a, point2 b, point2 c)
{
    static int i = 0;

    points[i++] = a;
    points[i++] = b;
    points[i++] = c;
}

void divide_triangle(point2 points[], point2 a, point2 b, point2 c, int k)
{
    if (k > 0)
    {
        // compute midpoints of sides
        point2 ab = (a + b) / 2.0f;
        point2 ac = (a + c) / 2.0f;
        point2 bc = (b + c) / 2.0f;

        // subdivide all but inner triangle
        divide_triangle(points, a, ab, ac, k-1);
        divide_triangle(points, c, ac, bc, k-1);
        divide_triangle(points, b, bc, ab, k-1);
    }
    else triangle(points, a, b, c);
}

const     int NumDivisions = 6;
constexpr int NumVertices  = static_cast<int>( pow(3, NumDivisions + 1) );

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
}

void init()
{
    point2 points[NumVertices];

    // A triangle in the plane z = 0
    point2 vertices[3] = {
       point2(-1.0, -1.0),
       point2( 0.0,  1.0),
       point2( 1.0, -1.0)
    };

    // An arbitrary initial point inside the triangle
    points[0] = point2(0.25, 0.5);

    divide_triangle(points, vertices[0], vertices[1], vertices[2], NumDivisions);

    //////////////////////////////////////
    //////////////////////////////////////


    // Set up single vertex array buffer:
    GLuint abuffer;
    glGenVertexArrays(1, &abuffer);
    glBindVertexArray(abuffer);

    // Create buffer object:
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points),
                 points, GL_STATIC_DRAW);
}

int main()
{
    init_cstdlib_rand();

    GLFWwindow* window = init_GLFW_window_and_GLEW();
    init();

    GLuint program = initShader("passthrough.vs", "minimal.fs");
    glUseProgram(program);

    GLuint loc = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(loc);

    #define BUFFER_OFFSET(bytes) ((GLvoid*) (bytes)) // sorry
                         //2D array of GL_FLOAT
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));

    while (!glfwWindowShouldClose(window))
    {
        // Render here
        render();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}