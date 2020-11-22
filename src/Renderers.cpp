#include "Renderers.h"

Renderer::~Renderer() {

}

SolidRenderer::SolidRenderer(RGBA color) {
    this->color = color;
}

void SolidRenderer::render(Canvas *convas) {

}
