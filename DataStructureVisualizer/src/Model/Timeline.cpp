#include "Model/Timeline.h"

Timeline::Timeline() : currentFrameIndex(0) {}

void Timeline::addFrame(const Frame& frame) {
    frames.push_back(frame);
}

void Timeline::clear() {
    frames.clear();
    currentFrameIndex = 0;
}

void Timeline::nextFrame() {
    if (!frames.empty() && currentFrameIndex + 1 < frames.size()) {
        ++currentFrameIndex;
    }
}

void Timeline::prevFrame() {
    if (currentFrameIndex > 0) {
        --currentFrameIndex;
    }
}

void Timeline::goToFrame(int index) {
    if (index >= 0 && index < frames.size()) {
        currentFrameIndex = index;
    }
}

void Timeline::goToFirstFrame() {
    if (!frames.empty()) {
        currentFrameIndex = 0;
    }
}

void Timeline::goToLastFrame() {
    if (!frames.empty()) {
        currentFrameIndex = frames.size() - 1;
    }
}

bool Timeline::isAtEnd() const {
    return frames.empty() || currentFrameIndex >= frames.size() - 1;
}

int Timeline::getFrameCount() const {
    return frames.size();
}

int Timeline::getCurrentFrameIndex() const {
    return currentFrameIndex;
}

const Frame* Timeline::getCurrentFrame() const {
    if (frames.empty() || currentFrameIndex < 0 || currentFrameIndex >= frames.size()) {
        return nullptr;
    }
    return &frames[currentFrameIndex];
}