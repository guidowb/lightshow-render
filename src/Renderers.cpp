#include "Renderers.h"

Renderer::~Renderer() {}

NullRenderer::NullRenderer() {}

void NullRenderer::render(Canvas *canvas) {}

BlockRenderer::BlockRenderer(std::vector<Renderer *> &renderers) {
    this->nrenderers = renderers.size();
    this->renderer = new Renderer *[this->nrenderers];
    for (int p = 0; p < this->nrenderers; p++) this->renderer[p] = renderers[p];
}

BlockRenderer::~BlockRenderer() {
    for (int p = 0; p < this->nrenderers; p++) delete this->renderer[p];
    delete this->renderer;
}

void BlockRenderer::render(Canvas *canvas) {
    printf("render block of %d", nrenderers);
    for (int p = 0; p < this->nrenderers; p++) this->renderer[p]->render(canvas);
}

SolidRenderer::SolidRenderer(RGBA color) {
    this->color = color;
}

void SolidRenderer::render(Canvas *canvas) {
    printf("render solid");
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
    printf("render dots with %d", ncolors);
    int c = 0;
    for (int p = 0; p < canvas->getSize(); p += spacing) {
        canvas->setPixel(p, color[c]);
        c = (c + 1) % ncolors;
    }
}
