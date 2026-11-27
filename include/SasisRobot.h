#pragma once
#include <string>

class SasisRobot {
private:
    float panjang_;
    float lebar_;
    float jarakRoda_;

public:
    SasisRobot(float panjang, float lebar, float jarakRoda);
    ~SasisRobot();

    bool setDimensi(float panjang, float lebar);  // harus > 0
    std::string ambilKonfigurasi() const;
    float getPanjang() const { return panjang_; }
    float getLebar() const { return lebar_; }
    float getJarakRoda() const { return jarakRoda_; }
};
