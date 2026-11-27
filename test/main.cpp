#include <iostream>
#include <vector>
#include "LenganManipulator.h"
#include "MotorPenggerakDC.h"
#include "PerencanaJalur.h"
#include "PetaGrid2D.h"
#include "RobotOtonom.h"
#include "SensorLiDAR.h"
#include "SensorUltrasonik.h"

static void judul(const std::string& t) {
    std::cout << "\n==================== " << t << " ====================\n";
}

int main() {
    // ---------------------------------------------------------------
    judul("1. Membuat komponen independen (calon agregasi & asosiasi)");
    PetaGrid2D* peta = new PetaGrid2D(10, 10, 0.5);
    PerencanaJalur* perencana = new PerencanaJalur();
    SensorUltrasonik* ultra = new SensorUltrasonik("US-01");
    SensorLiDAR* lidar = new SensorLiDAR("LDR-01");
    MotorPenggerakDC* motorKiri = new MotorPenggerakDC("MOT-L", 2.0, 0.5, 0.1);
    MotorPenggerakDC* motorKanan = new MotorPenggerakDC("MOT-R", 2.0, 0.5, 0.1);
    LenganManipulator* lengan = new LenganManipulator("ARM-01");

    motorKiri->setKecepatan(0.8);
    motorKanan->setKecepatan(0.8);
    motorKanan->setKecepatan(5.0);  // ditolak: validasi enkapsulasi
    lengan->artikulasi(1, 120.0f);  // ditolak: di luar -90..90

    peta->setTujuan(9, 9);
    peta->setRintangan(2, 2);  // rintangan yang sudah diketahui
    // Rintangan tak terlihat yang tepat berada di jalur terpendek
    peta->setRintanganTersembunyi(6, 0);
    peta->setRintanganTersembunyi(7, 0);
    peta->setRintanganTersembunyi(9, 5);

    std::cout << "\nPeta nyata (? = rintangan belum diketahui robot):\n";
    peta->tampilkan({0, 0}, {}, true);

    // ---------------------------------------------------------------
    judul("2. KOMPOSISI + navigasi + deteksi rintangan polimorfik");
    {
        std::cout << "-- Masuk scope: robot dibuat --\n";
        RobotOtonom robot("AMR-01", 0, 0, 100.0f);
        std::cout << "  " << robot.infoSasis() << "\n";

        robot.tambahSensor(ultra);
        robot.tambahSensor(lidar);
        robot.tambahAktuator(motorKiri);
        robot.tambahAktuator(motorKanan);
        robot.tambahAktuator(lengan);
        robot.setPeta(peta);
        robot.setPerencana(perencana);

        std::cout << "\n-- Uji enkapsulasi: validasi batas grid --\n";
        robot.bergerakKe(-1, 0);  // di luar grid
        robot.bergerakKe(5, 5);   // bukan sel bersebelahan

        std::cout << "\n-- Navigasi --\n";
        robot.navigasi();
        std::cout << "\nPeta akhir (# = rintangan yang dicatat robot, * = jejak):\n";
        robot.tampilkanPeta();
        std::cout << "\n";
        robot.laporanSensor();

        std::cout << "\n-- Keluar scope: robot dihancurkan --\n";
    }

    // ---------------------------------------------------------------
    judul("3. BUKTI AGREGASI: komponen tetap hidup setelah robot hancur");
    std::cout << "  Sensor " << ultra->getId() << " (" << ultra->getTipe() << ") masih hidup, jarak="
              << ultra->deteksiJarak() << "\n";
    std::cout << "  Sensor " << lidar->getId() << " (" << lidar->getTipe() << ") masih hidup, jarak="
              << lidar->deteksiJarak() << "\n";

    // ---------------------------------------------------------------
    judul("4. Konfigurasi ulang (agregasi) + LOW BATTERY & CUT-OFF");
    {
        PetaGrid2D arena2(10, 10, 0.5);
        arena2.setTujuan(9, 9);
        RobotOtonom robot2("AMR-02", 0, 0, 26.0f);
        robot2.tambahSensor(lidar);  // sensor yang sama dipakai ulang
        robot2.tambahAktuator(motorKiri);
        robot2.tambahAktuator(motorKanan);
        robot2.setPeta(&arena2);
        robot2.setPerencana(perencana);

        robot2.lepasSensor("LDR-01");  // dicopot dinamis
        robot2.tambahSensor(ultra);
        robot2.tambahSensor(lidar);
        robot2.navigasi();
        std::cout << "  Posisi akhir (" << robot2.getPosisiX() << "," << robot2.getPosisiY()
                  << "), baterai " << robot2.getLevelBaterai() << "%\n";
    }

    // ---------------------------------------------------------------
    judul("5. Pembersihan memori (delete manual)");
    delete lengan;
    delete motorKanan;
    delete motorKiri;
    delete lidar;
    delete ultra;
    delete perencana;
    delete peta;

    std::cout << "\nSelesai.\n";
    return 0;
}
