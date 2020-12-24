#include "Serialize.h"
#include "Renderers.h"

void NullRenderer::serialize(Serializer &serializer) {
    serializer.startObject("NullRenderer");
    serializer.endObject();
}

void LayeredRenderer::serialize(Serializer &serializer) {
    serializer.startObject("LayeredRenderer");
    serializer.addProperty("thisLayer", thisLayer);
    serializer.addProperty("nextLayer", nextLayer);
    serializer.endObject();
}

void DuplicateRenderer::serialize(Serializer &serializer) {
    serializer.startObject("DuplicateRenderer");
    serializer.addProperty("layer", layer);
    serializer.endObject();
}

void SolidRenderer::serialize(Serializer &serializer) {
    serializer.startObject("SolidRenderer");
    serializer.addProperty("color", color);
    serializer.endObject();
}

void DotsRenderer::serialize(Serializer &serializer) {
    serializer.startObject("DotsRenderer");
    serializer.addProperty("spacing", spacing);
    serializer.addProperty("colors", ncolors, color);
    serializer.endObject();
}

void TwinkleRenderer::serialize(Serializer &serializer) {
    serializer.startObject("TwinkleRenderer");
    serializer.addProperty("twinkles_per_minute", twinkles_per_minute);
    serializer.addProperty("cycle_brighten", cycle_brighten);
    serializer.addProperty("cycle_dim", cycle_dim);
    serializer.addProperty("color", color);
    serializer.endObject();
}

void SegmentRenderer::serialize(Serializer &serializer) {
    serializer.startObject("SegmentRenderer");
    serializer.addProperty("from", from);
    serializer.addProperty("to", to);
    serializer.addProperty("renderer", renderer);
    serializer.endObject();
}

void GradientRenderer::serialize(Serializer &serializer) {
    serializer.startObject("GradientRenderer");
    serializer.addProperty("colors", ncolors, color);
    serializer.endObject();
}

void FadeRenderer::serialize(Serializer &serializer) {
    serializer.startObject("FadeRenderer");
    serializer.addProperty("before", before);
    serializer.addProperty("after", after);
    serializer.addProperty("duration", duration);
    serializer.endObject();
}

void WipeRenderer::serialize(Serializer &serializer) {
    serializer.startObject("WipeRenderer");
    serializer.addProperty("before", before);
    serializer.addProperty("after", after);
    serializer.addProperty("duration", duration);
    serializer.endObject();
}

void AfterRenderer::serialize(Serializer &serializer) {
    serializer.startObject("AfterRenderer");
    serializer.addProperty("before", before);
    serializer.addProperty("after", after);
    serializer.addProperty("duration", duration);
    serializer.endObject();
}

void RepeatRenderer::serialize(Serializer &serializer) {
    serializer.startObject("RepeatRenderer");
    serializer.addProperty("clockSkew", clockSkew);
    serializer.addProperty("block", block);
    serializer.endObject();
}

void TimeRenderer::serialize(Serializer &serializer) {
    serializer.startObject("TimeRenderer");
    serializer.addProperty("from", from);
    serializer.addProperty("to", to);
    serializer.addProperty("block", block);
    serializer.endObject();
}

void DateRenderer::serialize(Serializer &serializer) {
    serializer.startObject("DateRenderer");
    serializer.addProperty("from", from);
    serializer.addProperty("to", to);
    serializer.addProperty("block", block);
    serializer.endObject();
}
