#include "SensorUltrasonik.h"
#include <iostream>

SensorUltrasonik::SensorUltrasonik(std::string id, double jangkauan, double ambang,
                                   double sudutPandang)
    : SensorJarak(std::move(id), jangkauan, ambang), sudutPandang_(sudutPandang) {}

SensorUltrasonik::~SensorUltrasonik() {}

double SensorUltrasonik::deteksiJarak() const { return jarakTerakhir_; }

bool SensorUltrasonik::statusRintangan() const { return jarakTerakhir_ <= ambang_; }

std::string SensorUltrasonik::getTipe() const { return "Ultrasonik"; }
