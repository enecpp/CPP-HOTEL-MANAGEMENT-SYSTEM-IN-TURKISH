#pragma once
#include "../../domain/services/OtelDomainService.h"
#include <memory>

struct OdaEkleRequest {
    int oda_numarasi;
    char klima_tipi; // 'K' veya 'N'
    char konfor_tipi; // 'L' veya 'S'
    char boyut_tipi; // 'B' veya 'K'
    float gunluk_kira;
};

struct OdaEkleResponse {
    bool basarili;
    std::string mesaj;
};

class OdaEkleUseCase {
private:
    std::shared_ptr<OtelDomainService> otel_service;
    
    OdaTipi charToOdaTipi(char c);
    KonforTipi charToKonforTipi(char c);
    BoyutTipi charToBoyutTipi(char c);

public:
    explicit OdaEkleUseCase(std::shared_ptr<OtelDomainService> service);
    
    OdaEkleResponse execute(const OdaEkleRequest& request);
};
