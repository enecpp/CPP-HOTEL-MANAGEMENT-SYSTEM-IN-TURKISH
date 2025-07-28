#pragma once
#include <string>

class Musteri {
private:
    std::string isim;
    std::string adres;
    std::string telefon;
    std::string giris_tarihi;
    std::string cikis_tarihi;
    float on_odeme;
    int rezervasyon_id;

public:
    Musteri();
    Musteri(const std::string& isim, const std::string& adres, const std::string& telefon,
            const std::string& giris_tarihi, const std::string& cikis_tarihi,
            float on_odeme, int rezervasyon_id);

    const std::string& getIsim() const;
    const std::string& getAdres() const;
    const std::string& getTelefon() const;
    const std::string& getGirisTarihi() const;
    const std::string& getCikisTarihi() const;
    float getOnOdeme() const;
    int getRezervasyonId() const;

    void setIsim(const std::string& isim);
    void setAdres(const std::string& adres);
    void setTelefon(const std::string& telefon);
    void setGirisTarihi(const std::string& giris_tarihi);
    void setCikisTarihi(const std::string& cikis_tarihi);
    void setOnOdeme(float on_odeme);
    void setRezervasyonId(int rezervasyon_id);

    bool isValid() const;
};
