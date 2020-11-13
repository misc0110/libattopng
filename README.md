# libattopng
[![Build Status](https://travis-ci.com/misc0110/libattopng.svg?branch=master)](https://travis-ci.com/misc0110/libattopng)

`libattopng` is a minimal C library to create uncompressed PNG images.
It is cross-platform compatible, has no dependencies and a very small footprint of less than 10kB.
The library supports palette, grayscale as well as raw RGB images all with and without transparency.

# Usage

Using `libattopng` is as simple as adding both `libattopng.h` and `libattopng.c` to your project. Independent of the used standard (from C89 to C11), the code will compile without warnings. The library does not require any special flags to compile. 

## Example

### Color gradient with alpha
```C
#define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))

libattopng_t* png = libattopng_new(250, 200, PNG_RGBA);

int x, y;
for (y = 0; y < 200; y++) {
  for (x = 0; x < 250; x++) {
    libattopng_set_pixel(png, x, y, RGBA(x & 255, y & 255, 128, (255 - ((x / 2) & 255))));
  }
}
libattopng_save(png, "test_rgba.png");
libattopng_destroy(png);
```

### Stripes with palette and alpha
```C
#define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))

// ceate palette image
libattopng_t *png = libattopng_new(256, 256, PNG_PALETTE);
uint32_t palette[] = {RGBA(0, 0, 0xff, 0xff), RGBA(0, 0xff, 0, 0x80), RGBA(0xff, 0, 0, 0xff), RGBA(0xff, 0, 0xff, 0x80)};
// 4 colors: blue, green (50% alpha), red, cyan (50% alpha) 
libattopng_set_palette(png, palette, 4);

int x, y;
for (y = 0; y < 256; y++) {
  for (x = 0; x < 256; x++) {
    libattopng_set_pixel(png, x, y, (x % 16) / 4);
  }
}

libattopng_save(png, "test_palette.png");
libattopng_destroy(png);
```

### More examples
See `test.c` for more examples.

# API documentation

### Brief content:

**Functions**:

> [`libattopng_new`](#function-libattopng_new): Create a new, empty PNG image to be used with all other functions.

> [`libattopng_destroy`](#function-libattopng_destroy): Destroys the reference to a PNG image and free all associated memory.

> [`libattopng_set_palette`](#function-libattopng_set_palette): Sets the image's palette if the image type is PNG_PALETTE.

> [`libattopng_set_pixel`](#function-libattopng_set_pixel): Sets the pixel's color at the specified position

> [`libattopng_get_pixel`](#function-libattopng_get_pixel): Returns the pixel's color at the specified position

> [`libattopng_start_stream`](#function-libattopng_start_stream): Set the start position for a batch of pixels

> [`libattopng_put_pixel`](#function-libattopng_put_pixel): Sets the pixel of the current pixel within a stream and advances to the next pixel

> [`libattopng_get_data`](#function-libattopng_get_data): Returns the image as PNG data stream

> [`libattopng_save`](#function-libattopng_save): Saves the image as a PNG file

## Functions

The module `libattopng.h` defines the following functions.

### Function `libattopng_new`

Create a new, empty PNG image to be used with all other functions.

> It's the callers responsibility to free the data structure. See  [`libattopng_destroy`](#function-libattopng_destroy)

**Parameters:**

- `width`: The width of the image in  pixels
- `height`: The height of the image in pixels
- `type`: The type of image. Possible values are

        - PNG_GRAYSCALE (8bit grayscale),
        - PNG_GRAYSCALE_ALPHA (8bit grayscale with 8bit alpha),
        - PNG_PALETTE (palette with up to 256 entries, each 32bit RGBA)
        - PNG_RGB (24bit RGB values)
        - PNG_RGBA (32bit RGB values with alpha)

**Returns:**

- reference to a PNG image to be used with all other functions or NULL on error.
        Possible errors are:
        - Out of memory
        - Width and height combined exceed the maximum integer size

### Function `libattopng_destroy`

Destroys the reference to a PNG image and free all associated memory.

**Parameters:**

- `png`: Reference to the image

**Returns:**

No return

### Function `libattopng_set_palette`

Sets the image's palette if the image type is PNG_PALETTE.

**Parameters:**

- `png`: Reference to the image
- `palette`: Color palette, each entry contains a 32bit RGBA value
- `length`: Number of palette entries

**Returns:**

- 0 on success, 1 if the palette contained more than 256 entries

### Function `libattopng_set_pixel`

Sets the pixel's color at the specified position

**Parameters:**

- `png`: Reference to the image
- `x`: X coordinate
- `y`: Y coordinate
- `color`: The pixel value, depending on the type this is

        - the 8bit palette index (PNG_PALETTE)
        - the 8bit gray value (PNG_GRAYSCALE)
        - a 16bit value where the lower 8bit are the gray value and
          the upper 8bit are the opacity (PNG_GRAYSCALE_ALPHA)
        - a 24bit RGB value (PNG_RGB)
        - a 32bit RGBA value (PNG_RGBA)

> If the coordinates are not within the bounds of the image, the functions does nothing.

**Returns:**

No return

### Function `libattopng_get_pixel`

Returns the pixel's color at the specified position

**Parameters:**

- `png`: Reference to the image
- `x`: X coordinate
- `y`: Y coordinate

**Returns:**

- The pixel value, depending on the type this is

        - the 8bit palette index (PNG_PALETTE)
        - the 8bit gray value (PNG_GRAYSCALE)
        - a 16bit value where the lower 8bit are the gray value and
          the upper 8bit are the opacity (PNG_GRAYSCALE_ALPHA)
        - a 24bit RGB value (PNG_RGB)
        - a 32bit RGBA value (PNG_RGBA)
        - 0 if the coordinates are out of bounds

### Function `libattopng_start_stream`

Set the start position for a batch of pixels

**Parameters:**

- `png`: Reference to the image
- `x`: X coordinate
- `y`: Y coordinate

**Returns:**

No return

**See:**

[`libattopng_put_pixel`](#f-libattopng_put_pixel)

### Function `libattopng_put_pixel`

Sets the pixel of the current pixel within a stream and advances to the next pixel

**Parameters:**

- `png`: Reference to the image
- `color`: The pixel value, depending on the type this is

        - the 8bit palette index (PNG_PALETTE)
        - the 8bit gray value (PNG_GRAYSCALE)
        - a 16bit value where the lower 8bit are the gray value and
          the upper 8bit are the opacity (PNG_GRAYSCALE_ALPHA)
        - a 24bit RGB value (PNG_RGB)
        - a 32bit RGBA value (PNG_RGBA)

**Returns:**

No return

### Function `libattopng_get_data`

Returns the image as PNG data stream

**Parameters:**

- `png`: Reference to the image
- `len`: The length of the data stream is written to this output parameter

**Returns:**

- A reference to the PNG output stream
> The data stream is free'd when calling \ref libattopng_destroy and
must not be free'd be the caller

### Function `libattopng_save`

Saves the image as a PNG file

**Parameters:**

- `png`: Reference to the image
- `filename`: Name of the file

**Returns:**

- 0 on success, 1 on error
