#include "PetaGrid2D.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

PetaGrid2D::PetaGrid2D(int lebar, int tinggi, double resolusi)
    : lebar_(lebar), tinggi_(tinggi), resolusi_(resolusi), tujuan_{-1, -1} {
    if (lebar <= 0 || tinggi <= 0 || resolusi <= 0)
        throw std::invalid_argument("Dimensi dan resolusi peta harus positif");
    gridDikenal_.assign(tinggi_, std::vector<int>(lebar_, 0));
    gridNyata_ = gridDikenal_;
    std::cout << "  [+] PetaGrid2D " << lebar_ << "x" << tinggi_ << " dibuat\n";
}

PetaGrid2D::~PetaGrid2D() { std::cout << "  [-] PetaGrid2D dihancurkan\n"; }

bool PetaGrid2D::dalamBatas(int x, int y) const {
    return x >= 0 && x < lebar_ && y >= 0 && y < tinggi_;
}

bool PetaGrid2D::setRintangan(int x, int y) {
    if (!dalamBatas(x, y)) return false;
    gridDikenal_[y][x] = 1;
    gridNyata_[y][x] = 1;
    return true;
}

bool PetaGrid2D::setRintanganTersembunyi(int x, int y) {
    if (!dalamBatas(x, y)) return false;
    gridNyata_[y][x] = 1;
    return true;
}

bool PetaGrid2D::cekAreaBebas(int x, int y) const {
    return dalamBatas(x, y) && gridDikenal_[y][x] == 0;
}

bool PetaGrid2D::adaRintanganNyata(int x, int y) const {
    return !dalamBatas(x, y) || gridNyata_[y][x] == 1;
}

bool PetaGrid2D::setTujuan(int x, int y) {
    if (!dalamBatas(x, y)) return false;
    tujuan_ = {x, y};
    return true;
}

void PetaGrid2D::tampilkan(const Koordinat& robot, const std::vector<Koordinat>& jejak,
                           bool tampilTersembunyi) const {
    for (int y = 0; y < tinggi_; ++y) {
        std::cout << "    ";
        for (int x = 0; x < lebar_; ++x) {
            Koordinat k{x, y};
            char c = '.';
            if (gridDikenal_[y][x]) c = '#';
            else if (tampilTersembunyi && gridNyata_[y][x]) c = '?';
            else if (std::find(jejak.begin(), jejak.end(), k) != jejak.end()) c = '*';
            if (k == tujuan_) c = 'T';
            if (k == robot) c = 'R';
            std::cout << c << ' ';
        }
        std::cout << '\n';
    }
}
