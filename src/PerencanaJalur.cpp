#include "PerencanaJalur.h"
#include <algorithm>
#include <iostream>
#include <queue>

PerencanaJalur::PerencanaJalur() { std::cout << "  [+] PerencanaJalur (BFS) dibuat\n"; }
PerencanaJalur::~PerencanaJalur() { std::cout << "  [-] PerencanaJalur dihancurkan\n"; }

std::vector<Koordinat> PerencanaJalur::rencanakan(const PetaGrid2D& peta, Koordinat awal,
                                                  Koordinat tujuan) const {
    std::vector<Koordinat> jalur;
    if (!peta.dalamBatas(awal.x, awal.y) || !peta.cekAreaBebas(tujuan.x, tujuan.y) ||
        awal == tujuan)
        return jalur;

    const int W = peta.getLebar(), H = peta.getTinggi();
    std::vector<std::vector<Koordinat>> induk(H, std::vector<Koordinat>(W, Koordinat{-1, -1}));
    std::vector<std::vector<bool>> dikunjungi(H, std::vector<bool>(W, false));
    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {-1, 0, 1, 0};

    std::queue<Koordinat> antrean;
    antrean.push(awal);
    dikunjungi[awal.y][awal.x] = true;
    while (!antrean.empty()) {
        Koordinat c = antrean.front();
        antrean.pop();
        if (c == tujuan) break;
        for (int i = 0; i < 4; ++i) {
            int nx = c.x + dx[i], ny = c.y + dy[i];
            if (!peta.cekAreaBebas(nx, ny) || dikunjungi[ny][nx]) continue;
            dikunjungi[ny][nx] = true;
            induk[ny][nx] = c;
            antrean.push({nx, ny});
        }
    }
    if (!dikunjungi[tujuan.y][tujuan.x]) return jalur;

    for (Koordinat c = tujuan; c != awal; c = induk[c.y][c.x]) jalur.push_back(c);
    std::reverse(jalur.begin(), jalur.end());
    return jalur;
}
