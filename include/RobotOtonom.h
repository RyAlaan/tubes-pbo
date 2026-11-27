#pragma once
#include <string>
#include <vector>
#include "Koordinat.h"
#include "SasisRobot.h"
#include "ModulBaterai.h"
#include "SensorJarak.h"
#include "Aktuator.h"
#include "PetaGrid2D.h"
#include "PerencanaJalur.h"

// Kelas utama.
//  KOMPOSISI : SasisRobot*, ModulBaterai*  (dibuat di ctor, dihapus di dtor)
//  AGREGASI  : vector<SensorJarak*>, vector<Aktuator*>  (TIDAK dihapus di dtor)
//  ASOSIASI  : PetaGrid2D*, PerencanaJalur*  (hanya dipakai, tidak dimiliki)
class RobotOtonom {
private:
    static constexpr float BIAYA_LANGKAH = 2.0f;
    static constexpr float BIAYA_PUTAR = 0.5f;

    std::string idRobot_;
    Koordinat posisi_;
    int arahX_, arahY_;

    SasisRobot* sasis_;
    ModulBaterai* baterai_;
    std::vector<SensorJarak*> sensor_;
    std::vector<Aktuator*> aktuator_;
    PetaGrid2D* peta_;
    PerencanaJalur* perencana_;

    std::vector<Koordinat> jalur_;
    std::vector<Koordinat> jejak_;
    bool peringatanLow_;

    void putarKe(const Koordinat& next);
    bool pindaiLingkungan();  // true jika ada rintangan BARU yang tercatat
    void cutOff();

public:
    RobotOtonom(std::string id, int x, int y, float levelBaterai, float panjang = 0.6f,
                float lebar = 0.4f, float jarakRoda = 0.3f);
    ~RobotOtonom();
    RobotOtonom(const RobotOtonom&) = delete;
    RobotOtonom& operator=(const RobotOtonom&) = delete;

    // Agregasi (konfigurasi dinamis)
    void tambahSensor(SensorJarak* s);
    bool lepasSensor(const std::string& id);
    void tambahAktuator(Aktuator* a);
    bool lepasAktuator(const std::string& id);

    // Asosiasi
    void setPeta(PetaGrid2D* peta) { peta_ = peta; }
    void setPerencana(PerencanaJalur* p) { perencana_ = p; }

    // Perilaku
    bool bergerakKe(int x, int y);  // satu sel; ditolak jika di luar grid / menabrak
    bool navigasi();                // menuju PetaGrid2D::getTujuan()
    void aktifkanAktuator();
    void hentikanAktuator();
    void laporanSensor() const;     // pemanggilan polimorfik pada semua sensor
    void tampilkanPeta(bool tampilTersembunyi = false) const;

    Koordinat getPosisi() const { return posisi_; }
    int getPosisiX() const { return posisi_.x; }
    int getPosisiY() const { return posisi_.y; }
    float getLevelBaterai() const { return baterai_->getLevel(); }
    const std::string& getId() const { return idRobot_; }
    std::string infoSasis() const { return sasis_->ambilKonfigurasi(); }
};
