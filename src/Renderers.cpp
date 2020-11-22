#include "Renderers.h"

Renderer::~Renderer() {

}

SolidRenderer::SolidRenderer(RGBA color) {
    this->color = color;
}

void SolidRenderer::render(Canvas *canvas) {
    for (int p = 0; p < canvas->getSize(); p++) canvas->setPixel(p, color);
}
