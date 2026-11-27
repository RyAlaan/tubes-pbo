#include "SensorLiDAR.h"

SensorLiDAR::SensorLiDAR(std::string id, double jangkauan, double ambang, double resolusiSudut)
    : SensorJarak(std::move(id), jangkauan, ambang),
      resolusiSudut_(resolusiSudut),
      jumlahPindai_(0) {}

SensorLiDAR::~SensorLiDAR() {}

void SensorLiDAR::perbarui(const PetaGrid2D& peta, int x, int y, int dx, int dy) {
    SensorJarak::perbarui(peta, x, y, dx, dy);
    ++jumlahPindai_;
}

double SensorLiDAR::deteksiJarak() const { return jarakTerakhir_; }

// LiDAR mengabaikan pembacaan maksimum (tidak ada pantulan) sebagai rintangan.
bool SensorLiDAR::statusRintangan() const {
    return jarakTerakhir_ < jangkauan_ && jarakTerakhir_ <= ambang_;
}

std::string SensorLiDAR::getTipe() const { return "LiDAR"; }
