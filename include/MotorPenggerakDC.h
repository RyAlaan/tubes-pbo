#pragma once
#include "Aktuator.h"

class MotorPenggerakDC : public Aktuator {
private:
    double kp_, ki_, kd_;
    double kecepatan_;        // target (m/s)
    double kecepatanAktual_;  // m/s
    double kecepatanMaks_;
    double galatAkumulasi_;
    double galatSebelumnya_;

public:
    MotorPenggerakDC(std::string id, double kp, double ki, double kd,
                     double kecepatanMaks = 1.0);
    ~MotorPenggerakDC() override;

    double hitungPID(double galat, double dt = 0.1);
    bool setKecepatan(double v);  // validasi 0..kecepatanMaks
    double getKecepatan() const { return kecepatan_; }

    void gerak() override;
    void berhenti() override;
};
