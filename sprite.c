#include <stdlib.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "sprite.h"
#include "texture.h"

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
    "in vec2 texture_coord;"
    "out vec2 _texture_coord;"
    "void main()"
    "{"
    "   _texture_coord = texture_coord;"
    "   gl_Position = vec4(position, 0.0, 1.0);"
    "}";
const GLchar default_fragment_shader_source[] =
    "#version 150 core\n"
    "in vec2 _texture_coord;"
    "out vec4 out_colour;"
    "uniform sampler2D in_texture;"
    "void main()"
    "{"
    "   out_colour = texture(in_texture, _texture_coord);"
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

void create_sprite(Sprite* sprite, GLfloat verticies[], GLfloat sizeof_vertices, GLuint element_order[], GLuint sizeof_elements)
{
    GLuint shader_program;
    GLuint position_attrib, texture_attrib;


    /* todo: importing textures */
    

    GLuint texture;
    float pixels[] = { /* black and white checkerboard */
        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
    };

    /* binding textures */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    /* setting wrapping settings for texture */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    /* setting filter for texture */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /* binding texture */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);


    /* calculating number of elements in array */
    sprite->ebo_element_cnt = sizeof_elements / sizeof(GLuint);
    /* creating vertex array object */
    glGenVertexArrays(1, &(sprite->vao)); 
    glBindVertexArray(sprite->vao); 
    /* creating vertex buffer object */
    glGenBuffers(1, &(sprite->vbo));
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof_vertices, verticies, GL_STATIC_DRAW);
    /* creating element buffer object */
    glGenBuffers(1, &(sprite->ebo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
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
    glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0); 
    glEnableVertexAttribArray(position_attrib); 
    texture_attrib = glGetAttribLocation(shader_program, "texture_coord"); 
    glVertexAttribPointer(texture_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat))); 
    glEnableVertexAttribArray(texture_attrib); 
}

void draw_sprite(Sprite* sprite)
{
    /* binding buffers */
    glBindVertexArray(sprite->vao); 
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
    /* drawing */
    glDrawElements(GL_TRIANGLES, sprite->ebo_element_cnt, GL_UNSIGNED_INT, 0);
}

void delete_sprite_buffers(Sprite* sprite)
{
    glDeleteBuffers(1, &(sprite->ebo));
    glDeleteBuffers(1, &(sprite->vbo));
    glDeleteVertexArrays(1, &(sprite->vao));
}