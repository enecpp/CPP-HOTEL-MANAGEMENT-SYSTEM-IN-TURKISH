#pragma once
#include "Musteri.h"
#include <memory>

enum class OdaTipi {
    KLIMALI,
    KLIMASIZ
};

enum class KonforTipi {
    LUKS,
    STANDART
};

enum class BoyutTipi {
    BUYUK,
    KUCUK
};

enum class OdaDurumu {
    MUSAIT,
    REZERVE
};

class Oda {
private:
    int oda_numarasi;
    OdaTipi klima_tipi;
    KonforTipi konfor_tipi;
    BoyutTipi boyut_tipi;
    float gunluk_kira;
    OdaDurumu durum;
    std::unique_ptr<Musteri> musteri;

public:
    
    Oda();
    Oda(int oda_numarasi, OdaTipi klima_tipi, KonforTipi konfor_tipi, 
        BoyutTipi boyut_tipi, float gunluk_kira);

    Oda(const Oda& other);
    Oda& operator=(const Oda& other);

    Oda(Oda&& other) noexcept;
    Oda& operator=(Oda&& other) noexcept;

    
    ~Oda() = default;

    
    int getOdaNumarasi() const;
    OdaTipi getKlimaTipi() const;
    KonforTipi getKonforTipi() const;
    BoyutTipi getBoyutTipi() const;
    float getGunlukKira() const;
    OdaDurumu getDurum() const;
    const Musteri* getMusteri() const;

    
    void setOdaNumarasi(int oda_numarasi);
    void setKlimaTipi(OdaTipi klima_tipi);
    void setKonforTipi(KonforTipi konfor_tipi);
    void setBoyutTipi(BoyutTipi boyut_tipi);
    void setGunlukKira(float gunluk_kira);

    bool isMusait() const;
    bool rezerveEt(const Musteri& musteri);
    bool checkOut();
    float hesaplaFatura(int gun_sayisi) const;
    bool isValid() const;

    static std::string odaTipiToString(OdaTipi tip);
    static std::string konforTipiToString(KonforTipi tip);
    static std::string boyutTipiToString(BoyutTipi tip);
};
