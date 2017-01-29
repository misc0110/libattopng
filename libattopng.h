#ifndef _LIBATTOPNG_H_
#define _LIBATTOPNG_H_

#include <stdio.h>
#include <stdint.h>

typedef enum {
    PNG_GRAYSCALE = 0,
    PNG_RGB = 2,
    PNG_PALETTE = 3,
    PNG_GRAYSCALE_ALPHA = 4,
    PNG_RGBA = 6
} libattopng_type_t;


typedef struct {
    libattopng_type_t type;
    size_t capacity;
    char* data;
    uint32_t* palette;
    size_t palette_length;
    size_t width;
    size_t height;
    char* out;
    size_t out_pos;
    size_t out_capacity;
    uint32_t crc;
    uint16_t s1, s2;
    size_t bpp;

} libattopng_t;

libattopng_t* libattopng_new(size_t width, size_t height, libattopng_type_t type);
void libattopng_destroy(libattopng_t* png);
int libattopng_set_palette(libattopng_t* png, uint32_t* palette, size_t length);
void libattopng_set_pixel(libattopng_t* png, size_t x, size_t y, uint32_t index);

char* libattopng_get_data(libattopng_t* png, size_t* len);
int libattopng_save(libattopng_t* png, const char* filename);

#endif

