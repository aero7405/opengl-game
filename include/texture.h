#ifndef TEXTURE_H
#define TEXTURE_H

/* Expected usage:

    char *image = NULL;
    int width, height;
    float *pixel_buff = NULL;
    ....
    image = load_file("resources/idle.bmp");
    pixel_buff = read_image(image, IMAGE_BITMAP, &width, &height); 
    ....
    delete image;
    delete pixel_buff;

*/

/* image types supported */
#define IMAGE_BITMAP 0

char *load_file(const char *filename);

float *read_image(char *image, int image_type, int *width_ptr, int *height_ptr);

#endif /* TEXTURE_H */