#ifndef TIMELINE_H
#define TIMELINE_H

#include <vector>
#include "Frame.h"

class Timeline {
    private:
    std::vector<Frame> frames;
    size_t currentFrameIndex;

    public:
    Timeline();
    void addFrame(const Frame& frame);
    void clear();

    void nextFrame();
    void prevFrame();
    void goToFrame(size_t index);
    void goToFirstFrame();
    void goToLastFrame();

    bool isAtEnd() const;
    size_t getFrameCount() const;
    size_t getCurrentFrameIndex() const;
    const Frame* getCurrentFrame() const;
};

#endif // TIMELINE_H