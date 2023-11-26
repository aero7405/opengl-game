#include <stdlib.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "sprite.h"

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error %i: %s\n", error, description);
}

/* temp variables for testing drawing */
GLfloat rect1_verts[] = {
    -0.5f, -0.5f,  0.0f,  0.0f,
     0.5f, -0.5f,  1.0f,  0.0f,
     0.5f,  0.5f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.0f,  1.0f
};
GLuint rect_el_order[] = {
    0, 1, 2,
    2, 3, 0
};
Sprite rect1;

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
    create_sprite(&rect1, rect1_verts, sizeof(rect1_verts), rect_el_order, sizeof(rect_el_order));

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        /* Drawing */
        draw_sprite(&rect1);

        /* Update frame */
        glfwSwapBuffers(window);

        /* Event handling - NOTE, assign callbacks if event handling is needed */
        glfwPollEvents();
    }

    /* cleaning up */
    delete_sprite_buffers(&rect1);

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}