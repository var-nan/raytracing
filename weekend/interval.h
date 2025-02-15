#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
    public:
        double min, max;
        interval() : min{+infinity}, max{-infinity} {} // why these values

        interval(double min_, double max_) : min{min_}, max{max_} {}

        double size() const { return max - min;}

        bool contains(double x) const { return min <= x && x <= max;}

        bool surrounds(double x) const {return min < x && x < max; }

        double clamp(double x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval (-infinity, infinity);

#endif