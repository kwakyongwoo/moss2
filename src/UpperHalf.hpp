#ifndef UPPER_HALF_HPP
#define UPPER_HALF_HPP

#include "ofMain.h"

#include <chrono>
#include <thread>

class UpperHalf {
public:
    UpperHalf(size_t x, size_t y, size_t width, size_t height) :
        x(x), y(y), width(width), height(height), trigged(false), bg(0),
        table("upper/table.png"),
        lunchbox("upper/lunchbox.png"),
        grandmother("upper/grandmother.png")
    {
        background[0].load("upper/background1.png");
        background[1].load("upper/background2.png");
        // Do Nothing
    }

    void draw() {
        if (trigged) {
            if (duration_check(start, runtime)) {
                trigged = false;
                grandmother.load("upper/grandmother.png");
            }
            else {
                /*if (duration_check(twinkle_start, twinkle)) {
                    bg = 1 - bg;
                    twinkle_start = std::chrono::steady_clock::now();
                }*/
            }
        }

        ofPushMatrix();

        if (duration_check(twinkle_start, twinkle)) {
            bg = 1 - bg;
            twinkle_start = std::chrono::steady_clock::now();
        }

        if (Constant::ROTATE) {
            ofTranslate(y, width);
            ofRotateDeg(270);
        }

        background[bg].draw(x, y, width, height);
        grandmother.draw(x, y, width, height);
        table.draw(x, y, width, height);

        if (trigged) {
            lunchbox.draw(x, y, width, height);
        }

        ofPopMatrix();
    }

    void update() {
        trigged = true;

        start = std::chrono::steady_clock::now();
        twinkle_start = start;

        grandmother.load("upper/grandmother_smile.png");
    }

private:
    size_t x, y;
    size_t width, height;

    bool trigged;

    size_t bg;
    ofImage background[2];

    ofImage table;
    ofImage lunchbox;

    ofImage grandmother;

    using time_point = std::chrono::time_point<std::chrono::steady_clock>;

    time_point start;
    time_point twinkle_start;

    static constexpr std::chrono::milliseconds runtime = 2000ms;
    static constexpr std::chrono::milliseconds twinkle = 1s; // 300ms;

    template <typename T>
    bool duration_check(time_point base, T thold) {
        using namespace std::chrono;
        return duration_cast<T>(steady_clock::now() - base) > thold;
    }
};

#endif