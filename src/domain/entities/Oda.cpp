#include "Oda.h"

Oda::Oda() : oda_numarasi(0), klima_tipi(OdaTipi::KLIMASIZ), konfor_tipi(KonforTipi::STANDART),
             boyut_tipi(BoyutTipi::KUCUK), gunluk_kira(0.0f), durum(OdaDurumu::MUSAIT),
             musteri(nullptr) {}

Oda::Oda(int oda_numarasi, OdaTipi klima_tipi, KonforTipi konfor_tipi, 
         BoyutTipi boyut_tipi, float gunluk_kira)
    : oda_numarasi(oda_numarasi), klima_tipi(klima_tipi), konfor_tipi(konfor_tipi),
      boyut_tipi(boyut_tipi), gunluk_kira(gunluk_kira), durum(OdaDurumu::MUSAIT),
      musteri(nullptr) {}

Oda::Oda(const Oda& other)
    : oda_numarasi(other.oda_numarasi), klima_tipi(other.klima_tipi),
      konfor_tipi(other.konfor_tipi), boyut_tipi(other.boyut_tipi),
      gunluk_kira(other.gunluk_kira), durum(other.durum) {
    if (other.musteri) {
        musteri = std::make_unique<Musteri>(*other.musteri);
    }
}

Oda& Oda::operator=(const Oda& other) {
    if (this != &other) {
        oda_numarasi = other.oda_numarasi;
        klima_tipi = other.klima_tipi;
        konfor_tipi = other.konfor_tipi;
        boyut_tipi = other.boyut_tipi;
        gunluk_kira = other.gunluk_kira;
        durum = other.durum;
        
        if (other.musteri) {
            musteri = std::make_unique<Musteri>(*other.musteri);
        } else {
            musteri.reset();
        }
    }
    return *this;
}

Oda::Oda(Oda&& other) noexcept
    : oda_numarasi(other.oda_numarasi), klima_tipi(other.klima_tipi),
      konfor_tipi(other.konfor_tipi), boyut_tipi(other.boyut_tipi),
      gunluk_kira(other.gunluk_kira), durum(other.durum),
      musteri(std::move(other.musteri)) {}

Oda& Oda::operator=(Oda&& other) noexcept {
    if (this != &other) {
        oda_numarasi = other.oda_numarasi;
        klima_tipi = other.klima_tipi;
        konfor_tipi = other.konfor_tipi;
        boyut_tipi = other.boyut_tipi;
        gunluk_kira = other.gunluk_kira;
        durum = other.durum;
        musteri = std::move(other.musteri);
    }
    return *this;
}

int Oda::getOdaNumarasi() const { return oda_numarasi; }
OdaTipi Oda::getKlimaTipi() const { return klima_tipi; }
KonforTipi Oda::getKonforTipi() const { return konfor_tipi; }
BoyutTipi Oda::getBoyutTipi() const { return boyut_tipi; }
float Oda::getGunlukKira() const { return gunluk_kira; }
OdaDurumu Oda::getDurum() const { return durum; }
const Musteri* Oda::getMusteri() const { return musteri.get(); }

void Oda::setOdaNumarasi(int oda_numarasi) { this->oda_numarasi = oda_numarasi; }
void Oda::setKlimaTipi(OdaTipi klima_tipi) { this->klima_tipi = klima_tipi; }
void Oda::setKonforTipi(KonforTipi konfor_tipi) { this->konfor_tipi = konfor_tipi; }
void Oda::setBoyutTipi(BoyutTipi boyut_tipi) { this->boyut_tipi = boyut_tipi; }
void Oda::setGunlukKira(float gunluk_kira) { this->gunluk_kira = gunluk_kira; }

bool Oda::isMusait() const {
    return durum == OdaDurumu::MUSAIT;
}

bool Oda::rezerveEt(const Musteri& musteri_bilgisi) {
    if (!isMusait() || !musteri_bilgisi.isValid()) {
        return false;
    }
    
    musteri = std::make_unique<Musteri>(musteri_bilgisi);
    durum = OdaDurumu::REZERVE;
    return true;
}

bool Oda::checkOut() {
    if (durum == OdaDurumu::MUSAIT) {
        return false;
    }
    
    musteri.reset();
    durum = OdaDurumu::MUSAIT;
    return true;
}

float Oda::hesaplaFatura(int gun_sayisi) const {
    return gun_sayisi * gunluk_kira;
}

bool Oda::isValid() const {
    return oda_numarasi > 0 && gunluk_kira > 0.0f;
}

std::string Oda::odaTipiToString(OdaTipi tip) {
    switch (tip) {
        case OdaTipi::KLIMALI: return "Klimali (K)";
        case OdaTipi::KLIMASIZ: return "Klimasiz (N)";
        default: return "Bilinmeyen";
    }
}

std::string Oda::konforTipiToString(KonforTipi tip) {
    switch (tip) {
        case KonforTipi::LUKS: return "Luks (L)";
        case KonforTipi::STANDART: return "Standart (S)";
        default: return "Bilinmeyen";
    }
}

std::string Oda::boyutTipiToString(BoyutTipi tip) {
    switch (tip) {
        case BoyutTipi::BUYUK: return "Buyuk (B)";
        case BoyutTipi::KUCUK: return "Kucuk (K)";
        default: return "Bilinmeyen";
    }
}
