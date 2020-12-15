#include <stdlib.h>

#include "LightShow.h"
#include "Compiler.h"

void render(Renderer *renderer, Canvas *canvas) {
    ((Renderer *) renderer)->render(canvas);
}

Renderer *compile(const char *sourceName, const char *pattern) {
    Compiler compiler(sourceName, pattern);
    Renderer *renderer = compiler.compile();
    return renderer;
}

RGBA::RGBA() : r(0), g(0), b(0), a(0) {}

RGBA::RGBA(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void RGBA::operator+=(const RGBA other) {
  this->r = ((this->r * (255 - other.a)) + (other.r * other.a)) / 255;
  this->g = ((this->g * (255 - other.a)) + (other.g * other.a)) / 255;
  this->b = ((this->b * (255 - other.a)) + (other.b * other.a)) / 255;
  this->a = ((this->a * (255 - other.a)) + (other.a * other.a)) / 255;
}

RGBA RGBA::operator+(const RGBA other) const {
    return RGBA(
        ((this->r * (255 - other.a)) + (other.r * other.a)) / 255,
        ((this->g * (255 - other.a)) + (other.g * other.a)) / 255,
        ((this->b * (255 - other.a)) + (other.b * other.a)) / 255,
        ((this->a * (255 - other.a)) + (other.a * other.a)) / 255
    );
}

bool RGBA::operator==(const RGBA other) const {
    return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
}

RGBA::operator uint32_t() const {
    return ((this->r << 24) & 0xff000000) | ((this->g << 16) & 0x00ff0000) | ((this->b << 8) & 0x0ff00) | (this->a & 0x0ff);
}

RGBA RGBA::blend(const RGBA other, uint32_t fraction) const {
    return RGBA(
        ((this->r * (1000 - fraction)) + (other.r * fraction)) / 1000,
        ((this->g * (1000 - fraction)) + (other.g * fraction)) / 1000,
        ((this->b * (1000 - fraction)) + (other.b * fraction)) / 1000,
        this->a
    );
}
