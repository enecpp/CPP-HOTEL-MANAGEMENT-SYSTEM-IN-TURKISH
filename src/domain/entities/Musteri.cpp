#include "Musteri.h"

Musteri::Musteri() : on_odeme(0.0f), rezervasyon_id(0) {}

Musteri::Musteri(const std::string& isim, const std::string& adres, const std::string& telefon,
                 const std::string& giris_tarihi, const std::string& cikis_tarihi,
                 float on_odeme, int rezervasyon_id)
    : isim(isim), adres(adres), telefon(telefon), giris_tarihi(giris_tarihi),
      cikis_tarihi(cikis_tarihi), on_odeme(on_odeme), rezervasyon_id(rezervasyon_id) {}

const std::string& Musteri::getIsim() const { return isim; }
const std::string& Musteri::getAdres() const { return adres; }
const std::string& Musteri::getTelefon() const { return telefon; }
const std::string& Musteri::getGirisTarihi() const { return giris_tarihi; }
const std::string& Musteri::getCikisTarihi() const { return cikis_tarihi; }
float Musteri::getOnOdeme() const { return on_odeme; }
int Musteri::getRezervasyonId() const { return rezervasyon_id; }

void Musteri::setIsim(const std::string& isim) { this->isim = isim; }
void Musteri::setAdres(const std::string& adres) { this->adres = adres; }
void Musteri::setTelefon(const std::string& telefon) { this->telefon = telefon; }
void Musteri::setGirisTarihi(const std::string& giris_tarihi) { this->giris_tarihi = giris_tarihi; }
void Musteri::setCikisTarihi(const std::string& cikis_tarihi) { this->cikis_tarihi = cikis_tarihi; }
void Musteri::setOnOdeme(float on_odeme) { this->on_odeme = on_odeme; }
void Musteri::setRezervasyonId(int rezervasyon_id) { this->rezervasyon_id = rezervasyon_id; }

bool Musteri::isValid() const {
    return !isim.empty() && !telefon.empty() && rezervasyon_id > 0;
}
