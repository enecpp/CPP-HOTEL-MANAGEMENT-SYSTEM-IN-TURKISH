#include "WebServer.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>

WebServer::WebServer(std::shared_ptr<OtelDomainService> service, int port)
    : otel_service(service), port(port), running(false) {
    oda_ekle_use_case = std::make_shared<OdaEkleUseCase>(service);
    check_in_use_case = std::make_shared<CheckInUseCase>(service);
    check_out_use_case = std::make_shared<CheckOutUseCase>(service);
}

WebServer::~WebServer() {
    stop();
}

void WebServer::start() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Socket oluştur
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket oluşturulamadı" << std::endl;
        return;
    }

    // Socket seçeneklerini ayarla
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Socket seçenekleri ayarlanamadı" << std::endl;
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Socket'i port'a bağla
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Port'a bağlanamadı" << std::endl;
        return;
    }

    // Dinlemeye başla
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Dinleme hatası" << std::endl;
        return;
    }

    running = true;
    std::cout << "🌐 Web Server başlatıldı: http://localhost:" << port << std::endl;
    std::cout << "📱 Tarayıcınızda yukarıdaki adresi açın!" << std::endl;

    while (running) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            if (running) {
                std::cerr << "Bağlantı kabul edilemedi" << std::endl;
            }
            continue;
        }

        // Her istek için yeni thread oluştur
        std::thread([this, client_socket]() {
            handleRequest(client_socket);
            close(client_socket);
        }).detach();
    }

    close(server_fd);
}

void WebServer::stop() {
    running = false;
}

void WebServer::handleRequest(int client_socket) {
    char buffer[4096] = {0};
    read(client_socket, buffer, 4096);
    
    std::string request(buffer);
    std::istringstream iss(request);
    std::string method, path, version;
    iss >> method >> path >> version;

    std::string response;
    
    if (method == "GET") {
        if (path == "/" || path == "/index.html") {
            response = generateHTML();
        } else if (path == "/odalar") {
            response = generateOdaListHTML();
        } else if (path == "/checkin") {
            response = generateCheckInFormHTML();
        } else if (path == "/checkout") {
            response = generateCheckOutFormHTML();
        } else if (path == "/oda-ekle") {
            response = generateOdaEkleFormHTML();
        } else if (path == "/rapor") {
            response = generateRaporHTML();
        } else if (path == "/api/musait-odalar") {
            response = handleMusaitOdalar();
        } else if (path == "/api/rapor") {
            response = handleRapor();
        } else {
            response = "HTTP/1.1 404 Not Found\r\n\r\n<h1>404 - Sayfa Bulunamadı</h1>";
        }
    } else if (method == "POST") {
        size_t content_length = 0;
        size_t pos = request.find("Content-Length: ");
        if (pos != std::string::npos) {
            std::string length_str = request.substr(pos + 16);
            content_length = std::stoi(length_str.substr(0, length_str.find("\r\n")));
        }
        
        size_t body_start = request.find("\r\n\r\n");
        std::string body;
        if (body_start != std::string::npos) {
            body = request.substr(body_start + 4);
        }
        
        auto params = parseFormData(body);
        
        if (path == "/api/oda-ekle") {
            response = handleOdaEkle(params);
        } else if (path == "/api/checkin") {
            response = handleCheckIn(params);
        } else if (path == "/api/checkout") {
            response = handleCheckOut(params);
        }
    }

    send(client_socket, response.c_str(), response.length(), 0);
}

