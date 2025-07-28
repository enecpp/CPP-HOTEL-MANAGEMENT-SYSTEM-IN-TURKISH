#include "OtelDomainService.h"
#include <algorithm>

OtelDomainService::OtelDomainService(std::shared_ptr<IOdaRepository> repository)
    : oda_repository(repository) {}

bool OtelDomainService::odaEkle(int oda_numarasi, OdaTipi klima_tipi, KonforTipi konfor_tipi, 
                                BoyutTipi boyut_tipi, float gunluk_kira) {
    if (oda_repository->odaVarMi(oda_numarasi)) {
        return false; // Oda zaten mevcut
    }
    
    Oda yeni_oda(oda_numarasi, klima_tipi, konfor_tipi, boyut_tipi, gunluk_kira);
    
    if (!yeni_oda.isValid()) {
        return false; // Geçersiz oda bilgileri
    }
    
    return oda_repository->ekle(yeni_oda);
}

bool OtelDomainService::checkIn(int oda_numarasi, const Musteri& musteri) {
    auto oda_opt = oda_repository->getByOdaNumarasi(oda_numarasi);
    
    if (!oda_opt.has_value()) {
        return false; // Oda bulunamadı
    }
    
    Oda oda = oda_opt.value();
    
    if (!oda.isMusait()) {
        return false; // Oda zaten rezerve
    }
    
    if (!musteri.isValid()) {
        return false; // Geçersiz müşteri bilgileri
    }
    
    if (oda.rezerveEt(musteri)) {
        return oda_repository->guncelle(oda);
    }
    
    return false;
}

bool OtelDomainService::checkOut(int oda_numarasi, int gun_sayisi, float& toplam_fatura, float& kalan_borc) {
    auto oda_opt = oda_repository->getByOdaNumarasi(oda_numarasi);
    
    if (!oda_opt.has_value()) {
        return false; // Oda bulunamadı
    }
    
    Oda oda = oda_opt.value();
    
    if (oda.isMusait()) {
        return false; // Oda zaten müsait
    }
    
    const Musteri* musteri = oda.getMusteri();
    if (!musteri) {
        return false; // Müşteri bilgisi bulunamadı
    }
    
    toplam_fatura = oda.hesaplaFatura(gun_sayisi);
    kalan_borc = toplam_fatura - musteri->getOnOdeme();
    
    if (oda.checkOut()) {
        return oda_repository->guncelle(oda);
    }
    
    return false;
}

std::vector<Oda> OtelDomainService::musaitOdalariGetir() {
    return oda_repository->musaitOdalariGetir();
}

std::vector<Oda> OtelDomainService::musteriAra(const std::string& musteri_ismi) {
    std::vector<Oda> rezerve_odalar = oda_repository->rezerveOdalariGetir();
    std::vector<Oda> bulunan_odalar;
    
    for (const auto& oda : rezerve_odalar) {
        const Musteri* musteri = oda.getMusteri();
        if (musteri && musteri->getIsim() == musteri_ismi) {
            bulunan_odalar.push_back(oda);
        }
    }
    
    return bulunan_odalar;
}

std::vector<Oda> OtelDomainService::misafirOzetRaporu() {
    return oda_repository->rezerveOdalariGetir();
}

std::optional<Oda> OtelDomainService::odaAra(int oda_numarasi) {
    return oda_repository->getByOdaNumarasi(oda_numarasi);
}

bool OtelDomainService::odaNumarasiKullanildaMi(int oda_numarasi) {
    return oda_repository->odaVarMi(oda_numarasi);
}
