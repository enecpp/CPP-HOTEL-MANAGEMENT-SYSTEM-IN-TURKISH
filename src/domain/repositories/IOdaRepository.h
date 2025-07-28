#pragma once
#include "../entities/Oda.h"
#include <vector>
#include <memory>
#include <optional>

class IOdaRepository {
public:
    virtual ~IOdaRepository() = default;
    
    virtual bool ekle(const Oda& oda) = 0;
    virtual std::optional<Oda> getByOdaNumarasi(int oda_numarasi) = 0;
    virtual bool guncelle(const Oda& oda) = 0;
    virtual bool sil(int oda_numarasi) = 0;
    virtual std::vector<Oda> tumOdalariGetir() = 0;
    virtual std::vector<Oda> musaitOdalariGetir() = 0;
    virtual std::vector<Oda> rezerveOdalariGetir() = 0;
    virtual bool odaVarMi(int oda_numarasi) = 0;
};
