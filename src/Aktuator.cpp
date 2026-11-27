#include "Aktuator.h"
#include <iostream>

Aktuator::Aktuator(std::string id) : idAktuator_(std::move(id)), aktif_(false) {
    std::cout << "  [+] Aktuator " << idAktuator_ << " dibuat\n";
}

Aktuator::~Aktuator() { std::cout << "  [-] Aktuator " << idAktuator_ << " dihancurkan\n"; }

void Aktuator::berhenti() { aktif_ = false; }
