#include "CheckInUseCase.h"

CheckInUseCase::CheckInUseCase(std::shared_ptr<OtelDomainService> service)
    : otel_service(service) {}

CheckInResponse CheckInUseCase::execute(const CheckInRequest& request) {
    CheckInResponse response;
    
    // Validation
    if (request.oda_numarasi <= 0) {
        response.basarili = false;
        response.mesaj = "Gecersiz oda numarasi";
        return response;
    }
    
    if (request.musteri_ismi.empty()) {
        response.basarili = false;
        response.mesaj = "Musteri ismi bos olamaz";
        return response;
    }
    
    if (request.rezervasyon_id <= 0) {
        response.basarili = false;
        response.mesaj = "Gecersiz rezervasyon ID";
        return response;
    }
    
    // Müşteri nesnesi oluştur
    Musteri musteri(request.musteri_ismi, request.adres, request.telefon,
                    request.giris_tarihi, request.cikis_tarihi, 
                    request.on_odeme, request.rezervasyon_id);
    
    // Check-in işlemi
    bool basarili = otel_service->checkIn(request.oda_numarasi, musteri);
    
    if (basarili) {
        response.basarili = true;
        response.mesaj = "Check-in islemi basariyla tamamlandi";
    } else {
        response.basarili = false;
        response.mesaj = "Check-in islemi basarisiz (Oda bulunamadi veya zaten rezerve)";
    }
    
    return response;
}
