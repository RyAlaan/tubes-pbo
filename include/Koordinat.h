#pragma once

// Struct data sederhana (value type) untuk posisi pada grid 2D.
struct Koordinat {
    int x;
    int y;
    bool operator==(const Koordinat& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Koordinat& o) const { return !(*this == o); }
};
