#include <libattopng.h>
#include <stdio.h>


#define W 256
#define H 256

int main(int argc, char *argv[]) {
    libattopng_t* png = libattopng_new(W, H, PNG_PALETTE);
    uint32_t palette[] = {0xffff0000, 0x8000ff00, 0xff0000ff, 0x80ff00ff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    libattopng_set_palette(png, palette, 16);

    int x, y;
    for(y = 0; y < H; y++) {
        for(x = 0; x < W; x++) {
            libattopng_set_pixel_palette(png, x, y, ((x + y) % 16) / 4);
        }
    }

    libattopng_save(png, "test.png");
    libattopng_destroy(png);
    return 0;
}
