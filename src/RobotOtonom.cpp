#include "RobotOtonom.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>

RobotOtonom::RobotOtonom(std::string id, int x, int y, float levelBaterai, float panjang,
                         float lebar, float jarakRoda)
    : idRobot_(std::move(id)),
      posisi_{x, y},
      arahX_(1), arahY_(0),
      sasis_(nullptr), baterai_(nullptr),
      peta_(nullptr), perencana_(nullptr),
      peringatanLow_(false) {
    std::cout << "[+] RobotOtonom " << idRobot_ << " dibuat\n";
    // KOMPOSISI: bagian dibuat bersamaan dengan robot
    sasis_ = new SasisRobot(panjang, lebar, jarakRoda);
    baterai_ = new ModulBaterai(levelBaterai);
}

RobotOtonom::~RobotOtonom() {
    std::cout << "[-] RobotOtonom " << idRobot_ << " dihancurkan\n";
    // KOMPOSISI: bagian ikut dimusnahkan
    delete baterai_;
    delete sasis_;
    // AGREGASI: sensor_ dan aktuator_ TIDAK di-delete (dimiliki pihak luar)
    std::cout << "    (" << sensor_.size() << " sensor & " << aktuator_.size()
              << " aktuator dilepas, tetap hidup)\n";
}

void RobotOtonom::tambahSensor(SensorJarak* s) {
    if (s) sensor_.push_back(s);
}

bool RobotOtonom::lepasSensor(const std::string& id) {
    auto it = std::find_if(sensor_.begin(), sensor_.end(),
                           [&](SensorJarak* s) { return s->getId() == id; });
    if (it == sensor_.end()) return false;
    sensor_.erase(it);  // hanya dilepas dari koleksi, objek tidak dihapus
    return true;
}

void RobotOtonom::tambahAktuator(Aktuator* a) {
    if (a) aktuator_.push_back(a);
}

bool RobotOtonom::lepasAktuator(const std::string& id) {
    auto it = std::find_if(aktuator_.begin(), aktuator_.end(),
                           [&](Aktuator* a) { return a->getId() == id; });
    if (it == aktuator_.end()) return false;
    aktuator_.erase(it);
    return true;
}

bool RobotOtonom::bergerakKe(int x, int y) {
    if (!peta_) {
        std::cout << "  [DITOLAK] Robot belum terhubung ke peta\n";
        return false;
    }
    if (!peta_->dalamBatas(x, y)) {
        std::cout << "  [DITOLAK] (" << x << "," << y << ") di luar batas grid\n";
        return false;
    }
    if (std::abs(x - posisi_.x) + std::abs(y - posisi_.y) != 1) {
        std::cout << "  [DITOLAK] (" << x << "," << y << ") bukan sel bersebelahan\n";
        return false;
    }
    if (baterai_->isCutOff()) {
        std::cout << "  [DITOLAK] Baterai cut-off\n";
        return false;
    }
    if (peta_->adaRintanganNyata(x, y)) {
        std::cout << "  [TABRAKAN] Rintangan fisik di (" << x << "," << y << ")\n";
        return false;
    }
    baterai_->kurangiDaya(BIAYA_LANGKAH);
    posisi_ = {x, y};
    jejak_.push_back(posisi_);
    return true;
}

void RobotOtonom::putarKe(const Koordinat& next) {
    int dx = next.x - posisi_.x, dy = next.y - posisi_.y;
    if (dx != arahX_ || dy != arahY_) {
        arahX_ = dx;
        arahY_ = dy;
        baterai_->kurangiDaya(BIAYA_PUTAR);
    }
}

bool RobotOtonom::pindaiLingkungan() {
    bool baru = false;
    for (SensorJarak* s : sensor_) {  // POLIMORFISME: satu loop, banyak tipe sensor
        s->perbarui(*peta_, posisi_.x, posisi_.y, arahX_, arahY_);
        if (!s->statusRintangan()) continue;
        int n = static_cast<int>(std::lround(s->deteksiJarak()));
        int cx = posisi_.x + arahX_ * n, cy = posisi_.y + arahY_ * n;
        if (!peta_->dalamBatas(cx, cy)) continue;  // tembok batas arena sudah diketahui
        std::cout << "  [Sensor " << s->getId() << " / " << s->getTipe()
                  << "] rintangan pada jarak " << n << " sel";
        if (peta_->dalamBatas(cx, cy) && peta_->cekAreaBebas(cx, cy)) {
            peta_->setRintangan(cx, cy);  // mencatat rintangan ke peta
            std::cout << " -> dicatat di (" << cx << "," << cy << ")";
            baru = true;
        }
        std::cout << '\n';
    }
    return baru;
}

