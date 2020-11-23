#include "Renderers.h"

Renderer::~Renderer() {

}

SolidRenderer::SolidRenderer(RGBA color) {
    this->color = color;
}

void SolidRenderer::render(Canvas *canvas) {
    for (int p = 0; p < canvas->getSize(); p++) canvas->setPixel(p, color);
}

DotsRenderer::DotsRenderer(int spacing, std::vector<RGBA> &colors) {
    this->spacing = spacing;
    this->ncolors = colors.size();
    this->color = new RGBA[this->ncolors];
    for (int p = 0; p < this->ncolors; p++) this->color[p] = colors[p];
}

DotsRenderer::~DotsRenderer() {
    delete this->color;
}

void DotsRenderer::render(Canvas *canvas) {
    int c = 0;
    for (int p = 0; p < canvas->getSize(); p += spacing) {
        canvas->setPixel(p, color[c]);
        c = (c + 1) % ncolors;
    }
}
