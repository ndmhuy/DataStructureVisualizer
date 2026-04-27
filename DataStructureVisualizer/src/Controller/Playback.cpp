#include "Controller/Playback.h"
#include <algorithm>

namespace {
constexpr float kMinSpeedMultiplier = 0.01f;
constexpr float kMinStepDuration = 0.01f;
constexpr float kMaxDeltaTime = 0.25f;
constexpr int kMaxStepsPerUpdate = 120;
}

Playback::Playback() : isPlaying(false), speedMultiplier(1.0f), timeAccumulator(0.0f), baseStepDuration(1.0f) {}

void Playback::update(float deltaTime) {
    if (!isPlaying || currentTimeline.getFrameCount() == 0) {
        return;
    }

    const float safeDelta = std::clamp(deltaTime, 0.0f, kMaxDeltaTime);
    timeAccumulator += safeDelta * speedMultiplier;

    int stepsThisUpdate = 0;
    while (timeAccumulator >= baseStepDuration && stepsThisUpdate < kMaxStepsPerUpdate) {
        currentTimeline.nextFrame();
        ++stepsThisUpdate;
        timeAccumulator -= baseStepDuration;

        if (currentTimeline.isAtEnd()) {
            pause();
            break;
        }
    }
}

void Playback::play() {
    if (currentTimeline.getFrameCount() == 0) {
        isPlaying = false;
        return;
    }

    // If user presses play at the end, restart from first frame.
    if (currentTimeline.isAtEnd()) {
        currentTimeline.goToFirstFrame();
        timeAccumulator = 0.0f;
    }

    isPlaying = true;
}

void Playback::pause() {
    isPlaying = false;
}

void Playback::stepForward() {
    pause();
    timeAccumulator = 0.0f;
    currentTimeline.nextFrame();
}

void Playback::stepBackward() {
    pause();
    timeAccumulator = 0.0f;
    currentTimeline.prevFrame();
}

void Playback::goToStep(size_t index) {
    pause();
    timeAccumulator = 0.0f;
    currentTimeline.goToFrame(index);
}

void Playback::goToFirstStep() {
    pause();
    timeAccumulator = 0.0f;
    currentTimeline.goToFirstFrame();
}

void Playback::goToFinalStep() {
    pause();
    timeAccumulator = 0.0f;
    currentTimeline.goToLastFrame();
}

void Playback::setSpeed(float multiplier) {
    speedMultiplier = std::max(multiplier, kMinSpeedMultiplier);
}

void Playback::setBaseStepDuration(float seconds) {
    baseStepDuration = std::max(seconds, kMinStepDuration);
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