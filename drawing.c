#include <stdlib.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "drawing.h"

/** 
 * TODO:
 *  - textures
 *  - transformations
 *  - custom shaders per object
 * 
 * https://open.gl/textures 
*/

/* I may at some stage want to allow for custom shaders, hence seperating the sources like this */
const GLchar default_vertex_shader_source[] =
    "#version 150 core\n"
    "in vec2 position;"
    "void main()"
    "{"
    "   gl_Position = vec4(position, 0.0, 1.0);"
    "}";
const GLchar default_fragment_shader_source[] =
    "#version 150 core\n"
    "out vec4 out_colour;"
    "void main()"
    "{"
    "   out_colour = vec4(1.0, 1.0, 1.0, 1.0);"
    "}";
const GLchar* vertex_shader_source_ptr = default_vertex_shader_source;
const GLchar* fragment_shader_source_ptr = default_fragment_shader_source;
GLuint* vertex_shader_ptr = NULL;
GLuint* fragment_shader_ptr = NULL;
void compile_shaders(void)
{
    GLint shader_status;
    char shader_error_buffer[512];

    /* creating memory for pointer to point to if not existant */
    if (vertex_shader_ptr == NULL || fragment_shader_ptr == NULL)
    {
        vertex_shader_ptr = (GLuint*)malloc(sizeof(GLuint));
        fragment_shader_ptr = (GLuint*)malloc(sizeof(GLuint));
        /* not going to worry about cleanup because these probably can persist until the end of time */
    }
    /* creating vertex shader */
    *vertex_shader_ptr = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(*vertex_shader_ptr, 1, &vertex_shader_source_ptr, NULL);
    glCompileShader(*vertex_shader_ptr);
    /* checking vertex shader compiled */
    glGetShaderiv(*vertex_shader_ptr, GL_COMPILE_STATUS, &shader_status);
    glGetShaderInfoLog(*vertex_shader_ptr, 512, NULL, shader_error_buffer);
    if (shader_status != GL_TRUE)
    {
        printf("Vertex shader error: \n");
        printf(shader_error_buffer);
    }
    /* creating fragment shader */
    *fragment_shader_ptr = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(*fragment_shader_ptr, 1, &fragment_shader_source_ptr, NULL);
    glCompileShader(*fragment_shader_ptr);
    /* checking vertex shader compiled */
    glGetShaderiv(*fragment_shader_ptr, GL_COMPILE_STATUS, &shader_status);
    glGetShaderInfoLog(*fragment_shader_ptr, 512, NULL, shader_error_buffer);
    if (shader_status != GL_TRUE) 
    {
        printf("Fragment shader error: \n");
        printf(shader_error_buffer);
    }
}

void create_shape(Shape* shape, GLfloat verticies[], GLfloat sizeof_vertices, GLuint element_order[], GLuint sizeof_elements)
{
    GLuint shader_program;
    GLuint position_attrib;

    /* calculating number of elements in array */
    shape->ebo_element_cnt = sizeof_elements / sizeof(GLuint);
    /* creating vertex array object */
    glGenVertexArrays(1, &(shape->vao)); 
    glBindVertexArray(shape->vao); 
    /* creating vertex buffer object */
    glGenBuffers(1, &(shape->vbo));
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof_vertices, verticies, GL_STATIC_DRAW);
    /* creating element buffer object */
    glGenBuffers(1, &(shape->ebo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof_elements, element_order, GL_STATIC_DRAW);

    /* NOTE, I'm sure there is a way to use a single shader program and render many shapes but I aren't clever enough to do that */
    /* shaders must be compiled prior to running this function */
    if (vertex_shader_ptr == NULL || fragment_shader_ptr == NULL)
    {
        printf("Error : Shaders must be compiled prior to creating shapes!\n");
        return;
    }
    /* connecting shaders with shader program */
    shader_program = glCreateProgram();
    glAttachShader(shader_program, *vertex_shader_ptr);
    glAttachShader(shader_program, *fragment_shader_ptr);
    glBindFragDataLocation(shader_program, 0, "out_colour"); 
    glLinkProgram(shader_program); 
    glUseProgram(shader_program); 
    /* linking vertex data and attributes */
    position_attrib = glGetAttribLocation(shader_program, "position"); 
    glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(position_attrib); 
}

void draw_shape(Shape* shape)
{
    /* binding buffers */
    glBindVertexArray(shape->vao); 
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->ebo);
    /* drawing */
    glDrawElements(GL_TRIANGLES, shape->ebo_element_cnt, GL_UNSIGNED_INT, 0);
}

void delete_shape_buffers(Shape* shape)
{
    glDeleteBuffers(1, &(shape->ebo));
    glDeleteBuffers(1, &(shape->vbo));
    glDeleteVertexArrays(1, &(shape->vao));
}