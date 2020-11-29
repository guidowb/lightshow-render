#include "Renderers.h"

#include <math.h>
#include <sys/time.h>

extern long millis();

Renderer::~Renderer() {}

static long random(long seed) {
	long r = seed * 179426323.0;
    printf("random %ld\n", r);
    return r;
}

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

TwinkleRenderer::TwinkleRenderer(RGBA color) {
	this->cycle_total = 10000; // 10 seconds
	this->cycle_brighten = 100; // 1/10th second
	this->cycle_dim = 1000; // 1 second
    this->color = color;
}

void TwinkleRenderer::render(Canvas *canvas) {
    for (int p = 0; p < canvas->getSize(); p++) {
        long ptime = (millis() + random(p)) % cycle_total;
        printf("twinkle - pixel %d time %ld\n", p, ptime);
        if (ptime < cycle_brighten) {
            printf("twinkle - pixel %d brightening\n", p);
            int alpha = ((double) ptime / cycle_brighten) * (this->color & 0x0ff);
            RGBA color = (this->color & 0xffffff00) + alpha;
            canvas->setPixel(p, color);
        }
        else if (ptime < (cycle_dim + cycle_brighten)) {
            printf("twinkle - pixel %d dimming\n", p);
            ptime -= cycle_brighten;
            int alpha = (1.0 - (double) ptime / cycle_dim) * (this->color & 0x0ff);
            RGBA color = (this->color & 0xffffff00) + alpha;
            canvas->setPixel(p, color);
        }
    }
}