void RobotOtonom::aktifkanAktuator() {
    for (Aktuator* a : aktuator_) a->gerak();  // POLIMORFISME
}

void RobotOtonom::hentikanAktuator() {
    for (Aktuator* a : aktuator_) a->berhenti();
}

void RobotOtonom::cutOff() {
    std::cout << "[CUT-OFF] Baterai " << std::fixed << std::setprecision(1)
              << baterai_->getLevel() << "% <= " << ModulBaterai::AMBANG_CUTOFF
              << "%. Misi dihentikan di (" << posisi_.x << "," << posisi_.y << ")\n";
    hentikanAktuator();
}

bool RobotOtonom::navigasi() {
    if (!peta_ || !perencana_) {
        std::cout << "[GAGAL] Peta/perencana belum terpasang\n";
        return false;
    }
    const Koordinat tujuan = peta_->getTujuan();
    std::cout << "[" << idRobot_ << "] Mulai navigasi (" << posisi_.x << "," << posisi_.y
              << ") -> (" << tujuan.x << "," << tujuan.y << ")\n";

    jejak_.clear();
    jejak_.push_back(posisi_);
    peringatanLow_ = false;
    aktifkanAktuator();

    jalur_ = perencana_->rencanakan(*peta_, posisi_, tujuan);
    if (jalur_.empty() && posisi_ != tujuan) {
        std::cout << "[GAGAL] Tidak ada jalur\n";
        hentikanAktuator();
        return false;
    }

    int langkah = 0;
    while (posisi_ != tujuan) {
        if (baterai_->isCutOff()) {
            cutOff();
            return false;
        }
        Koordinat next = jalur_.front();
        putarKe(next);

        if (pindaiLingkungan()) {  // ada rintangan baru -> rencanakan ulang
            jalur_ = perencana_->rencanakan(*peta_, posisi_, tujuan);
            std::cout << "  [Replan] " << jalur_.size() << " langkah tersisa\n";
            if (jalur_.empty()) {
                std::cout << "[GAGAL] Tidak ada jalur alternatif\n";
                hentikanAktuator();
                return false;
            }
            continue;
        }

        if (!bergerakKe(next.x, next.y)) {  // rintangan tak terdeteksi sensor
            if (peta_->adaRintanganNyata(next.x, next.y) && peta_->cekAreaBebas(next.x, next.y)) {
                peta_->setRintangan(next.x, next.y);
                jalur_ = perencana_->rencanakan(*peta_, posisi_, tujuan);
                if (!jalur_.empty()) continue;
            }
            hentikanAktuator();
            return false;
        }
        jalur_.erase(jalur_.begin());
        std::cout << "  Langkah " << ++langkah << ": (" << posisi_.x << "," << posisi_.y
                  << ") baterai " << std::fixed << std::setprecision(1) << baterai_->getLevel()
                  << "%\n";

        if (baterai_->isLowBattery() && !peringatanLow_) {
            peringatanLow_ = true;
            std::cout << "  [PERINGATAN] LOW BATTERY (" << baterai_->getLevel() << "%)\n";
        }
    }
    std::cout << "[BERHASIL] Tujuan tercapai di (" << posisi_.x << "," << posisi_.y << ")\n";
    hentikanAktuator();
    return true;
}

void RobotOtonom::laporanSensor() const {
    std::cout << "  Laporan sensor " << idRobot_ << " (dipanggil polimorfik):\n";
    for (const SensorJarak* s : sensor_) {
        std::cout << "    - " << std::left << std::setw(8) << s->getId() << std::setw(11)
                  << s->getTipe() << " jarak=" << s->deteksiJarak() << " sel  rintangan="
                  << (s->statusRintangan() ? "YA" : "tidak") << '\n';
    }
}

void RobotOtonom::tampilkanPeta(bool tampilTersembunyi) const {
    if (peta_) peta_->tampilkan(posisi_, jejak_, tampilTersembunyi);
}
