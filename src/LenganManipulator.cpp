#include "LenganManipulator.h"
#include <iostream>
#include <stdexcept>

LenganManipulator::LenganManipulator(std::string id, int jumlahSendi)
    : Aktuator(std::move(id)) {
    if (jumlahSendi <= 0) throw std::invalid_argument("Jumlah sendi harus positif");
    sudutSendi_.assign(jumlahSendi, 0.0f);
}

LenganManipulator::~LenganManipulator() {}

bool LenganManipulator::artikulasi(int indeks, float sudut) {
    if (indeks < 0 || indeks >= static_cast<int>(sudutSendi_.size()) || sudut < -90.0f ||
        sudut > 90.0f) {
        std::cout << "  [Lengan " << idAktuator_ << "] DITOLAK: sendi " << indeks
                  << " sudut " << sudut << " tidak valid\n";
        return false;
    }
    sudutSendi_[indeks] = sudut;
    return true;
}

void LenganManipulator::reset() {
    for (float& s : sudutSendi_) s = 0.0f;
}

void LenganManipulator::gerak() {
    aktif_ = true;
    for (size_t i = 0; i < sudutSendi_.size(); ++i) artikulasi(static_cast<int>(i), 30.0f * (i % 2 ? -1 : 1));
    std::cout << "  [Lengan " << idAktuator_ << "] pose siaga (" << sudutSendi_.size()
              << " sendi diartikulasikan)\n";
}

void LenganManipulator::berhenti() {
    Aktuator::berhenti();
    reset();
    std::cout << "  [Lengan " << idAktuator_ << "] kembali ke posisi nol & terkunci\n";
}
