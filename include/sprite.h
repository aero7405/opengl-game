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
} Sprite;

void compile_shaders(void);

void create_sprite(Sprite* sprite, GLfloat verticies[], GLfloat sizeof_vertices, GLuint element_order[], GLuint sizeof_elements);

void draw_sprite(Sprite* sprite);

void delete_sprite_buffers(Sprite* sprite);

#endif /* DRAWING_H */