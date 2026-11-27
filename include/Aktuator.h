#pragma once
#include <string>

// ABSTRACT BASE CLASS aktuator.
class Aktuator {
protected:
    std::string idAktuator_;
    bool aktif_;

public:
    explicit Aktuator(std::string id);
    virtual ~Aktuator();

    virtual void gerak() = 0;     // pure virtual
    virtual void berhenti();      // default; boleh di-override

    const std::string& getId() const { return idAktuator_; }
    bool isAktif() const { return aktif_; }
};
