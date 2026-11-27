# Proyek PBO Kelompok 3 - Sistem Navigasi & Kendali Robot Beroda Otonom (AMR)

Anggota: Alvin Syahrinaldi (H1H025040), Muhammad Rafid Baihaqi (H1H025042), Derry Alantis (H1H025053)

## Build & Run
Butuh `g++` dengan dukungan C++17.

    make run      # compile + jalankan demo
    make clean

## Struktur
    include/  header (.h)        src/   implementasi (.cpp)
    test/     main.cpp (driver)  doc/   laporan PDF & UML

## Peta Kelas -> CPMK
| Kelas | Peran |
|---|---|
| `SensorJarak` (abstrak) | `deteksiJarak()`, `statusRintangan()`, `getTipe()` pure virtual |
| `SensorUltrasonik`, `SensorLiDAR` | turunan konkret (override) |
| `Aktuator` (abstrak) | `gerak()` pure virtual, `berhenti()` virtual |
| `MotorPenggerakDC`, `LenganManipulator` | turunan konkret (override) |
| `SasisRobot`, `ModulBaterai` | **Komposisi** dalam `RobotOtonom` (new di ctor, delete di dtor) |
| `vector<SensorJarak*>`, `vector<Aktuator*>` | **Agregasi** di `RobotOtonom` (tidak di-delete di dtor) |
| `PetaGrid2D`, `PerencanaJalur` | **Asosiasi** dari `RobotOtonom` (pointer, tidak dimiliki) |
| `RobotOtonom` | kelas utama / koordinator |

## Skenario Demo (`test/main.cpp`)
1. Pembuatan komponen independen + validasi enkapsulasi (kecepatan motor, sudut lengan).
2. Komposisi: log ctor/dtor `SasisRobot` & `ModulBaterai` ikut robot; navigasi, deteksi rintangan polimorfik, replan.
3. Agregasi: sensor tetap hidup setelah robot keluar scope.
4. Konfigurasi ulang sensor/aktuator, LOW BATTERY (<=20%) dan CUT-OFF (<=10%).
5. Pembersihan memori manual (tanpa memory leak, sudah dicek dengan `-fsanitize=address`).
