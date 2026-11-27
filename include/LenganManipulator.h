#pragma once
#include <vector>
#include "Aktuator.h"

class LenganManipulator : public Aktuator {
private:
    std::vector<float> sudutSendi_;  // derajat, tiap sendi dibatasi -90..90

public:
    LenganManipulator(std::string id, int jumlahSendi = 3);
    ~LenganManipulator() override;

    bool artikulasi(int indeks, float sudut);  // validasi indeks & rentang sudut
    void reset();

    void gerak() override;
    void berhenti() override;
};
