#pragma once
#include <string>
#include "PetaGrid2D.h"

// ABSTRACT BASE CLASS sensor jarak. Satuan jarak: jumlah sel grid.
class SensorJarak {
protected:
    std::string idSensor_;
    double jangkauan_;      // jangkauan maksimum (sel)
    double ambang_;         // batas jarak yang dianggap "rintangan"
    double jarakTerakhir_;  // hasil pindai terakhir

    // Logika umum pemindaian satu garis lurus (dipakai bersama oleh subclass).
    double pindaiGaris(const PetaGrid2D& peta, int x, int y, int dx, int dy) const;

public:
    SensorJarak(std::string id, double jangkauan, double ambang);
    virtual ~SensorJarak();

    // Memperbarui pembacaan sensor dari posisi (x,y) menghadap (dx,dy).
    virtual void perbarui(const PetaGrid2D& peta, int x, int y, int dx, int dy);

    // Kontrak murni (pure virtual)
    virtual double deteksiJarak() const = 0;
    virtual bool statusRintangan() const = 0;
    virtual std::string getTipe() const = 0;

    const std::string& getId() const { return idSensor_; }
    double getJangkauan() const { return jangkauan_; }
};
