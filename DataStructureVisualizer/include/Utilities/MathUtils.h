#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include "Utilities/Position.h"

#include <random>
#include <vector>

namespace MathUtils {
    // Random
    // Returns a random integer in the inclusive range [min, max].
    inline int getRandomInRange(int min, int max) {
        if (min > max) std::swap(min, max);

        static thread_local std::random_device rd;
        static thread_local std::mt19937 gen(rd());

        std::uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    }

    // Geometry
    // Returns the orientation of the ordered triplet (p, q, r).
    inline int orientation(Position p, Position q, Position r) {
        long long val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0) return 0; 
        return (val > 0) ? 1 : 2;
    }

    // Returns true when q lies on segment pr.
    inline bool onSegment(Position p, Position q, Position r) {
        return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
    }

    // Returns true if the two line segments intersect.
    inline bool doIntersect(Position p1, Position q1, Position p2, Position q2) {
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        if (o1 != o2 && o3 != o4) return true;

        if (o1 == 0 && onSegment(p1, p2, q1)) return true;
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;

        return false;
    }
} // namespace MathUtils

#endif