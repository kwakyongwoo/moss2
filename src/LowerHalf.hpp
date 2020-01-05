#ifndef LOWER_HALF_HPP
#define LOWER_HALF_HPP

#include "ofMain.h"

#include <chrono>
#include <functional>
#include <thread>

class LowerHalf {
public:
    LowerHalf(size_t x, size_t y, size_t width, size_t height) :
        x(x), y(y), width(width), height(height),
        background("lower/background.png"), hand("lower/hand.png"), base("lower/base.png"),
        n_tagged(0), reserved(false), go_up(false), delta(height / n_delta), idx(0)
    {
        size_t i = 0;
        for (std::string filename : { "first", "second", "cover" }) {
            lunchbox[i++].load("lower/" + filename + ".png");
        }
    }

    void set_upper_trigger(std::function<void()> const& callback) {
        upper_trigger = callback;
    }

    void draw() {
        using namespace std::literals;

        if (go_up) {
            std::this_thread::sleep_for(time_delta);

            if (idx >= n_frame) {
                release_go_up();
                n_tagged = 0;

                upper_trigger();
            }
            else if (!reserved) {
                idx += 1;
            }
        }

        double px = x + width * idx * ratio / 2;
        double py = y - idx * delta;
        double pw = width * (1.0 - idx * ratio);
        double ph = height * (1.0 - idx * ratio);

        ofPushMatrix();

        if (Constant::ROTATE) {
            ofTranslate(0, width);
            ofRotateDeg(270);
        }

        background.draw(x, y, width, height);
        hand.draw(px, py, pw, ph);
        base.draw(px, py, pw, ph);

        size_t limit = n_tagged > (n_floor + 1) ? (n_floor + 1) : n_tagged;
        for (size_t i = 0; i < limit; ++i) {
            lunchbox[i].draw(px, py, pw, ph);
        }

        if (reserved && check_duration(thold)) {
            n_tagged += 1;
            lunchbox[n_floor].draw(px, py, pw, ph);

            reserved = false;
        }

        ofPopMatrix();
    }

    void update() {
        n_tagged += 1;
        
        reserved = n_tagged == n_floor;
        if (reserved) {
            start = std::chrono::steady_clock::now();
        }

        if (n_tagged >= n_floor && !go_up) {
            set_go_up();
        }
    }

private:
    size_t x, y;
    size_t height, width;

    ofImage background;
    ofImage hand;
    ofImage base;

    size_t n_tagged;
    static constexpr size_t n_floor = 2;

    ofImage lunchbox[n_floor + 1];

    bool reserved;
    std::chrono::time_point<std::chrono::steady_clock> start;

    static constexpr std::chrono::milliseconds thold = 500ms;

    bool go_up;
    size_t delta;
    size_t idx;

    static constexpr size_t n_delta = 10;
    static constexpr size_t n_frame = 5;
    static constexpr double ratio = 0.06;
    static constexpr std::chrono::milliseconds time_delta = 200ms;

    std::function<void()> upper_trigger;

    template <typename T>
    bool check_duration(T threshold) {
        using namespace std::chrono;
        return duration_cast<T>(steady_clock::now() - start) > threshold;
    }

    void set_go_up() {
        go_up = true;
        idx = 0;
        hand.load("lower/go_up.png");
    }

    void release_go_up() {
        go_up = false;
        idx = 0;
        hand.load("lower/hand.png");
    }
};

#endif