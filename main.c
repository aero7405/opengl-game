#include <stdlib.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "drawing.h"

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error %i: %s\n", error, description);
}

/* temp variables for testing drawing */
GLfloat rect1_verts[] = {
    -0.1f, -0.5f,
    -0.4f, -0.5f,
    -0.4f,  0.5f,
    -0.1f,  0.5f
};
GLfloat rect2_verts[] = {
     0.1f, -0.5f,
     0.4f, -0.5f,
     0.4f,  0.5f,
     0.1f,  0.5f
};
GLuint rect_el_order[] = {
    0, 1, 2,
    2, 3, 0
};
Shape rect1;
Shape rect2;

int main(void)
{
    GLFWwindow *window;

    /* Setting callback functions */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
        return -1;
    }

    /* Creating context */
    window = glfwCreateWindow(640, 480, "Simple Platformer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    /* Creating shapes */
    compile_shaders();
    create_shape(&rect1, rect1_verts, sizeof(rect1_verts), rect_el_order, sizeof(rect_el_order));
    create_shape(&rect2, rect2_verts, sizeof(rect2_verts), rect_el_order, sizeof(rect_el_order));

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        /* Drawing */
        draw_shape(&rect1);
        draw_shape(&rect2);

        /* Update frame */
        glfwSwapBuffers(window);

        /* Event handling - NOTE, assign callbacks if event handling is needed */
        glfwPollEvents();
    }

    /* cleaning up */
    delete_shape_buffers(&rect1);
    delete_shape_buffers(&rect2);

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}