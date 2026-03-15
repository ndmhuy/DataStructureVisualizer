#ifndef TIMELINE_H
#define TIMELINE_H

#include <vector>
#include "Frame.h"

class Timeline {
    public:
    std::vector<Frame> frames;
    int currentFrameIndex;

    Timeline();
    void addFrame(const Frame& frame);
    void nextFrame();
    void clear();
    int getFrameCount() const;
    const Frame* getCurrentFrame() const;
};

#endif // TIMELINE_H