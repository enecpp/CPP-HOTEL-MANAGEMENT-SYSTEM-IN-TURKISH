#pragma once
#include "../../domain/services/OtelDomainService.h"
#include "../../application/use-cases/OdaEkleUseCase.h"
#include "../../application/use-cases/CheckInUseCase.h"
#include "../../application/use-cases/CheckOutUseCase.h"
#include <memory>
#include <string>
#include <map>

class WebServer {
private:
    std::shared_ptr<OtelDomainService> otel_service;
    std::shared_ptr<OdaEkleUseCase> oda_ekle_use_case;
    std::shared_ptr<CheckInUseCase> check_in_use_case;
    std::shared_ptr<CheckOutUseCase> check_out_use_case;
    
    int port;
    bool running;

public:
    explicit WebServer(std::shared_ptr<OtelDomainService> service, int port = 8080);
    ~WebServer();
    
    void start();
    void stop();
    
private:
    // HTTP Response helpers
    std::string generateHTML();
    std::string generateOdaListHTML();
    std::string generateCheckInFormHTML();
    std::string generateCheckOutFormHTML();
    std::string generateOdaEkleFormHTML();
    std::string generateRaporHTML();
    
    // API Endpoints
    std::string handleOdaEkle(const std::map<std::string, std::string>& params);
    std::string handleCheckIn(const std::map<std::string, std::string>& params);
    std::string handleCheckOut(const std::map<std::string, std::string>& params);
    std::string handleMusaitOdalar();
    std::string handleRapor();
    
    // Utility functions
    std::map<std::string, std::string> parseFormData(const std::string& data);
    std::string urlDecode(const std::string& str);
    void handleRequest(int client_socket);
    std::string getResponseHeaders(int content_length, const std::string& content_type = "text/html; charset=utf-8");
};
