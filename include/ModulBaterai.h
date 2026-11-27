#pragma once

class ModulBaterai {
private:
    float levelBaterai_;  // persen 0..100

public:
    static constexpr float AMBANG_LOW = 20.0f;
    static constexpr float AMBANG_CUTOFF = 10.0f;

    explicit ModulBaterai(float levelAwal = 100.0f);
    ~ModulBaterai();

    void kurangiDaya(float jumlah);
    float getLevel() const { return levelBaterai_; }
    bool isLowBattery() const { return levelBaterai_ <= AMBANG_LOW; }
    bool isCutOff() const { return levelBaterai_ <= AMBANG_CUTOFF; }
};
