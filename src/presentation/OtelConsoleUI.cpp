#include "OtelConsoleUI.h"
#include <limits>

OtelConsoleUI::OtelConsoleUI(std::shared_ptr<OtelDomainService> service)
    : otel_service(service) {
    oda_ekle_use_case = std::make_shared<OdaEkleUseCase>(service);
    check_in_use_case = std::make_shared<CheckInUseCase>(service);
    check_out_use_case = std::make_shared<CheckOutUseCase>(service);
}

void OtelConsoleUI::run() {
    int secim;
    
    do {
        anaMenuGoster();
        std::cin >> secim;
        
        // Input buffer temizle
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (secim) {
            case 1:
                odalariYonet();
                break;
            case 2:
                checkIn();
                break;
            case 3:
                musaitOdalarGoster();
                break;
            case 4:
                musteriAra();
                break;
            case 5:
                checkOut();
                break;
            case 6:
                misafirOzetRaporu();
                break;
            case 7:
                std::cout << "\nYAZILIMI KULLANDIGINIZ ICIN TESEKKUR EDERIZ\n";
                break;
            default:
                std::cout << "\nLutfen dogru secenegi girin\n";
                break;
        }
        
        if (secim != 7) {
            bekle();
        }
        
    } while (secim != 7);
}

void OtelConsoleUI::anaMenuGoster() {
    std::cout << "\n######## Otel Yonetimi #########\n";
    std::cout << "1. Odalari Yonet\n";
    std::cout << "2. Odaya Check-In Yap\n";
    std::cout << "3. Musait Odalar\n";
    std::cout << "4. Musteri Ara\n";
    std::cout << "5. Odaya Check-Out Yap\n";
    std::cout << "6. Misafir Ozet Raporu\n";
    std::cout << "7. Cikis\n";
    std::cout << "\nSecenek Girin: ";
}

void OtelConsoleUI::odalariYonet() {
    int secim;
    
    do {
        std::cout << "\n### Odalari Yonet ###\n";
        std::cout << "1. Oda Ekle\n";
        std::cout << "2. Oda Ara\n";
        std::cout << "3. Ana Menuye Don\n";
        std::cout << "\nSecenek Girin: ";
        std::cin >> secim;
        
        switch (secim) {
            case 1:
                odaEkle();
                break;
            case 2:
                odaAra();
                break;
            case 3:
                break;
            default:
                std::cout << "\nLutfen dogru secenegi girin\n";
                break;
        }
        
        if (secim != 3 && secim >= 1 && secim <= 2) {
            bekle();
        }
        
    } while (secim != 3);
}

void OtelConsoleUI::odaEkle() {
    OdaEkleRequest request;
    
    std::cout << "\nOda Numarasi Girin: ";
    std::cin >> request.oda_numarasi;
    
    std::cout << "Tip Klima/Klimasiz (K/N): ";
    std::cin >> request.klima_tipi;
    
    std::cout << "Tip Konfor (L/S): ";
    std::cin >> request.konfor_tipi;
    
    std::cout << "Tip Boyut (B/K): ";
    std::cin >> request.boyut_tipi;
    
    std::cout << "Gunluk Kira: ";
    std::cin >> request.gunluk_kira;
    
    OdaEkleResponse response = oda_ekle_use_case->execute(request);
    
    std::cout << "\n" << response.mesaj << "\n";
}

void OtelConsoleUI::odaAra() {
    int oda_numarasi;
    
    std::cout << "\nOda numarasini girin: ";
    std::cin >> oda_numarasi;
    
    auto oda_opt = otel_service->odaAra(oda_numarasi);
    
    if (oda_opt.has_value()) {
        const Oda& oda = oda_opt.value();
        std::cout << "\nOda Detaylari:\n";
        
        if (oda.isMusait()) {
            std::cout << "Oda Musait\n";
        } else {
            std::cout << "Oda Rezerve Edilmis\n";
        }
        
        odaBilgileriniGoster(oda);
    } else {
        std::cout << "\nOda bulunamadi\n";
    }
}

void OtelConsoleUI::checkIn() {
    auto odalar = otel_service->musaitOdalariGetir();
    
    if (odalar.empty()) {
        std::cout << "\nOda verisi mevcut degil.\nLutfen once odalari ekleyin.\n";
        return;
    }
    
    CheckInRequest request;
    
    std::cout << "\nOda numarasini girin: ";
    std::cin >> request.oda_numarasi;
    
    std::cout << "Rezervasyon ID'si girin: ";
    std::cin >> request.rezervasyon_id;
    
    std::cin.ignore(); // Buffer temizle
    
    std::cout << "Musteri Adi (Ilk Adi): ";
    std::getline(std::cin, request.musteri_ismi);
    
    std::cout << "Adres (sadece sehir): ";
    std::getline(std::cin, request.adres);
    
    std::cout << "Telefon: ";
    std::getline(std::cin, request.telefon);
    
    std::cout << "Giris Tarihi: ";
    std::getline(std::cin, request.giris_tarihi);
    
    std::cout << "Cikis Tarihi: ";
    std::getline(std::cin, request.cikis_tarihi);
    
    std::cout << "On Odeme: ";
    std::cin >> request.on_odeme;
    
    CheckInResponse response = check_in_use_case->execute(request);
    
    std::cout << "\n" << response.mesaj << "\n";
}

