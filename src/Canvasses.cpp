#include "Canvasses.h"

uint32_t Canvas::sceneTime() { return globalTime(); }
uint32_t Canvas::epochTime() { return 0; }

MappedCanvas::MappedCanvas(Canvas *parent) {
    this->parent = parent;
}

uint16_t MappedCanvas::getSize() {
    return parent->getSize();
}

uint32_t MappedCanvas::globalTime() {
    return parent->globalTime();
}

uint32_t MappedCanvas::sceneTime() {
    return parent->sceneTime();
}

void MappedCanvas::setPixel(uint16_t pixel, RGBA color) {
    parent->setPixel(pixel, color);
}
