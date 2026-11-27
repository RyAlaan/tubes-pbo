#pragma once
#include <vector>
#include "Koordinat.h"
#include "PetaGrid2D.h"

// Perencana jalur BFS (jalur terpendek 4-arah) pada PetaGrid2D.
// Berelasi ASOSIASI dengan PetaGrid2D (peta hanya dipakai lewat parameter).
class PerencanaJalur {
public:
    PerencanaJalur();
    ~PerencanaJalur();

    // Mengembalikan langkah-langkah dari sel setelah 'awal' sampai 'tujuan'.
    // Kosong jika tidak ada jalur (atau awal == tujuan).
    std::vector<Koordinat> rencanakan(const PetaGrid2D& peta, Koordinat awal,
                                      Koordinat tujuan) const;
};
