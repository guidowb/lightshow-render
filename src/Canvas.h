#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RGBA;

class Canvas {
    virtual int getSize() = 0;
    virtual int getTimestamp() = 0;
    virtual void setPixel(int pixel, RGBA color) = 0;
};

class Renderer {
    virtual void render(Canvas canvas) = 0;
};
