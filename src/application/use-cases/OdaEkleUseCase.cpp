#include "OdaEkleUseCase.h"

OdaEkleUseCase::OdaEkleUseCase(std::shared_ptr<OtelDomainService> service)
    : otel_service(service) {}

OdaEkleResponse OdaEkleUseCase::execute(const OdaEkleRequest& request) {
    OdaEkleResponse response;
    
    // Validation
    if (request.oda_numarasi <= 0) {
        response.basarili = false;
        response.mesaj = "Gecersiz oda numarasi";
        return response;
    }
    
    if (request.gunluk_kira <= 0) {
        response.basarili = false;
        response.mesaj = "Gecersiz gunluk kira miktari";
        return response;
    }
    
    // Oda numarası kontrolü
    if (otel_service->odaNumarasiKullanildaMi(request.oda_numarasi)) {
        response.basarili = false;
        response.mesaj = "Oda numarasi zaten mevcut";
        return response;
    }
    
    // Enum dönüşümleri
    OdaTipi klima_tipi = charToOdaTipi(request.klima_tipi);
    KonforTipi konfor_tipi = charToKonforTipi(request.konfor_tipi);
    BoyutTipi boyut_tipi = charToBoyutTipi(request.boyut_tipi);
    
    // Oda ekleme
    bool basarili = otel_service->odaEkle(request.oda_numarasi, klima_tipi, 
                                          konfor_tipi, boyut_tipi, request.gunluk_kira);
    
    response.basarili = basarili;
    response.mesaj = basarili ? "Oda basariyla eklendi" : "Oda eklenemedi";
    
    return response;
}

OdaTipi OdaEkleUseCase::charToOdaTipi(char c) {
    return (c == 'K' || c == 'k') ? OdaTipi::KLIMALI : OdaTipi::KLIMASIZ;
}

KonforTipi OdaEkleUseCase::charToKonforTipi(char c) {
    return (c == 'L' || c == 'l') ? KonforTipi::LUKS : KonforTipi::STANDART;
}

BoyutTipi OdaEkleUseCase::charToBoyutTipi(char c) {
    return (c == 'B' || c == 'b') ? BoyutTipi::BUYUK : BoyutTipi::KUCUK;
}
