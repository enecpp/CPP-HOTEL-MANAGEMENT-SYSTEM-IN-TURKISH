#pragma once
#include "../../domain/services/OtelDomainService.h"
#include <memory>

struct CheckOutRequest {
    int oda_numarasi;
    int gun_sayisi;
};

struct CheckOutResponse {
    bool basarili;
    std::string mesaj;
    float toplam_fatura;
    float on_odeme;
    float kalan_borc;
    std::string musteri_bilgileri;
};

class CheckOutUseCase {
private:
    std::shared_ptr<OtelDomainService> otel_service;

public:
    explicit CheckOutUseCase(std::shared_ptr<OtelDomainService> service);
    
    CheckOutResponse execute(const CheckOutRequest& request);
};