std::string WebServer::generateHTML() {
    return R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8

<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🏨 Otel Yönetim Sistemi</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container { 
            max-width: 1200px; 
            margin: 0 auto; 
            background: white; 
            border-radius: 15px; 
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            overflow: hidden;
        }
        .header { 
            background: linear-gradient(45deg, #1e3c72 0%, #2a5298 100%);
            color: white; 
            padding: 30px; 
            text-align: center; 
        }
        .header h1 { font-size: 2.5em; margin-bottom: 10px; }
        .header p { font-size: 1.2em; opacity: 0.9; }
        .nav { 
            display: grid; 
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); 
            gap: 20px; 
            padding: 40px; 
        }
        .nav-item { 
            background: linear-gradient(45deg, #667eea 0%, #764ba2 100%);
            color: white; 
            padding: 25px; 
            text-decoration: none; 
            border-radius: 10px; 
            text-align: center; 
            transition: all 0.3s ease;
            box-shadow: 0 5px 15px rgba(0,0,0,0.1);
        }
        .nav-item:hover { 
            transform: translateY(-5px); 
            box-shadow: 0 10px 25px rgba(0,0,0,0.2);
        }
        .nav-item h3 { font-size: 1.3em; margin-bottom: 10px; }
        .nav-item p { opacity: 0.9; font-size: 0.9em; }
        .footer { 
            background: #f8f9fa; 
            padding: 20px; 
            text-align: center; 
            color: #666; 
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🏨 Otel Yönetim Sistemi</h1>
        </div>
        
        <div class="nav">
            <a href="/oda-ekle" class="nav-item">
                <h3>🏠 Oda Ekle</h3>
                <p>Otele yeni oda ekleyin</p>
            </a>
            
            <a href="/odalar" class="nav-item">
                <h3>🗂️ Odaları Görüntüle</h3>
                <p>Tüm odaları listeleyin</p>
            </a>
            
            <a href="/checkin" class="nav-item">
                <h3>🔑 Check-In</h3>
                <p>Müşteri girişi yapın</p>
            </a>
            
            <a href="/checkout" class="nav-item">
                <h3>🧾 Check-Out</h3>
                <p>Müşteri çıkışı ve fatura</p>
            </a>
            
            <a href="/rapor" class="nav-item">
                <h3>📊 Misafir Raporu</h3>
                <p>Mevcut misafirleri görüntüle</p>
            </a>
            
            <a href="/api/musait-odalar" class="nav-item">
                <h3>✅ Müsait Odalar</h3>
                <p>Boş odaları görüntüle</p>
            </a>
        </div>
        
        <div class="footer">
            <p>© 2025 Otel Yönetim Sistemi - Modern C++17 & SOLID Principles</p>
        </div>
    </div>
</body>
</html>)";
}

std::string WebServer::generateOdaEkleFormHTML() {
    return R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8

<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🏠 Oda Ekle - Otel Yönetim</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container { 
            max-width: 600px; 
            margin: 0 auto; 
            background: white; 
            border-radius: 15px; 
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            overflow: hidden;
        }
        .header { 
            background: linear-gradient(45deg, #1e3c72 0%, #2a5298 100%);
            color: white; 
            padding: 30px; 
            text-align: center; 
        }
        .form-container { padding: 40px; }
        .form-group { margin-bottom: 25px; }
        label { 
            display: block; 
            margin-bottom: 8px; 
            font-weight: 600; 
            color: #333; 
        }
        input, select { 
            width: 100%; 
            padding: 12px; 
            border: 2px solid #ddd; 
            border-radius: 8px; 
            font-size: 16px;
            transition: border-color 0.3s ease;
        }
        input:focus, select:focus { 
            outline: none; 
            border-color: #667eea; 
        }
        .btn { 
            background: linear-gradient(45deg, #667eea 0%, #764ba2 100%);
            color: white; 
            padding: 15px 30px; 
            border: none; 
            border-radius: 8px; 
            font-size: 16px; 
            cursor: pointer; 
            width: 100%;
            transition: transform 0.3s ease;
        }
        .btn:hover { transform: translateY(-2px); }
        .back-btn { 
            display: inline-block; 
            margin-bottom: 20px; 
            color: #667eea; 
            text-decoration: none; 
            font-weight: 600;
        }
        .back-btn:hover { text-decoration: underline; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🏠 Oda Ekle</h1>
        </div>
        
        <div class="form-container">
            <a href="/" class="back-btn">← Ana Sayfa</a>
            
            <form action="/api/oda-ekle" method="POST">
                <div class="form-group">
                    <label for="oda_numarasi">Oda Numarası:</label>
                    <input type="number" id="oda_numarasi" name="oda_numarasi" required min="1">
                </div>
                
                <div class="form-group">
                    <label for="klima_tipi">Klima Tipi:</label>
                    <select id="klima_tipi" name="klima_tipi" required>
                        <option value="">Seçiniz</option>
                        <option value="K">Klimali (K)</option>
                        <option value="N">Klimasiz (N)</option>
                    </select>
                </div>
                
                <div class="form-group">
                    <label for="konfor_tipi">Konfor Tipi:</label>
                    <select id="konfor_tipi" name="konfor_tipi" required>
                        <option value="">Seçiniz</option>
                        <option value="L">Lüks (L)</option>
                        <option value="S">Standart (S)</option>
                    </select>
                </div>
                
                <div class="form-group">
                    <label for="boyut_tipi">Boyut Tipi:</label>
                    <select id="boyut_tipi" name="boyut_tipi" required>
                        <option value="">Seçiniz</option>
                        <option value="B">Büyük (B)</option>
                        <option value="K">Küçük (K)</option>
                    </select>
                </div>
                
                <div class="form-group">
                    <label for="gunluk_kira">Günlük Kira (TL):</label>
                    <input type="number" id="gunluk_kira" name="gunluk_kira" required min="0" step="0.01">
                </div>
                
                <button type="submit" class="btn">🏠 Oda Ekle</button>
            </form>
        </div>
    </div>
</body>
</html>)";
}

std::string WebServer::handleOdaEkle(const std::map<std::string, std::string>& params) {
    OdaEkleRequest request;
    
    try {
        request.oda_numarasi = std::stoi(params.at("oda_numarasi"));
        request.klima_tipi = params.at("klima_tipi")[0];
        request.konfor_tipi = params.at("konfor_tipi")[0];
        request.boyut_tipi = params.at("boyut_tipi")[0];
        request.gunluk_kira = std::stof(params.at("gunluk_kira"));
        
        OdaEkleResponse response = oda_ekle_use_case->execute(request);
        
        std::string status = response.basarili ? "success" : "error";
        std::string icon = response.basarili ? "✅" : "❌";
        
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n"
               "<script>alert('" + icon + " " + response.mesaj + "'); window.location.href = '/';</script>";
        
    } catch (const std::exception& e) {
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n"
               "<script>alert('❌ Hata: Geçersiz veri'); window.location.href = '/oda-ekle';</script>";
    }
}

std::map<std::string, std::string> WebServer::parseFormData(const std::string& data) {
    std::map<std::string, std::string> params;
    std::istringstream stream(data);
    std::string pair;
    
    while (std::getline(stream, pair, '&')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            params[key] = urlDecode(value);
        }
    }
    
    return params;
}

std::string WebServer::urlDecode(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && i + 2 < str.length()) {
            int hex = std::stoi(str.substr(i + 1, 2), nullptr, 16);
            result += static_cast<char>(hex);
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}

std::string WebServer::handleMusaitOdalar() {
    auto odalar = otel_service->musaitOdalariGetir();
    
    std::ostringstream html;
    html << R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8

<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>✅ Müsait Odalar</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container { 
            max-width: 1000px; 
            margin: 0 auto; 
            background: white; 
            border-radius: 15px; 
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            overflow: hidden;
        }
        .header { 
            background: linear-gradient(45deg, #1e3c72 0%, #2a5298 100%);
            color: white; 
            padding: 30px; 
            text-align: center; 
        }
        .content { padding: 40px; }
        .back-btn { 
            display: inline-block; 
            margin-bottom: 20px; 
            color: #667eea; 
            text-decoration: none; 
            font-weight: 600;
        }
        table { 
            width: 100%; 
            border-collapse: collapse; 
            border-radius: 8px; 
            overflow: hidden;
            box-shadow: 0 5px 15px rgba(0,0,0,0.1);
        }
        th, td { padding: 15px; text-align: left; }
        th { 
            background: linear-gradient(45deg, #667eea 0%, #764ba2 100%);
            color: white; 
            font-weight: 600;
        }
        tr:nth-child(even) { background: #f8f9fa; }
        tr:hover { background: #e9ecef; }
        .no-rooms { 
            text-align: center; 
            padding: 40px; 
            color: #666; 
            font-size: 1.1em;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>✅ Müsait Odalar</h1>
        </div>
        
        <div class="content">
            <a href="/" class="back-btn">← Ana Sayfa</a>)";

    if (odalar.empty()) {
        html << R"(
            <div class="no-rooms">
                <h3>😕 Müsait oda bulunamadı</h3>
                <p>Tüm odalar rezerve edilmiş durumda.</p>
            </div>)";
    } else {
        html << R"(
            <table>
                <thead>
                    <tr>
                        <th>Oda No</th>
                        <th>Klima</th>
                        <th>Konfor</th>
                        <th>Boyut</th>
                        <th>Günlük Kira</th>
                    </tr>
                </thead>
                <tbody>)";
        
        for (const auto& oda : odalar) {
            html << "<tr>"
                 << "<td>" << oda.getOdaNumarasi() << "</td>"
                 << "<td>" << Oda::odaTipiToString(oda.getKlimaTipi()) << "</td>"
                 << "<td>" << Oda::konforTipiToString(oda.getKonforTipi()) << "</td>"
                 << "<td>" << Oda::boyutTipiToString(oda.getBoyutTipi()) << "</td>"
                 << "<td>" << oda.getGunlukKira() << " TL</td>"
                 << "</tr>";
        }
        
        html << R"(
                </tbody>
            </table>)";
    }

    html << R"(
        </div>
    </div>
</body>
</html>)";

    return html.str();
}

// Diğer metodları da implement edelim - basit versiyonlar
std::string WebServer::generateCheckInFormHTML() {
    return R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8

<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🔑 Check-In - Otel Yönetim</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container { 
            max-width: 600px; 
            margin: 0 auto; 
            background: white; 
            border-radius: 15px; 
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            overflow: hidden;
        }
        .header { 
            background: linear-gradient(45deg, #1e3c72 0%, #2a5298 100%);
            color: white; 
            padding: 30px; 
            text-align: center; 
        }
        .form-container { padding: 40px; }
        .form-group { margin-bottom: 20px; }
        label { 
            display: block; 
            margin-bottom: 8px; 
            font-weight: 600; 
            color: #333; 
        }
        input, select { 
            width: 100%; 
            padding: 12px; 
            border: 2px solid #ddd; 
            border-radius: 8px; 
            font-size: 16px;
            transition: border-color 0.3s ease;
        }
        input:focus, select:focus { 
            outline: none; 
            border-color: #667eea; 
        }
        .btn { 
            background: linear-gradient(45deg, #667eea 0%, #764ba2 100%);
            color: white; 
            padding: 15px 30px; 
            border: none; 
            border-radius: 8px; 
            font-size: 16px; 
            cursor: pointer; 
            width: 100%;
            transition: transform 0.3s ease;
        }
        .btn:hover { transform: translateY(-2px); }
        .back-btn { 
            display: inline-block; 
            margin-bottom: 20px; 
            color: #667eea; 
            text-decoration: none; 
            font-weight: 600;
        }
        .back-btn:hover { text-decoration: underline; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🔑 Check-In</h1>
        </div>
        
        <div class="form-container">
            <a href="/" class="back-btn">← Ana Sayfa</a>
            
            <form action="/api/checkin" method="POST">
                <div class="form-group">
                    <label for="oda_numarasi">Oda Numarası:</label>
                    <input type="number" id="oda_numarasi" name="oda_numarasi" required min="1">
                </div>
                
                <div class="form-group">
                    <label for="rezervasyon_id">Rezervasyon ID:</label>
                    <input type="number" id="rezervasyon_id" name="rezervasyon_id" required min="1">
                </div>
                
                <div class="form-group">
                    <label for="musteri_ismi">Müşteri İsmi:</label>
                    <input type="text" id="musteri_ismi" name="musteri_ismi" required>
                </div>
                
                <div class="form-group">
                    <label for="adres">Adres:</label>
                    <input type="text" id="adres" name="adres" required>
                </div>
                
                <div class="form-group">
                    <label for="telefon">Telefon:</label>
                    <input type="tel" id="telefon" name="telefon" required>
                </div>
                
                <div class="form-group">
                    <label for="giris_tarihi">Giriş Tarihi:</label>
                    <input type="date" id="giris_tarihi" name="giris_tarihi" required>
                </div>
                
                <div class="form-group">
                    <label for="cikis_tarihi">Çıkış Tarihi:</label>
                    <input type="date" id="cikis_tarihi" name="cikis_tarihi" required>
                </div>
                
                <div class="form-group">
                    <label for="on_odeme">Ön Ödeme (TL):</label>
                    <input type="number" id="on_odeme" name="on_odeme" required min="0" step="0.01">
                </div>
                
                <button type="submit" class="btn">🔑 Check-In Yap</button>
            </form>
        </div>
    </div>
</body>
</html>)";
}

std::string WebServer::generateCheckOutFormHTML() {
    return R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8

<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🧾 Check-Out - Otel Yönetim</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container { 
            max-width: 600px; 
            margin: 0 auto; 
            background: white; 
            border-radius: 15px; 
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            overflow: hidden;
        }
        .header { 
            background: linear-gradient(45deg, #1e3c72 0%, #2a5298 100%);
            color: white; 
            padding: 30px; 
            text-align: center; 
        }
        .form-container { padding: 40px; }
        .form-group { margin-bottom: 25px; }
        label { 
            display: block; 
            margin-bottom: 8px; 
            font-weight: 600; 
            color: #333; 
        }
        input { 
            width: 100%; 
            padding: 12px; 
            border: 2px solid #ddd; 
            border-radius: 8px; 
            font-size: 16px;
            transition: border-color 0.3s ease;
        }
        input:focus { 
            outline: none; 
            border-color: #667eea; 
        }
        .btn { 
            background: linear-gradient(45deg, #667eea 0%, #764ba2 100%);
            color: white; 
            padding: 15px 30px; 
            border: none; 
            border-radius: 8px; 
            font-size: 16px; 
            cursor: pointer; 
            width: 100%;
            transition: transform 0.3s ease;
        }
        .btn:hover { transform: translateY(-2px); }
        .back-btn { 
            display: inline-block; 
            margin-bottom: 20px; 
            color: #667eea; 
            text-decoration: none; 
            font-weight: 600;
        }
        .back-btn:hover { text-decoration: underline; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🧾 Check-Out</h1>
        </div>
        
        <div class="form-container">
            <a href="/" class="back-btn">← Ana Sayfa</a>
            
            <form action="/api/checkout" method="POST">
                <div class="form-group">
                    <label for="oda_numarasi">Oda Numarası:</label>
                    <input type="number" id="oda_numarasi" name="oda_numarasi" required min="1">
                </div>
                
                <div class="form-group">
                    <label for="gun_sayisi">Konaklama Gün Sayısı:</label>
                    <input type="number" id="gun_sayisi" name="gun_sayisi" required min="1">
                </div>
                
                <button type="submit" class="btn">🧾 Check-Out Yap</button>
            </form>
        </div>
    </div>
</body>
</html>)";
}

std::string WebServer::generateOdaListHTML() {
    auto tum_odalar = otel_service->odaAra(-1); // Tüm odaları getirmek için özel bir method gerekebilir
    // Alternatif olarak domain service'e getAllRooms() metodu ekleyelim
    
    std::ostringstream html;
    html << R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8

<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🗂️ Tüm Odalar</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container { 
            max-width: 1200px; 
            margin: 0 auto; 
            background: white; 
            border-radius: 15px; 
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            overflow: hidden;
        }
        .header { 
            background: linear-gradient(45deg, #1e3c72 0%, #2a5298 100%);
            color: white; 
            padding: 30px; 
            text-align: center; 
        }
        .content { padding: 40px; }
        .back-btn { 
            display: inline-block; 
            margin-bottom: 20px; 
            color: #667eea; 
            text-decoration: none; 
            font-weight: 600;
        }
        .room-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(300px, 1fr));
            gap: 20px;
            margin-top: 20px;
        }
        .room-card {
            background: #f8f9fa;
            border-radius: 10px;
            padding: 20px;
            box-shadow: 0 5px 15px rgba(0,0,0,0.1);
            transition: transform 0.3s ease;
        }
        .room-card:hover {
            transform: translateY(-5px);
        }
        .room-number {
            font-size: 1.5em;
            font-weight: bold;
            color: #1e3c72;
            margin-bottom: 10px;
        }
        .room-details {
            font-size: 0.9em;
            color: #666;
            line-height: 1.6;
        }
        .status-badge {
            display: inline-block;
            padding: 5px 10px;
            border-radius: 20px;
            font-size: 0.8em;
            font-weight: bold;
            margin-top: 10px;
        }
        .status-available {
            background: #d4edda;
            color: #155724;
        }
        .status-occupied {
            background: #f8d7da;
            color: #721c24;
        }
        .no-rooms { 
            text-align: center; 
            padding: 40px; 
            color: #666; 
            font-size: 1.1em;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🗂️ Tüm Odalar</h1>
        </div>
        
        <div class="content">
            <a href="/" class="back-btn">← Ana Sayfa</a>
            
            <div class="room-grid">)";

    // Şimdilik müsait ve rezerve odaları birleştirerek gösterelim
    auto musait_odalar = otel_service->musaitOdalariGetir();
    auto rezerve_odalar = otel_service->misafirOzetRaporu();
    
    if (musait_odalar.empty() && rezerve_odalar.empty()) {
        html << R"(
            </div>
            <div class="no-rooms">
                <h3>😕 Henüz oda eklenmemiş</h3>
                <p>Sisteme oda eklemek için <a href="/oda-ekle">buraya tıklayın</a>.</p>
            </div>)";
    } else {
        // Müsait odaları göster
        for (const auto& oda : musait_odalar) {
            html << "<div class=\"room-card\">"
                 << "<div class=\"room-number\">Oda " << oda.getOdaNumarasi() << "</div>"
                 << "<div class=\"room-details\">"
                 << "Klima: " << Oda::odaTipiToString(oda.getKlimaTipi()) << "<br>"
                 << "Konfor: " << Oda::konforTipiToString(oda.getKonforTipi()) << "<br>"
                 << "Boyut: " << Oda::boyutTipiToString(oda.getBoyutTipi()) << "<br>"
                 << "Günlük Kira: " << oda.getGunlukKira() << " TL"
                 << "</div>"
                 << "<span class=\"status-badge status-available\">✅ Müsait</span>"
                 << "</div>";
        }
        
        // Rezerve odaları göster
        for (const auto& oda : rezerve_odalar) {
            html << "<div class=\"room-card\">"
                 << "<div class=\"room-number\">Oda " << oda.getOdaNumarasi() << "</div>"
                 << "<div class=\"room-details\">"
                 << "Klima: " << Oda::odaTipiToString(oda.getKlimaTipi()) << "<br>"
                 << "Konfor: " << Oda::konforTipiToString(oda.getKonforTipi()) << "<br>"
                 << "Boyut: " << Oda::boyutTipiToString(oda.getBoyutTipi()) << "<br>"
                 << "Günlük Kira: " << oda.getGunlukKira() << " TL<br>";
            
            if (oda.getMusteri()) {
                html << "Müşteri: " << oda.getMusteri()->getIsim();
            }
            
            html << "</div>"
                 << "<span class=\"status-badge status-occupied\">🔒 Rezerve</span>"
                 << "</div>";
        }
        
        html << "</div>";
    }

    html << R"(
        </div>
    </div>
</body>
</html>)";

    return html.str();
}

std::string WebServer::generateRaporHTML() {
    auto rezerve_odalar = otel_service->misafirOzetRaporu();
    
    std::ostringstream html;
    html << R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8

<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>📊 Misafir Raporu</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container { 
            max-width: 1200px; 
            margin: 0 auto; 
            background: white; 
            border-radius: 15px; 
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            overflow: hidden;
        }
        .header { 
            background: linear-gradient(45deg, #1e3c72 0%, #2a5298 100%);
            color: white; 
            padding: 30px; 
            text-align: center; 
        }
        .content { padding: 40px; }
        .back-btn { 
            display: inline-block; 
            margin-bottom: 20px; 
            color: #667eea; 
            text-decoration: none; 
            font-weight: 600;
        }
        .guest-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(350px, 1fr));
            gap: 20px;
            margin-top: 20px;
        }
        .guest-card {
            background: #f8f9fa;
            border-radius: 10px;
            padding: 25px;
            box-shadow: 0 5px 15px rgba(0,0,0,0.1);
            border-left: 5px solid #667eea;
        }
        .guest-name {
            font-size: 1.3em;
            font-weight: bold;
            color: #1e3c72;
            margin-bottom: 15px;
        }
        .guest-details {
            font-size: 0.95em;
            color: #666;
            line-height: 1.8;
        }
        .room-info {
            background: #e9ecef;
            padding: 10px;
            border-radius: 5px;
            margin-top: 10px;
            font-size: 0.9em;
        }
        .no-guests { 
            text-align: center; 
            padding: 40px; 
            color: #666; 
            font-size: 1.1em;
        }
        .stats {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        .stat-card {
            background: linear-gradient(45deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 20px;
            border-radius: 10px;
            text-align: center;
        }
        .stat-number {
            font-size: 2em;
            font-weight: bold;
            margin-bottom: 5px;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>📊 Misafir Raporu</h1>
        </div>
        
        <div class="content">
            <a href="/" class="back-btn">← Ana Sayfa</a>
            
            <div class="stats">
                <div class="stat-card">
                    <div class="stat-number">)" << rezerve_odalar.size() << R"(</div>
                    <div>Aktif Misafir</div>
                </div>
                <div class="stat-card">
                    <div class="stat-number">)" << otel_service->musaitOdalariGetir().size() << R"(</div>
                    <div>Müsait Oda</div>
                </div>
            </div>)";

    if (rezerve_odalar.empty()) {
        html << R"(
            <div class="no-guests">
                <h3>😊 Şu anda misafir bulunmuyor</h3>
                <p>Tüm odalar müsait durumda.</p>
            </div>)";
    } else {
        html << R"(
            <div class="guest-grid">)";
        
        for (const auto& oda : rezerve_odalar) {
            const Musteri* musteri = oda.getMusteri();
            if (musteri) {
                html << "<div class=\"guest-card\">"
                     << "<div class=\"guest-name\">👤 " << musteri->getIsim() << "</div>"
                     << "<div class=\"guest-details\">"
                     << "📞 <strong>Telefon:</strong> " << musteri->getTelefon() << "<br>"
                     << "📍 <strong>Adres:</strong> " << musteri->getAdres() << "<br>"
                     << "📅 <strong>Giriş:</strong> " << musteri->getGirisTarihi() << "<br>"
                     << "📅 <strong>Çıkış:</strong> " << musteri->getCikisTarihi() << "<br>"
                     << "💰 <strong>Ön Ödeme:</strong> " << musteri->getOnOdeme() << " TL<br>"
                     << "🆔 <strong>Rezervasyon ID:</strong> " << musteri->getRezervasyonId()
                     << "</div>"
                     << "<div class=\"room-info\">"
                     << "🏠 <strong>Oda " << oda.getOdaNumarasi() << "</strong> | "
                     << Oda::odaTipiToString(oda.getKlimaTipi()) << " | "
                     << Oda::konforTipiToString(oda.getKonforTipi()) << " | "
                     << "Günlük: " << oda.getGunlukKira() << " TL"
                     << "</div>"
                     << "</div>";
            }
        }
        
        html << "</div>";
    }

    html << R"(
        </div>
    </div>
</body>
</html>)";

    return html.str();
}

std::string WebServer::handleCheckIn(const std::map<std::string, std::string>& params) {
    CheckInRequest request;
    
    try {
        request.oda_numarasi = std::stoi(params.at("oda_numarasi"));
        request.rezervasyon_id = std::stoi(params.at("rezervasyon_id"));
        request.musteri_ismi = params.at("musteri_ismi");
        request.adres = params.at("adres");
        request.telefon = params.at("telefon");
        request.giris_tarihi = params.at("giris_tarihi");
        request.cikis_tarihi = params.at("cikis_tarihi");
        request.on_odeme = std::stof(params.at("on_odeme"));
        
        CheckInResponse response = check_in_use_case->execute(request);
        
        std::string icon = response.basarili ? "✅" : "❌";
        
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n"
               "<script>alert('" + icon + " " + response.mesaj + "'); window.location.href = '/';</script>";
        
    } catch (const std::exception& e) {
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n"
               "<script>alert('❌ Hata: Geçersiz veri'); window.location.href = '/checkin';</script>";
    }
}

std::string WebServer::handleCheckOut(const std::map<std::string, std::string>& params) {
    CheckOutRequest request;
    
    try {
        request.oda_numarasi = std::stoi(params.at("oda_numarasi"));
        request.gun_sayisi = std::stoi(params.at("gun_sayisi"));
        
        CheckOutResponse response = check_out_use_case->execute(request);
        
        if (response.basarili) {
            std::string fatura_bilgisi = 
                "\\n\\nFATURA BİLGİLERİ:\\n" +
                response.musteri_bilgileri + "\\n" +
                "Toplam Fatura: " + std::to_string(response.toplam_fatura) + " TL\\n" +
                "Ön Ödeme: " + std::to_string(response.on_odeme) + " TL\\n" +
                "Kalan Borç: " + std::to_string(response.kalan_borc) + " TL";
            
            return "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n"
                   "<script>alert('✅ " + response.mesaj + fatura_bilgisi + "'); window.location.href = '/';</script>";
        } else {
            return "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n"
                   "<script>alert('❌ " + response.mesaj + "'); window.location.href = '/checkout';</script>";
        }
        
    } catch (const std::exception& e) {
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n"
               "<script>alert('❌ Hata: Geçersiz veri'); window.location.href = '/checkout';</script>";
    }
}

std::string WebServer::handleRapor() {
    return "HTTP/1.1 200 OK\\r\\nContent-Type: text/html\\r\\n\\r\\n<h1>Rapor API - Yakında!</h1>";
}

std::string WebServer::getResponseHeaders(int content_length, const std::string& content_type) {
    return "HTTP/1.1 200 OK\\r\\nContent-Type: " + content_type + "\\r\\nContent-Length: " + 
           std::to_string(content_length) + "\\r\\n\\r\\n";
}
