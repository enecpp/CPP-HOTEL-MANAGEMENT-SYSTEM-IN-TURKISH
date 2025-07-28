#pragma once
#include "../../domain/services/OtelDomainService.h"
#include <memory>

struct CheckInRequest {
    int oda_numarasi;
    int rezervasyon_id;
    std::string musteri_ismi;
    std::string adres;
    std::string telefon;
    std::string giris_tarihi;
    std::string cikis_tarihi;
    float on_odeme;
};

struct CheckInResponse {
    bool basarili;
    std::string mesaj;
};

class CheckInUseCase {
private:
    std::shared_ptr<OtelDomainService> otel_service;

public:
    explicit CheckInUseCase(std::shared_ptr<OtelDomainService> service);
    
    CheckInResponse execute(const CheckInRequest& request);
};
