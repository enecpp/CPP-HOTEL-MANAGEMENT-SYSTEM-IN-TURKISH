#include "src/presentation/OtelConsoleUI.h"
#include "src/infrastructure/repositories/InMemoryOdaRepository.h"
#include "src/domain/services/OtelDomainService.h"
#include <memory>
#include <iostream>

int main() {
    try {
        
        auto oda_repository = std::make_shared<InMemoryOdaRepository>();
        
        auto otel_service = std::make_shared<OtelDomainService>(oda_repository);
        
        auto console_ui = std::make_unique<OtelConsoleUI>(otel_service);
        

        console_ui->run();
        
    } catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Bilinmeyen bir hata olustu!" << std::endl;
        return 1;
    }
    
    return 0;
}
