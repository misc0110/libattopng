/**
 * @file libattopng.h
 * @brief A minimal C library to write uncompressed PNG files.
 *
 * libattopng is a minimal C library to create uncompressed PNG images.
 * It is cross-platform compatible, has no dependencies and a very small footprint.
 * The library supports palette, grayscale as well as raw RGB images all with and without transparency.
 *
 * @author Michael Schwarz
 * @date 29 Jan 2017
 */

#ifndef _LIBATTOPNG_H_
#define _LIBATTOPNG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

/**
 * @brief PNG type.
 *
 * The type of PNG image. It determines how the pixels are stored.
 */
typedef enum {
    PNG_GRAYSCALE = 0,          /**< 256 shades of gray, 8bit per pixel  */
    PNG_RGB = 2,                /**< 24bit RGB values */
    PNG_PALETTE = 3,            /**< Up to 256 RGBA palette colors, 8bit per pixel */
    PNG_GRAYSCALE_ALPHA = 4,    /**< 256 shades of gray plus alpha channel, 16bit per pixel */
    PNG_RGBA = 6                /**< 24bit RGB values plus 8bit alpha channel */
} libattopng_type_t;


/**
 * @brief Reference to a PNG image
 *
 * This struct holds the internal state of the PNG. The members should never be used directly.
 */
typedef struct {
    libattopng_type_t type;      /**< File type */
    size_t capacity;             /**< Reserved memory for raw data */
    char *data;                  /**< Raw pixel data, format depends on type */
    uint32_t *palette;           /**< Palette for image */
    size_t palette_length;       /**< Entries for palette, 0 if unused */
    size_t width;                /**< Image width */
    size_t height;               /**< Image height */

    char *out;                   /**< Buffer to store final PNG */
    size_t out_pos;              /**< Current size of output buffer */
    size_t out_capacity;         /**< Capacity of output buffer */
    uint32_t crc;                /**< Currecnt CRC32 checksum */
    uint16_t s1;                 /**< Helper variables for Adler checksum */
    uint16_t s2;                 /**< Helper variables for Adler checksum */
    size_t bpp;                  /**< Bytes per pixel */

    size_t stream_x;             /**< Current x coordinate for pixel streaming */
    size_t stream_y;             /**< Current y coordinate for pixel streaming */
} libattopng_t;


/**
 * @function libattopng_new
 *
 * @brief Create a new, empty PNG image to be used with all other functions.
 *
 * @param width The width of the image in  pixels
 * @param height The height of the image in pixels
 * @param type The type of image. Possible values are
 *                  - PNG_GRAYSCALE (8bit grayscale),
 *                  - PNG_GRAYSCALE_ALPHA (8bit grayscale with 8bit alpha),
 *                  - PNG_PALETTE (palette with up to 256 entries, each 32bit RGBA)
 *                  - PNG_RGB (24bit RGB values)
 *                  - PNG_RGBA (32bit RGB values with alpha)
 * @return reference to a PNG image to be used with all other functions or NULL on error.
 *          Possible errors are:
 *              - Out of memory
 *              - Width and height combined exceed the maximum integer size
 * @note It's the callers responsibility to free the data structure.
 *       See @ref libattopng_destroy
 */
libattopng_t *libattopng_new(size_t width, size_t height, libattopng_type_t type);


/**
 * @function libattopng_destroy
 *
 * @brief Destroys the reference to a PNG image and free all associated memory.
 *
 * @param png Reference to the image
 *
 */
void libattopng_destroy(libattopng_t *png);


/**
 * @function libattopng_set_palette
 *
 * @brief Sets the image's palette if the image type is \ref PNG_PALETTE.
 *
 * @param png      Reference to the image
 * @param palette  Color palette, each entry contains a 32bit RGBA value
 * @param length   Number of palette entries
 * @return 0 on success, 1 if the palette contained more than 256 entries
 */
int libattopng_set_palette(libattopng_t *png, uint32_t *palette, size_t length);


/**
 * @function libattopng_set_pixel
 *
 * @brief Sets the pixel's color at the specified position
 *
 * @param png    Reference to the image
 * @param x      X coordinate
 * @param y      Y coordinate
 * @param color The pixel value, depending on the type this is
 *              - the 8bit palette index (\ref PNG_PALETTE)
 *              - the 8bit gray value (\ref PNG_GRAYSCALE)
 *              - a 16bit value where the lower 8bit are the gray value and
 *                the upper 8bit are the opacity (\ref PNG_GRAYSCALE_ALPHA)
 *              - a 24bit RGB value (\ref PNG_RGB)
 *              - a 32bit RGBA value (\ref PNG_RGBA)
 * @note If the coordinates are not within the bounds of the image,
 *       the functions does nothing.
 */
void libattopng_set_pixel(libattopng_t *png, size_t x, size_t y, uint32_t color);


/**
 * @function libattopng_get_pixel
 *
 * @brief Returns the pixel's color at the specified position
 *
 * @param png   Reference to the image
 * @param x     X coordinate
 * @param y     Y coordinate
 * @return      The pixel value, depending on the type this is
 *              - the 8bit palette index (\ref PNG_PALETTE)
 *              - the 8bit gray value (\ref PNG_GRAYSCALE)
 *              - a 16bit value where the lower 8bit are the gray value and
 *                the upper 8bit are the opacity (\ref PNG_GRAYSCALE_ALPHA)
 *              - a 24bit RGB value (\ref PNG_RGB)
 *              - a 32bit RGBA value (\ref PNG_RGBA)
 *              - 0 if the coordinates are out of bounds
 */
uint32_t libattopng_get_pixel(libattopng_t *png, size_t x, size_t y);


/**
 * @function libattopng_start_stream
 *
 * @brief Set the start position for a batch of pixels
 *
 * @param png  Reference to the image
 * @param x    X coordinate
 * @param y    Y coordinate
 *
 * @see libattopng_put_pixel
 */
void libattopng_start_stream(libattopng_t *png, size_t x, size_t y);


/**
 * @function libattopng_put_pixel
 *
 * @brief Sets the pixel of the current pixel within a stream and advances to the next pixel
 *
 * @param png   Reference to the image
 * @param color The pixel value, depending on the type this is
 *              - the 8bit palette index (\ref PNG_PALETTE)
 *              - the 8bit gray value (\ref PNG_GRAYSCALE)
 *              - a 16bit value where the lower 8bit are the gray value and
 *                the upper 8bit are the opacity (\ref PNG_GRAYSCALE_ALPHA)
 *              - a 24bit RGB value (\ref PNG_RGB)
 *              - a 32bit RGBA value (\ref PNG_RGBA)
 */
void libattopng_put_pixel(libattopng_t *png, uint32_t color);


/**
 * @function libattopng_get_data
 *
 * @brief Returns the image as PNG data stream
 *
 * @param png  Reference to the image
 * @param len  The length of the data stream is written to this output parameter
 * @return A reference to the PNG output stream
 * @note The data stream is free'd when calling \ref libattopng_destroy and
 *       must not be free'd be the caller
 */
char *libattopng_get_data(libattopng_t *png, size_t *len);


/**
 * @function libattopng_save
 *
 * @brief Saves the image as a PNG file
 *
 * @param png      Reference to the image
 * @param filename Name of the file
 * @return 0 on success, 1 on error
 */
int libattopng_save(libattopng_t *png, const char *filename);


#ifdef __cplusplus
}
#endif
#endif

