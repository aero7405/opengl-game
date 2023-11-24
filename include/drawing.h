#ifndef DRAWING_H
#define DRAWING_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

typedef struct
{
    /* vertex buffer object, vertex array object, element buffer object */
    GLuint vao, vbo, ebo;
    /* number of elements in ebo */
    GLuint ebo_element_cnt;
} Shape;

void compile_shaders(void);

void create_shape(Shape* shape, GLfloat verticies[], GLfloat sizeof_vertices, GLuint element_order[], GLuint sizeof_elements);

void draw_shape(Shape* shape);

void delete_shape_buffers(Shape* shape);

#endif /* DRAWING_H */