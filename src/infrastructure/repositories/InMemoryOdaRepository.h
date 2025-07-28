#pragma once
#include "../../domain/repositories/IOdaRepository.h"
#include <vector>
#include <algorithm>

class InMemoryOdaRepository : public IOdaRepository {
private:
    std::vector<Oda> odalar;

public:
    InMemoryOdaRepository() = default;
    ~InMemoryOdaRepository() override = default;
    
    bool ekle(const Oda& oda) override;
    std::optional<Oda> getByOdaNumarasi(int oda_numarasi) override;
    bool guncelle(const Oda& oda) override;
    bool sil(int oda_numarasi) override;
    std::vector<Oda> tumOdalariGetir() override;
    std::vector<Oda> musaitOdalariGetir() override;
    std::vector<Oda> rezerveOdalariGetir() override;
    bool odaVarMi(int oda_numarasi) override;
    
private:
    std::vector<Oda>::iterator findByOdaNumarasi(int oda_numarasi);
};
