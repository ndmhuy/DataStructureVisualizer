#include "Model/Timeline.h"

Timeline::Timeline() : currentFrameIndex(0) {}

void Timeline::addFrame(const Frame& frame) {
    frames.push_back(frame);
}

void Timeline::nextFrame() {
    if (currentFrameIndex < frames.size() - 1) {
        ++currentFrameIndex;
    }
}

void Timeline::clear() {
    frames.clear();
    currentFrameIndex = 0;
}

int Timeline::getFrameCount() const {
    return frames.size();
}

const Frame* Timeline::getCurrentFrame() const {
    if (frames.empty()) {
        return nullptr;
    }
    return &frames[currentFrameIndex];
}