void OtelConsoleUI::musaitOdalarGoster() {
    auto odalar = otel_service->musaitOdalariGetir();
    
    if (odalar.empty()) {
        std::cout << "\nTum odalar rezerve edilmis veya hic oda eklenmemis\n";
        return;
    }
    
    std::cout << "\nMusait Odalar:\n";
    std::cout << "==============\n";
    
    for (const auto& oda : odalar) {
        odaBilgileriniGoster(oda);
        std::cout << "\n--------------------------\n";
    }
}

void OtelConsoleUI::musteriAra() {
    auto rezerve_odalar = otel_service->misafirOzetRaporu();
    
    if (rezerve_odalar.empty()) {
        std::cout << "\nOdalar mevcut degil.\nLutfen once odalari ekleyin.\n";
        return;
    }
    
    std::string musteri_ismi;
    std::cin.ignore(); // Buffer temizle
    std::cout << "Musteri Adini Girin: ";
    std::getline(std::cin, musteri_ismi);
    
    auto bulunan_odalar = otel_service->musteriAra(musteri_ismi);
    
    if (bulunan_odalar.empty()) {
        std::cout << "\nKisi bulunamadi.\n";
        return;
    }
    
    std::cout << "\nBulunan Kayitlar:\n";
    std::cout << "=================\n";
    
    for (const auto& oda : bulunan_odalar) {
        musteriBilgileriniGoster(oda);
        std::cout << "\n--------------------------\n";
    }
}

void OtelConsoleUI::checkOut() {
    auto rezerve_odalar = otel_service->misafirOzetRaporu();
    
    if (rezerve_odalar.empty()) {
        std::cout << "\nOdalar mevcut degil.\nLutfen once odalari ekleyin.\n";
        return;
    }
    
    CheckOutRequest request;
    
    std::cout << "Oda Numarasini Girin: ";
    std::cin >> request.oda_numarasi;
    
    std::cout << "Gun Sayisini Girin: ";
    std::cin >> request.gun_sayisi;
    
    CheckOutResponse response = check_out_use_case->execute(request);
    
    if (response.basarili) {
        std::cout << "\n\t######## CheckOut Detaylari ########\n";
        std::cout << response.musteri_bilgileri << "\n";
        std::cout << "Toplam Borc: " << response.toplam_fatura << " TL\n";
        std::cout << "On Odeme: " << response.on_odeme << " TL\n";
        std::cout << "*** Odenecek Toplam: " << response.kalan_borc << " TL sadece\n";
    } else {
        std::cout << "\n" << response.mesaj << "\n";
    }
}

void OtelConsoleUI::misafirOzetRaporu() {
    auto rezerve_odalar = otel_service->misafirOzetRaporu();
    
    if (rezerve_odalar.empty()) {
        std::cout << "\nOtelde Misafir Yok !!\n";
        return;
    }
    
    std::cout << "\nMisafir Ozet Raporu:\n";
    std::cout << "====================\n";
    
    for (const auto& oda : rezerve_odalar) {
        musteriBilgileriniGoster(oda);
        std::cout << "\n---------------------------------------\n";
    }
}

void OtelConsoleUI::odaBilgileriniGoster(const Oda& oda) {
    std::cout << "Oda Numarasi: " << oda.getOdaNumarasi() << "\n";
    std::cout << "Tip Klima: " << Oda::odaTipiToString(oda.getKlimaTipi()) << "\n";
    std::cout << "Tip Konfor: " << Oda::konforTipiToString(oda.getKonforTipi()) << "\n";
    std::cout << "Tip Boyut: " << Oda::boyutTipiToString(oda.getBoyutTipi()) << "\n";
    std::cout << "Kira: " << oda.getGunlukKira() << " TL\n";
}

void OtelConsoleUI::musteriBilgileriniGoster(const Oda& oda) {
    const Musteri* musteri = oda.getMusteri();
    if (musteri) {
        std::cout << "Musteri Adi: " << musteri->getIsim() << "\n";
        std::cout << "Oda Numarasi: " << oda.getOdaNumarasi() << "\n";
        std::cout << "Adres (sadece sehir): " << musteri->getAdres() << "\n";
        std::cout << "Telefon: " << musteri->getTelefon() << "\n";
    }
}

void OtelConsoleUI::bekle() {
    std::cout << "\nDevam etmek icin Enter'a basin...";
    std::cin.get();
}
