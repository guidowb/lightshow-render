#include "Renderers.h"

#include <math.h>

#define printf(fmt, ...) {}

Renderer::~Renderer() {}

// This, of course, isn't truly random. We're not doing security here.
// We need something that looks random but is perfectly predictable, so
// that, when we calculate consecutive frames, we get identical "random"
// values for each pixel.

static uint32_t random(uint32_t seed) {
	uint32_t r = seed * 179426323;
    printf("random %ld\n", r);
    return r;
}

NullRenderer::NullRenderer() {}

void NullRenderer::render(Canvas *canvas) {}

LayeredRenderer::LayeredRenderer(Renderer *thisLayer, Renderer *nextLayer) {
    this->thisLayer = thisLayer;
    this->nextLayer = nextLayer;
}

LayeredRenderer::~LayeredRenderer() {
    if (this->thisLayer) delete this->thisLayer;
    if (this->nextLayer) delete this->nextLayer;
}

void LayeredRenderer::render(Canvas *canvas) {
    if (this->thisLayer) this->thisLayer->render(canvas);
    if (this->nextLayer) this->nextLayer->render(canvas);
}

SolidRenderer::SolidRenderer(RGBA color) {
    this->color = color;
}

void SolidRenderer::render(Canvas *canvas) {
    printf("render solid");
    for (int p = 0; p < canvas->getSize(); p++) canvas->setPixel(p, color);
}

DotsRenderer::DotsRenderer(int spacing, Vector<RGBA> &colors) {
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

TwinkleRenderer::TwinkleRenderer(RGBA color, int tpm) {

    // Any given pixel twinkles every cycle_total (N) millis.
    // It takes cycle_brighten (K) time to brighten, and
    // cycle_dim (M) time to dim.
    //
    // Full bright is white (A).
    // Dim is 100% (B) of given color.
    //
    // Given a pixel and its start time
    // if T < K brightness is increasing
    // if K < T < M+K brightness is dimming
    // else brightness is dim
    //
    // Given pixel p, its start time is determined by multiplying
    // its position by some really big prime. That is the part that
    // spreads the twinkle across the whole pattern evenly but
    // seemingly randomly.
    //
    // The number of twinkles across the entire patterns is
    // controlled by the total cycle time for each pixel. It's
    // parameterized by specifying the desired number of twinkles
    // per minute across the entire pattern.

	this->twinkles_per_minute = tpm ? tpm : 300; // 5 per second
	this->cycle_brighten = 100; // 1/10th second
	this->cycle_dim = 1000; // 1 second
    this->color = color;
}

void TwinkleRenderer::render(Canvas *canvas) {

    uint32_t cycle_total = (60000 * canvas->getSize()) / this->twinkles_per_minute;
    for (int p = 0; p < canvas->getSize(); p++) {
        uint32_t ptime = (canvas->getTime() + random(p)) % cycle_total;
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

SegmentRenderer::SegmentRenderer(int from, int to, Renderer *renderer) {
    this->from  = from;
    this->to = to;
    this->renderer = renderer;
}

SegmentRenderer::~SegmentRenderer() {
    delete this->renderer;
}

void SegmentRenderer::render(Canvas *canvas) {

    class SegmentCanvas : public Canvas {
    public:
        SegmentCanvas(Canvas *parent, int from, int to) { this->parent = parent; this->from = from; this->to = to; }
        virtual int getSize() { return to - from; }
        virtual void setPixel(int pixel, RGBA color) { parent->setPixel(pixel + from, color); }
        virtual long getTime() { return parent->getTime(); }

    private:
        Canvas *parent;
        int from;
        int to;
    };

    SegmentCanvas segment(canvas, from, to);
    renderer->render(&segment);
}

GradientRenderer::GradientRenderer(Vector<RGBA> &colors) {
    this->ncolors = colors.size();
    this->color = new RGBA[this->ncolors];
    for (int p = 0; p < this->ncolors; p++) this->color[p] = colors[p];
}

GradientRenderer::~GradientRenderer() {
    delete this->color;
}

void GradientRenderer::render(Canvas *canvas) {
    int segments = ncolors - 1;
    int interval = canvas->getSize() / segments;
    for (int p = 0; p < canvas->getSize(); p++) {
        int i1 = (p / interval) % ncolors;
        int i2 = (i1 + 1) % ncolors;
        int fraction = p % interval;
		RGBA c1 = color[i1];
        uint8_t r1 = (c1 >> 24) & 0x0ff;
        uint8_t g1 = (c1 >> 16) & 0x0ff;
        uint8_t b1 = (c1 >>  8) & 0x0ff;
        uint8_t a1 = (c1      ) & 0x0ff;
		RGBA c2 = color[i2];
        uint8_t r2 = (c2 >> 24) & 0x0ff;
        uint8_t g2 = (c2 >> 16) & 0x0ff;
        uint8_t b2 = (c2 >>  8) & 0x0ff;
        uint8_t a2 = (c2      ) & 0x0ff;
        int r = (r1 * (interval - fraction) + r2 * fraction) / interval;
        int g = (g1 * (interval - fraction) + g2 * fraction) / interval;
        int b = (b1 * (interval - fraction) + b2 * fraction) / interval;
        int a = (a1 * (interval - fraction) + a2 * fraction) / interval;
        RGBA c3 = (r << 24) + (g << 16) + (b << 8) + a;
        canvas->setPixel(p, c3);
    }
}
