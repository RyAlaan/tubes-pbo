#pragma once
#include "SensorJarak.h"

class SensorUltrasonik : public SensorJarak {
private:
    double sudutPandang_;  // derajat

public:
    SensorUltrasonik(std::string id, double jangkauan = 4.0, double ambang = 2.0,
                     double sudutPandang = 30.0);
    ~SensorUltrasonik() override;

    double deteksiJarak() const override;
    bool statusRintangan() const override;
    std::string getTipe() const override;
    double getSudutPandang() const { return sudutPandang_; }
};
