#include "Controller/Playback.h"
#include <algorithm>

Playback::Playback() : isPlaying(false), speedMultiplier(1.0f), timeAccumulator(0.0f), baseStepDuration(1.0f) {}

void Playback::update(float deltaTime) {
    if (isPlaying && currentTimeline.getFrameCount() > 0) {
        timeAccumulator += deltaTime * speedMultiplier;

        if (timeAccumulator >= baseStepDuration) {
            stepForward();

            timeAccumulator -= baseStepDuration;

            if (currentTimeline.currentFrameIndex >= currentTimeline.getFrameCount() - 1) {
                pause();
            }
        }
    }
}

void Playback::play() {
    isPlaying = true;
}

void Playback::pause() {
    isPlaying = false;
}

void Playback::stepForward() {
    currentTimeline.nextFrame();
}

void Playback::stepBackward() {
    if (currentTimeline.currentFrameIndex > 0) {
        --currentTimeline.currentFrameIndex;
    }
}

void Playback::goToFinalStep() {
    currentTimeline.currentFrameIndex = std::max(0, currentTimeline.getFrameCount() - 1);
}

void Playback::setSpeed(float multiplier) {
    speedMultiplier = multiplier;
}

void Playback::setBaseStepDuration(float seconds) {
    baseStepDuration = seconds;
}

const Timeline& Playback::getTimeline() const {
    return currentTimeline;
}

void Playback::setTimeline(const Timeline& timeline) {
    currentTimeline = timeline;
    timeAccumulator = 0.0f;
    isPlaying = false;
}

bool Playback::isPlayingState() const {
    return isPlaying;
}