#include "SasisRobot.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

SasisRobot::SasisRobot(float panjang, float lebar, float jarakRoda)
    : panjang_(panjang), lebar_(lebar), jarakRoda_(jarakRoda) {
    if (panjang <= 0 || lebar <= 0 || jarakRoda <= 0)
        throw std::invalid_argument("Dimensi sasis harus positif");
    std::cout << "  [+] SasisRobot dibuat (bagian dari robot)\n";
}

SasisRobot::~SasisRobot() { std::cout << "  [-] SasisRobot dihancurkan\n"; }

bool SasisRobot::setDimensi(float panjang, float lebar) {
    if (panjang <= 0 || lebar <= 0) return false;
    panjang_ = panjang;
    lebar_ = lebar;
    return true;
}

std::string SasisRobot::ambilKonfigurasi() const {
    std::ostringstream os;
    os << "Sasis " << panjang_ << "m x " << lebar_ << "m, jarak roda " << jarakRoda_ << "m";
    return os.str();
}
