#include "MotorPenggerakDC.h"
#include <iostream>

MotorPenggerakDC::MotorPenggerakDC(std::string id, double kp, double ki, double kd,
                                   double kecepatanMaks)
    : Aktuator(std::move(id)),
      kp_(kp), ki_(ki), kd_(kd),
      kecepatan_(0.0), kecepatanAktual_(0.0), kecepatanMaks_(kecepatanMaks),
      galatAkumulasi_(0.0), galatSebelumnya_(0.0) {}

MotorPenggerakDC::~MotorPenggerakDC() {}

double MotorPenggerakDC::hitungPID(double galat, double dt) {
    galatAkumulasi_ += galat * dt;
    double turunan = (galat - galatSebelumnya_) / dt;
    galatSebelumnya_ = galat;
    return kp_ * galat + ki_ * galatAkumulasi_ + kd_ * turunan;
}

bool MotorPenggerakDC::setKecepatan(double v) {
    if (v < 0.0 || v > kecepatanMaks_) {
        std::cout << "  [Motor " << idAktuator_ << "] DITOLAK: kecepatan " << v
                  << " di luar 0.." << kecepatanMaks_ << " m/s\n";
        return false;
    }
    kecepatan_ = v;
    return true;
}

void MotorPenggerakDC::gerak() {
    aktif_ = true;
    for (int i = 0; i < 20; ++i) kecepatanAktual_ += hitungPID(kecepatan_ - kecepatanAktual_) * 0.1;
    std::cout << "  [Motor " << idAktuator_ << "] berputar, target " << kecepatan_
              << " m/s, aktual " << kecepatanAktual_ << " m/s (PID)\n";
}

void MotorPenggerakDC::berhenti() {
    Aktuator::berhenti();
    kecepatanAktual_ = 0.0;
    galatAkumulasi_ = galatSebelumnya_ = 0.0;
    std::cout << "  [Motor " << idAktuator_ << "] berhenti\n";
}
