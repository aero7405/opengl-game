#ifndef IMAGE_H
#define IMAGE_H

/* Expected usage:

    char *image = NULL;
    int image_length;
    int width, height;
    float *pixel_buff = NULL;
    ....
    image_length = load_file("resources/idle.bmp", &image);
    pixel_buff = read_image(image, IMAGE_BITMAP, &width, &height); 
    ....
    delete image;
    delete pixel_buff;

*/

/* image types supported */
#define IMAGE_BITMAP 0

float *read_image(const char *filename, int image_type, int *width_ptr, int *height_ptr);

#endif /* IMAGE_H */