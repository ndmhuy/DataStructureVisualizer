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
    // Creates a playback controller in the paused state.
    Playback();

    // Advances playback state based on the elapsed time.
    void update(float deltaTime);

    // Starts playback.
    void play();
    // Pauses playback.
    void pause();
    // Moves forward one frame.
    void stepForward();
    // Moves backward one frame.
    void stepBackward();
    // Jumps to the specified frame index.
    void goToStep(size_t index);
    // Jumps to the first frame.
    void goToFirstStep();
    // Jumps to the final frame.
    void goToFinalStep();
    // Sets the playback speed multiplier.
    void setSpeed(float multiplier);
    // Sets the duration of a single timeline step.
    void setBaseStepDuration(float seconds);

    // Returns the managed timeline.
    const Timeline& getTimeline() const;
    // Replaces the managed timeline.
    void setTimeline(const Timeline& timeline);

    // Returns whether playback is currently running.
    bool isPlayingState() const;
};

#endif // PLAYBACK_H