#include "src/presentation/web/WebServer.h"
#include "src/infrastructure/repositories/InMemoryOdaRepository.h"
#include "src/domain/services/OtelDomainService.h"
#include <memory>
#include <iostream>
#include <signal.h>

std::unique_ptr<WebServer> web_server;

void signalHandler(int signum) {
    std::cout << "\n\n🛑 Server kapatılıyor..." << std::endl;
    if (web_server) {
        web_server->stop();
    }
    exit(signum);
}

int main() {
    try {
        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);
        
        auto oda_repository = std::make_shared<InMemoryOdaRepository>();
        auto otel_service = std::make_shared<OtelDomainService>(oda_repository);
        
        web_server = std::make_unique<WebServer>(otel_service, 8080);
        
        std::cout << "🌟 MODERN C++ OTEL YÖNETİM SİSTEMİ" << std::endl;
        std::cout << "🚀 Web GUI Sürümü" << std::endl;
        std::cout << "============================================" << std::endl;
        
        web_server->start();
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Hata: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Bilinmeyen bir hata oluştu!" << std::endl;
        return 1;
    }
    
    return 0;
}
