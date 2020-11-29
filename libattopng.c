#include "libattopng.h"
#include <stdlib.h>
#include <string.h>

#define LIBATTOPNG_ADLER_BASE 65521

static const uint32_t libattopng_crc32[256] = {
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832,
        0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
        0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a,
        0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
        0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3,
        0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
        0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab,
        0xb6662d3d, 0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01, 0x6b6b51f4,
        0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
        0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074,
        0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
        0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525,
        0x206f85b3, 0xb966d409, 0xce61e49f, 0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
        0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615,
        0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 0xfed41b76,
        0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
        0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c, 0x36034af6,
        0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
        0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7,
        0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
        0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7,
        0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278,
        0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
        0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 0xbdbdf21c, 0xcabac28a, 0x53b39330,
        0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
        0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

/* ------------------------------------------------------------------------ */
libattopng_t *libattopng_new(size_t width, size_t height, libattopng_type_t type) {
    libattopng_t *png;
    if (SIZE_MAX / 4 / width < height) {
        /* ensure no type leads to an integer overflow */
        return NULL;
    }
    png = (libattopng_t *) calloc(sizeof(libattopng_t), 1);
    png->width = width;
    png->height = height;
    png->capacity = width * height;
    png->palette_length = 0;
    png->palette = NULL;
    png->out = NULL;
    png->out_capacity = 0;
    png->out_pos = 0;
    png->type = type;
    png->stream_x = 0;
    png->stream_y = 0;

    if (type == PNG_PALETTE) {
        png->palette = (uint32_t *) calloc(256, sizeof(uint32_t));
        if (!png->palette) {
            free(png);
            return NULL;
        }
        png->bpp = 1;
    } else if (type == PNG_GRAYSCALE) {
        png->bpp = 1;
    } else if (type == PNG_GRAYSCALE_ALPHA) {
        png->capacity *= 2;
        png->bpp = 2;
    } else if (type == PNG_RGB) {
        png->capacity *= 4;
        png->bpp = 3;
    } else if (type == PNG_RGBA) {
        png->capacity *= 4;
        png->bpp = 4;
    }

    png->data = (char *) calloc(png->capacity, 1);
    if (!png->data) {
        free(png->palette);
        free(png);
        return NULL;
    }
    return png;
}

/* ------------------------------------------------------------------------ */
int libattopng_set_palette(libattopng_t *png, uint32_t *palette, size_t length) {
    if (length > 256) {
        return 1;
    }
    memmove(png->palette, palette, length * sizeof(uint32_t));
    png->palette_length = length;
    return 0;
}

/* ------------------------------------------------------------------------ */
void libattopng_set_pixel(libattopng_t *png, size_t x, size_t y, uint32_t color) {
    if (!png || x >= png->width || y >= png->height) {
        return;
    }
    if (png->type == PNG_PALETTE || png->type == PNG_GRAYSCALE) {
        png->data[x + y * png->width] = (char) (color & 0xff);
    } else if (png->type == PNG_GRAYSCALE_ALPHA) {
        ((uint16_t *) png->data)[x + y * png->width] = (uint16_t) (color & 0xffff);
    } else {
        ((uint32_t *) png->data)[x + y * png->width] = color;
    }
}

/* ------------------------------------------------------------------------ */
uint32_t libattopng_get_pixel(libattopng_t* png, size_t x, size_t y) {
    uint32_t pixel = 0;
    if (!png || x >= png->width || y >= png->height) {
        return pixel;
    }
    if (png->type == PNG_PALETTE || png->type == PNG_GRAYSCALE) {
        pixel = (uint32_t)(png->data[x + y * png->width] & 0xff);
    } else if (png->type == PNG_GRAYSCALE_ALPHA) {
        pixel = (uint32_t)(((uint16_t *) png->data)[x + y * png->width] & 0xffff);
    } else {
        pixel = ((uint32_t *) png->data)[x + y * png->width];
    }
    return pixel;
}

/* ------------------------------------------------------------------------ */
void libattopng_start_stream(libattopng_t* png, size_t x, size_t y) {
    if(!png || x >= png->width || y >= png->height) {
        return;
    }
    png->stream_x = x;
    png->stream_y = y;
}

/* ------------------------------------------------------------------------ */
void libattopng_put_pixel(libattopng_t* png, uint32_t color) {
    size_t x, y;
    if(!png) {
        return;
    }
    x = png->stream_x;
    y = png->stream_y;
    if (png->type == PNG_PALETTE || png->type == PNG_GRAYSCALE) {
        png->data[x + y * png->width] = (char) (color & 0xff);
    } else if (png->type == PNG_GRAYSCALE_ALPHA) {
        ((uint16_t *) png->data)[x + y * png->width] = (uint16_t) (color & 0xffff);
    } else {
        ((uint32_t *) png->data)[x + y * png->width] = color;
    }
    x++;
    if(x >= png->width) {
        x = 0;
        y++;
        if(y >= png->height) {
            y = 0;
        }
    }
    png->stream_x = x;
    png->stream_y = y;
}

/* ------------------------------------------------------------------------ */
static uint32_t libattopng_swap32(uint32_t num) {
    return ((num >> 24) & 0xff) |
           ((num << 8) & 0xff0000) |
           ((num >> 8) & 0xff00) |
           ((num << 24) & 0xff000000);
}

/* ------------------------------------------------------------------------ */
static uint32_t libattopng_crc(const unsigned char *data, size_t len, uint32_t crc) {
    size_t i;
    for (i = 0; i < len; i++) {
        crc = libattopng_crc32[(crc ^ data[i]) & 255] ^ (crc >> 8);
    }
    return crc;
}

/* ------------------------------------------------------------------------ */
static void libattopng_out_raw_write(libattopng_t *png, const char *data, size_t len) {
    size_t i;
    for (i = 0; i < len; i++) {
        png->out[png->out_pos++] = data[i];
    }
}

/* ------------------------------------------------------------------------ */
static void libattopng_out_raw_uint(libattopng_t *png, uint32_t val) {
    *(uint32_t *) (png->out + png->out_pos) = val;
    png->out_pos += 4;
}

/* ------------------------------------------------------------------------ */
static void libattopng_out_raw_uint16(libattopng_t *png, uint16_t val) {
    *(uint16_t *) (png->out + png->out_pos) = val;
    png->out_pos += 2;
}

/* ------------------------------------------------------------------------ */
static void libattopng_out_raw_uint8(libattopng_t *png, uint8_t val) {
    *(uint8_t *) (png->out + png->out_pos) = val;
    png->out_pos++;
}

/* ------------------------------------------------------------------------ */
static void libattopng_new_chunk(libattopng_t *png, const char *name, size_t len) {
    png->crc = 0xffffffff;
    libattopng_out_raw_uint(png, libattopng_swap32((uint32_t) len));
    png->crc = libattopng_crc((const unsigned char *) name, 4, png->crc);
    libattopng_out_raw_write(png, name, 4);
}

/* ------------------------------------------------------------------------ */
static void libattopng_end_chunk(libattopng_t *png) {
    libattopng_out_raw_uint(png, libattopng_swap32(~png->crc));
}

/* ------------------------------------------------------------------------ */
static void libattopng_out_uint32(libattopng_t *png, uint32_t val) {
    png->crc = libattopng_crc((const unsigned char *) &val, 4, png->crc);
    libattopng_out_raw_uint(png, val);
}

/* ------------------------------------------------------------------------ */
static void libattopng_out_uint16(libattopng_t *png, uint16_t val) {
    png->crc = libattopng_crc((const unsigned char *) &val, 2, png->crc);
    libattopng_out_raw_uint16(png, val);
}

/* ------------------------------------------------------------------------ */
static void libattopng_out_uint8(libattopng_t *png, uint8_t val) {
    png->crc = libattopng_crc((const unsigned char *) &val, 1, png->crc);
    libattopng_out_raw_uint8(png, val);
}

/* ------------------------------------------------------------------------ */
static void libattopng_out_write(libattopng_t *png, const char *data, size_t len) {
    png->crc = libattopng_crc((const unsigned char *) data, len, png->crc);
    libattopng_out_raw_write(png, data, len);
}

/* ------------------------------------------------------------------------ */
static void libattopng_out_write_adler(libattopng_t *png, unsigned char data) {
    libattopng_out_write(png, (char *) &data, 1);
    png->s1 = (uint16_t) ((png->s1 + data) % LIBATTOPNG_ADLER_BASE);
    png->s2 = (uint16_t) ((png->s2 + png->s1) % LIBATTOPNG_ADLER_BASE);
}

/* ------------------------------------------------------------------------ */
static void libattopng_pixel_header(libattopng_t *png, size_t offset, size_t bpl) {
    if (offset > bpl) {
        /* not the last line */
        libattopng_out_write(png, "\0", 1);
        libattopng_out_uint16(png, (uint16_t) bpl);
        libattopng_out_uint16(png, (uint16_t) ~bpl);
    } else {
        /* last line */
        libattopng_out_write(png, "\1", 1);
        libattopng_out_uint16(png, (uint16_t) offset);
        libattopng_out_uint16(png, (uint16_t) ~offset);
    }
}

/* ------------------------------------------------------------------------ */
char *libattopng_get_data(libattopng_t *png, size_t *len) {
    size_t index, bpl, raw_size, size, p, pos, corr;
    unsigned char *pixel;
    if (!png) {
        return NULL;
    }
    if (png->out) {
        /* delete old output if any */
        free(png->out);
    }
    png->out_capacity = png->capacity + 4096 * 8 + png->width * png->height;
    png->out = (char *) calloc(png->out_capacity, 1);
    png->out_pos = 0;
    if (!png->out) {
        return NULL;
    }

    libattopng_out_raw_write(png, "\211PNG\r\n\032\n", 8);

    /* IHDR */
    libattopng_new_chunk(png, "IHDR", 13);
    libattopng_out_uint32(png, libattopng_swap32((uint32_t) (png->width)));
    libattopng_out_uint32(png, libattopng_swap32((uint32_t) (png->height)));
    libattopng_out_uint8(png, 8); /* bit depth */
    libattopng_out_uint8(png, (uint8_t) png->type);
    libattopng_out_uint8(png, 0); /* compression */
    libattopng_out_uint8(png, 0); /* filter */
    libattopng_out_uint8(png, 0); /* interlace method */
    libattopng_end_chunk(png);

    /* palette */
    if (png->type == PNG_PALETTE) {
        char entry[3];
        size_t s = png->palette_length;
        if (s < 16) {
            s = 16; /* minimum palette length */
        }
        libattopng_new_chunk(png, "PLTE", 3 * s);
        for (index = 0; index < s; index++) {
            entry[0] = (char) (png->palette[index] & 255);
            entry[1] = (char) ((png->palette[index] >> 8) & 255);
            entry[2] = (char) ((png->palette[index] >> 16) & 255);
            libattopng_out_write(png, entry, 3);
        }
        libattopng_end_chunk(png);

        /* transparency */
        libattopng_new_chunk(png, "tRNS", s);
        for (index = 0; index < s; index++) {
            entry[0] = (char) ((png->palette[index] >> 24) & 255);
            libattopng_out_write(png, entry, 1);
        }
        libattopng_end_chunk(png);
    }

    /* data */
    bpl = 1 + png->bpp * png->width;
    if(bpl >= 65536) {
        fprintf(stderr, "[libattopng] ERROR: maximum supported width for this type of PNG is %d pixel\n", (int)(65535 / png->bpp));
        return NULL;
    }
    raw_size = png->height * bpl;
    size = 2 + png->height * (5 + bpl) + 4;
    libattopng_new_chunk(png, "IDAT", size);
    libattopng_out_write(png, "\170\332", 2);

    pixel = (unsigned char *) png->data;
    png->s1 = 1;
    png->s2 = 0;
    index = 0;
    if (png->type == PNG_RGB) {
        corr = 1;
    } else {
        corr = 0;
    }
    for (pos = 0; pos < png->width * png->height; pos++) {
        if (index == 0) {
            /* line header */
            libattopng_pixel_header(png, raw_size, bpl);
            libattopng_out_write_adler(png, 0); /* no filter */
            raw_size--;
        }

        /* pixel */
        for (p = 0; p < png->bpp; p++) {
            libattopng_out_write_adler(png, *pixel);
            pixel++;
        }
        pixel += corr;

        raw_size -= png->bpp;
        index = (index + 1) % png->width;
    }
    /* checksum */
    png->s1 %= LIBATTOPNG_ADLER_BASE;
    png->s2 %= LIBATTOPNG_ADLER_BASE;
    libattopng_out_uint32(png, libattopng_swap32((uint32_t) ((png->s2 << 16) | png->s1)));
    libattopng_end_chunk(png);

    /* end of image */
    libattopng_new_chunk(png, "IEND", 0);
    libattopng_end_chunk(png);

    if (len) {
        *len = png->out_pos;
    }
    return png->out;
}

/* ------------------------------------------------------------------------ */
int libattopng_save(libattopng_t *png, const char *filename) {
    size_t len;
    FILE* f;
    char *data = libattopng_get_data(png, &len);
    if (!data) {
        return 1;
    }
    f = fopen(filename, "wb");
    if (!f) {
        return 1;
    }
    if (fwrite(data, len, 1, f) != 1) {
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}

/* ------------------------------------------------------------------------ */
void libattopng_destroy(libattopng_t *png) {
    if (!png) {
        return;
    }
    free(png->palette);
    png->palette = NULL;
    free(png->out);
    png->out = NULL;
    free(png->data);
    png->data = NULL;
    free(png);
}
