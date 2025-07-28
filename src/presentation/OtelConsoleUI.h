#pragma once
#include "../application/use-cases/OdaEkleUseCase.h"
#include "../application/use-cases/CheckInUseCase.h"
#include "../application/use-cases/CheckOutUseCase.h"
#include "../domain/services/OtelDomainService.h"
#include <memory>
#include <iostream>

class OtelConsoleUI {
private:
    std::shared_ptr<OtelDomainService> otel_service;
    std::shared_ptr<OdaEkleUseCase> oda_ekle_use_case;
    std::shared_ptr<CheckInUseCase> check_in_use_case;
    std::shared_ptr<CheckOutUseCase> check_out_use_case;

public:
    explicit OtelConsoleUI(std::shared_ptr<OtelDomainService> service);
    
    void run();
    
private:
    void anaMenuGoster();
    void odalariYonet();
    void odaEkle();
    void odaAra();
    void checkIn();
    void musaitOdalarGoster();
    void musteriAra();
    void checkOut();
    void misafirOzetRaporu();
    
    void odaBilgileriniGoster(const Oda& oda);
    void musteriBilgileriniGoster(const Oda& oda);
    void bekle();
};
