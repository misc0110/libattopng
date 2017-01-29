#include <libattopng.h>
#include <stdio.h>


#define W 300
#define H 200

int main(int argc, char *argv[]) {
    libattopng_t* png = libattopng_new(W, H, PNG_PALETTE);
    uint32_t palette[] = {0xffff0000, 0x8000ff00, 0xff0000ff, 0x80ff00ff};
    libattopng_set_palette(png, palette, 4);

    int x, y;
    for(y = 0; y < H; y++) {
        for(x = 0; x < W; x++) {
            libattopng_set_pixel(png, x, y, ((x + y) % 16) / 4);
        }
    }

    libattopng_save(png, "test.png");
    libattopng_destroy(png);

    // -----------------

    printf("-----------------------\n");

    png = libattopng_new(W, H, PNG_RGBA);

    for(y = 0; y < H; y++) {
        for(x = 0; x < W; x++) {
            libattopng_set_pixel(png, x, y, palette[((x + y) % 16) / 4]);
        }
    }
    libattopng_save(png, "test1.png");
    libattopng_destroy(png);
    return 0;
}
