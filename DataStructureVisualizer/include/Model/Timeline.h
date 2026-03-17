#ifndef TIMELINE_H
#define TIMELINE_H

#include <vector>
#include "Frame.h"

class Timeline {
    private:
    std::vector<Frame> frames;
    int currentFrameIndex;

    public:
    Timeline();
    void addFrame(const Frame& frame);
    void clear();

    void nextFrame();
    void prevFrame();
    void goToFrame(int index);
    void goToFirstFrame();
    void goToLastFrame();

    bool isAtEnd() const;
    int getFrameCount() const;
    int getCurrentFrameIndex() const;
    const Frame* getCurrentFrame() const;
};

#endif // TIMELINE_H