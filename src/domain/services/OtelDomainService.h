#pragma once
#include "../entities/Oda.h"
#include "../entities/Musteri.h"
#include "../repositories/IOdaRepository.h"
#include <memory>
#include <vector>

class OtelDomainService {
private:
    std::shared_ptr<IOdaRepository> oda_repository;

public:
    explicit OtelDomainService(std::shared_ptr<IOdaRepository> repository);
    
    // Oda yönetimi
    bool odaEkle(int oda_numarasi, OdaTipi klima_tipi, KonforTipi konfor_tipi, 
                 BoyutTipi boyut_tipi, float gunluk_kira);
    
    // Rezervasyon işlemleri
    bool checkIn(int oda_numarasi, const Musteri& musteri);
    bool checkOut(int oda_numarasi, int gun_sayisi, float& toplam_fatura, float& kalan_borc);
    
    // Sorgulama işlemleri
    std::vector<Oda> musaitOdalariGetir();
    std::vector<Oda> musteriAra(const std::string& musteri_ismi);
    std::vector<Oda> misafirOzetRaporu();
    std::optional<Oda> odaAra(int oda_numarasi);
    
    // Validation
    bool odaNumarasiKullanildaMi(int oda_numarasi);
};
