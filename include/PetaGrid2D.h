#pragma once
#include <vector>
#include "Koordinat.h"

// Model arena 2D. Menyimpan dua lapisan:
//  - gridDikenal_ : rintangan yang SUDAH diketahui robot (dipakai perencana jalur)
//  - gridNyata_   : rintangan fisik sebenarnya (hanya bisa "dilihat" lewat sensor)
class PetaGrid2D {
private:
    int lebar_;
    int tinggi_;
    double resolusi_;  // meter per sel
    std::vector<std::vector<int>> gridDikenal_;
    std::vector<std::vector<int>> gridNyata_;
    Koordinat tujuan_;

public:
    PetaGrid2D(int lebar, int tinggi, double resolusi = 0.5);
    ~PetaGrid2D();

    bool dalamBatas(int x, int y) const;
    bool setRintangan(int x, int y);             // rintangan diketahui (juga nyata)
    bool setRintanganTersembunyi(int x, int y);  // rintangan nyata, belum diketahui robot
    bool cekAreaBebas(int x, int y) const;       // bebas menurut pengetahuan robot
    bool adaRintanganNyata(int x, int y) const;  // kondisi fisik (luar grid = dinding)

    bool setTujuan(int x, int y);
    Koordinat getTujuan() const { return tujuan_; }
    int getLebar() const { return lebar_; }
    int getTinggi() const { return tinggi_; }
    double getResolusi() const { return resolusi_; }

    void tampilkan(const Koordinat& robot, const std::vector<Koordinat>& jejak,
                   bool tampilTersembunyi = false) const;
};
