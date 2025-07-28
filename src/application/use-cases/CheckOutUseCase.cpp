#include "CheckOutUseCase.h"

CheckOutUseCase::CheckOutUseCase(std::shared_ptr<OtelDomainService> service)
    : otel_service(service) {}

CheckOutResponse CheckOutUseCase::execute(const CheckOutRequest& request) {
    CheckOutResponse response;
    response.basarili = false;
    response.toplam_fatura = 0.0f;
    response.on_odeme = 0.0f;
    response.kalan_borc = 0.0f;
    
    // Validation
    if (request.oda_numarasi <= 0) {
        response.mesaj = "Gecersiz oda numarasi";
        return response;
    }
    
    if (request.gun_sayisi <= 0) {
        response.mesaj = "Gecersiz gun sayisi";
        return response;
    }
    
    // Önce oda bilgilerini al
    auto oda_opt = otel_service->odaAra(request.oda_numarasi);
    if (!oda_opt.has_value()) {
        response.mesaj = "Oda bulunamadi";
        return response;
    }
    
    const Oda& oda = oda_opt.value();
    const Musteri* musteri = oda.getMusteri();
    
    if (!musteri) {
        response.mesaj = "Oda zaten check-out yapilmis";
        return response;
    }
    
    // Müşteri bilgilerini kaydet
    response.musteri_bilgileri = "Musteri: " + musteri->getIsim() + 
                                ", Telefon: " + musteri->getTelefon() +
                                ", Adres: " + musteri->getAdres();
    response.on_odeme = musteri->getOnOdeme();
    
    // Check-out işlemi
    float toplam_fatura, kalan_borc;
    bool basarili = otel_service->checkOut(request.oda_numarasi, request.gun_sayisi, 
                                           toplam_fatura, kalan_borc);
    
    if (basarili) {
        response.basarili = true;
        response.mesaj = "Check-out islemi basariyla tamamlandi";
        response.toplam_fatura = toplam_fatura;
        response.kalan_borc = kalan_borc;
    } else {
        response.mesaj = "Check-out islemi basarisiz";
    }
    
    return response;
}
