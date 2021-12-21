#include "Canvasses.h"

uint32_t Canvas::sceneTime() { return globalTime(); }
uint32_t Canvas::epochTime() { return 0; }

MappedCanvas::MappedCanvas(Canvas *canvas) {
    this->canvas = canvas;
}

uint16_t MappedCanvas::getSize() {
    return canvas->getSize();
}

uint32_t MappedCanvas::globalTime() {
    return canvas->globalTime();
}

uint32_t MappedCanvas::sceneTime() {
    return canvas->sceneTime();
}

void MappedCanvas::setPixel(uint16_t pixel, RGBA color) {
    canvas->setPixel(pixel, color);
}
