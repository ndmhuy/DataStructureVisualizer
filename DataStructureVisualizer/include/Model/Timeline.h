#ifndef TIMELINE_H
#define TIMELINE_H

#include <vector>
#include "Frame.h"

class Timeline {
    private:
    std::vector<Frame> frames;
    size_t currentFrameIndex;

    public:
    // Creates an empty timeline.
    Timeline();
    // Appends a frame to the timeline.
    void addFrame(const Frame& frame);
    // Removes all frames and resets the playback cursor.
    void clear();

    // Advances to the next frame if possible.
    void nextFrame();
    // Moves to the previous frame if possible.
    void prevFrame();
    // Jumps to a specific frame index.
    void goToFrame(size_t index);
    // Jumps to the first frame.
    void goToFirstFrame();
    // Jumps to the last frame.
    void goToLastFrame();

    // Returns whether the playback cursor is at the end.
    bool isAtEnd() const;
    // Returns the number of frames stored in the timeline.
    size_t getFrameCount() const;
    // Returns the current playback index.
    size_t getCurrentFrameIndex() const;
    // Returns the current frame, or nullptr if empty.
    const Frame* getCurrentFrame() const;
};

#endif // TIMELINE_H