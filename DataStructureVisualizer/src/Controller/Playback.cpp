#include "Controller/Playback.h"
#include <algorithm>

Playback::Playback() : isPlaying(false), speedMultiplier(1.0f), timeAccumulator(0.0f), baseStepDuration(1.0f) {}

void Playback::update(float deltaTime) {
    if (isPlaying && currentTimeline.getFrameCount() > 0) {
        timeAccumulator += deltaTime * speedMultiplier;

        while (timeAccumulator >= baseStepDuration) {
            stepForward();

            timeAccumulator -= baseStepDuration;

            if (currentTimeline.isAtEnd()) {
                pause();
                break;
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
    currentTimeline.prevFrame();
}

void Playback::goToStep(size_t index) {
    currentTimeline.goToFrame(index);
}

void Playback::goToFirstStep() {
    currentTimeline.goToFirstFrame();
}

void Playback::goToFinalStep() {
    currentTimeline.goToLastFrame();
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