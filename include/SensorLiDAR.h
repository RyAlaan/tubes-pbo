#pragma once
#include "SensorJarak.h"

class SensorLiDAR : public SensorJarak {
private:
    double resolusiSudut_;  // derajat
    int jumlahPindai_;

public:
    SensorLiDAR(std::string id, double jangkauan = 8.0, double ambang = 3.0,
                double resolusiSudut = 0.25);
    ~SensorLiDAR() override;

    void perbarui(const PetaGrid2D& peta, int x, int y, int dx, int dy) override;
    double deteksiJarak() const override;
    bool statusRintangan() const override;
    std::string getTipe() const override;
    int getJumlahPindai() const { return jumlahPindai_; }
};
