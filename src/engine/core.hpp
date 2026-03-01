#pragma once

namespace core {

// TODO cut down by 3/5
const int WINDOW_WIDTH = 660;
const int WINDOW_HEIGHT = 480;

struct Rect {
    float x;
    float y;
    float width;
    float height;
};

struct Point {
    float x;
    float y;
};

} // namespace core