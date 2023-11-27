#include <stdlib.h>
#include <stdio.h>

#include "image.h"

/**
 * https://github.com/lvandeve/lodepng
 * 
 * Less cancerous way of doing this if this doesn't work out!
 */

float *read_image(const char *filename, int image_type, int *width_ptr, int *height_ptr)
{
    float *points = NULL;
    FILE *file_ptr = NULL;

    file_ptr = fopen(filename, "rb");

    if (file_ptr) {
        if (image_type == IMAGE_BITMAP)
        {
            /* checking file has BM at start of header */
            if (fgetc(file_ptr) == 'B' && fgetc(file_ptr) == 'M')
            {
                int addr_offset = 0;
                int num_colours = 0;
                int num_bits = 0;
                /* int *colour_table = NULL; */

                /* skipping unused data */
                fseek(file_ptr, 8, SEEK_CUR);
                /* getting addr_offset */
                fread(&addr_offset, 4, 1, file_ptr);
                /* Info header size */
                fseek(file_ptr, 4, SEEK_CUR);
                /* width and height */
                fread(width_ptr, 4, 1, file_ptr);
                fread(height_ptr, 4, 1, file_ptr);
                /* skipping unused data */
                fseek(file_ptr, 2, SEEK_CUR);
                /* getting bits per pixel */
                fread(&num_bits, 2, 1, file_ptr);
                /* skipping unused data */
                fseek(file_ptr, 16, SEEK_CUR);
                /* getting number of colours 8 */
                fread(&num_colours, 4, 1, file_ptr);
                /* skipping unused data */
                fseek(file_ptr, 4, SEEK_CUR);

                printf("%i\n", num_bits);

                if (num_bits > 8)
                {
                    int i = 0;
                    int num_px = (*width_ptr) * (*height_ptr);

                    printf("addr_offset : %i\n", addr_offset);

                    /* jumping to pixeldata */
                    fseek(file_ptr, 0, SEEK_SET);
                    fseek(file_ptr, addr_offset, SEEK_CUR);

                    points = (float *)malloc(3 * sizeof(float) * (*width_ptr) * (*height_ptr));
                    
                    /* probably not memory safe to do this without checking for EOF */
                    while (i < num_px)
                    {
                        int r, g, b;
                        fread(&r, 1, 1, file_ptr);
                        fread(&g, 1, 1, file_ptr);
                        fread(&b, 1, 1, file_ptr);
                        fseek(file_ptr, 1, SEEK_CUR);
                    
                        points[3 * i + 0] = (float)r / 255.0f;
                        points[3 * i + 1] = (float)g / 255.0f;
                        points[3 * i + 2] = (float)b / 255.0f;

                        i++;
                    }
                }
                else
                {
                    printf("Unsupported bits per pixel (bpp<=8).");
                }
            }
            else
            {
                printf("Expected Bitmap format to start with \"BM\".\n");
            }
        }

        fclose(file_ptr);
    }
    else
    {
        printf("Failed to open file \"%s\".\n", filename);
    }

    return points; /* make sure that this is cleaned up somewhere */
}