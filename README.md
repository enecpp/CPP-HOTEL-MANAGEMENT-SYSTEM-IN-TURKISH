# Otel Yönetim Sistemi

Bu proje, C++ dilinde Onion Architecture ve OOP prensipleri kullanılarak geliştirilmiş bir otel yönetim sistemidir.

## Mimari

Proje Onion Architecture (Soğan Mimarisi) prensiplerine uygun olarak tasarlanmıştır:

### Katmanlar:

1. **Domain Layer (Çekirdek)** - `src/domain/`
   - **Entities**: `Musteri`, `Oda` varlık sınıfları
   - **Repository Interfaces**: `IOdaRepository` 
   - **Domain Services**: `OtelDomainService`

2. **Application Layer** - `src/application/`
   - **Use Cases**: İş mantığı operasyonları
     - `OdaEkleUseCase`
     - `CheckInUseCase` 
     - `CheckOutUseCase`

3. **Infrastructure Layer** - `src/infrastructure/`
   - **Repository Implementations**: `InMemoryOdaRepository`
   - Veri erişim katmanı

4. **Presentation Layer** - `src/presentation/`
   - **UI**: `OtelConsoleUI` - Konsol kullanıcı arayüzü

## Özellikler

- **Oda Yönetimi**: Oda ekleme, arama, görüntüleme
- **Rezervasyon**: Check-in ve check-out işlemleri
- **Müşteri Takibi**: Müşteri bilgileri ve arama
- **Raporlama**: Misafir özet raporu, müsait odalar

## OOP Prensipleri

- **Encapsulation**: Veri gizleme ve getter/setter metodları
- **Inheritance**: Repository interface implementasyonu
- **Polymorphism**: Virtual metodlar ve interface kullanımı
- **Abstraction**: Repository pattern ve use case pattern

## Modern C++ Özellikleri

- Smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- Enum classes
- `std::optional` kullanımı
- RAII prensipleri
- Move semantics

## Derleme

### CMake ile:

```bash
mkdir build
cd build
cmake ..
make
./bin/otel_yonetim
```

### Makefile ile:

```bash
make
make run
```

### Doğrudan g++ ile:

```bash
g++ -std=c++17 -Wall -Wextra -g -I. main.cpp \
    src/domain/entities/*.cpp \
    src/domain/services/*.cpp \
    src/application/use-cases/*.cpp \
    src/infrastructure/repositories/*.cpp \
    src/presentation/*.cpp \
    -o otel_yonetim
```

## Kullanım

Program başlatıldığında bir menü sistemi görüntülenir:

1. **Odaları Yönet**: Oda ekleme ve arama
2. **Check-In**: Müşteri giriş işlemleri
3. **Müsait Odalar**: Boş odaları listele
4. **Müşteri Ara**: İsme göre müşteri arama
5. **Check-Out**: Müşteri çıkış ve fatura hesaplama
6. **Misafir Özet Raporu**: Mevcut misafirleri listele
7. **Çıkış**: Programdan çık

## Dependency Injection

Proje, bağımlılık enjeksiyonu kullanarak gevşek bağlı bir yapı sağlar:

```cpp
// Infrastructure Layer
auto oda_repository = std::make_shared<InMemoryOdaRepository>();

// Domain Layer  
auto otel_service = std::make_shared<OtelDomainService>(oda_repository);

// Presentation Layer
auto console_ui = std::make_unique<OtelConsoleUI>(otel_service);
```

## Genişletilebilirlik

- **Yeni Repository**: Veritabanı desteği için `IOdaRepository` implement edin
- **Yeni UI**: Web veya GUI arayüzü için yeni presentation layer ekleyin
- **Yeni Use Cases**: Yeni iş mantığı operasyonları ekleyin
- **Yeni Entities**: Rezervasyon, Ödeme gibi yeni varlıklar ekleyin

## Mimari Avantajları

1. **Testability**: Her katman bağımsız test edilebilir
2. **Maintainability**: Kod bakımı kolay
3. **Scalability**: Yeni özellikler kolayca eklenebilir
4. **Separation of Concerns**: Her katmanın sorumluluğu açık
5. **Database Independence**: Repository pattern sayesinde veri kaynağı değiştirilebilir
