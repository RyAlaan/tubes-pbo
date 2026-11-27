#include "ModulBaterai.h"
#include <algorithm>
#include <iostream>

ModulBaterai::ModulBaterai(float levelAwal) : levelBaterai_(std::clamp(levelAwal, 0.0f, 100.0f)) {
    std::cout << "  [+] ModulBaterai dibuat (" << levelBaterai_ << "%)\n";
}

ModulBaterai::~ModulBaterai() { std::cout << "  [-] ModulBaterai dihancurkan\n"; }

void ModulBaterai::kurangiDaya(float jumlah) {
    if (jumlah <= 0.0f) return;  // invarian: baterai hanya boleh berkurang
    levelBaterai_ = std::max(0.0f, levelBaterai_ - jumlah);
}
