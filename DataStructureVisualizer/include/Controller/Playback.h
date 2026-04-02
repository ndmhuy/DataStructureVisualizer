#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "Model/Timeline.h"

class Playback {
    private:
    Timeline currentTimeline;
    bool isPlaying;
    float speedMultiplier;
    float timeAccumulator;
    float baseStepDuration;

    public:
    Playback();

    void update(float deltaTime);

    void play();
    void pause();
    void stepForward();
    void stepBackward();
    void goToStep(size_t index);
    void goToFirstStep();
    void goToFinalStep();
    void setSpeed(float multiplier);
    void setBaseStepDuration(float seconds);

    const Timeline& getTimeline() const;
    void setTimeline(const Timeline& timeline);

    bool isPlayingState() const;
};

#endif // PLAYBACK_H