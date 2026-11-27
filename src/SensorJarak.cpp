#include "SensorJarak.h"
#include <iostream>
#include <stdexcept>

SensorJarak::SensorJarak(std::string id, double jangkauan, double ambang)
    : idSensor_(std::move(id)), jangkauan_(jangkauan), ambang_(ambang), jarakTerakhir_(jangkauan) {
    if (jangkauan <= 0 || ambang <= 0)
        throw std::invalid_argument("Jangkauan dan ambang sensor harus positif");
    std::cout << "  [+] Sensor " << idSensor_ << " dibuat\n";
}

SensorJarak::~SensorJarak() { std::cout << "  [-] Sensor " << idSensor_ << " dihancurkan\n"; }

double SensorJarak::pindaiGaris(const PetaGrid2D& peta, int x, int y, int dx, int dy) const {
    const int maks = static_cast<int>(jangkauan_);
    for (int k = 1; k <= maks; ++k)
        if (peta.adaRintanganNyata(x + dx * k, y + dy * k)) return k;
    return jangkauan_;
}

void SensorJarak::perbarui(const PetaGrid2D& peta, int x, int y, int dx, int dy) {
    jarakTerakhir_ = pindaiGaris(peta, x, y, dx, dy);
}
