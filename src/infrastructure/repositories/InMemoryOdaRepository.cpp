#include "InMemoryOdaRepository.h"

bool InMemoryOdaRepository::ekle(const Oda& oda) {
    if (odaVarMi(oda.getOdaNumarasi())) {
        return false; // Oda zaten mevcut
    }
    
    odalar.push_back(oda);
    return true;
}

std::optional<Oda> InMemoryOdaRepository::getByOdaNumarasi(int oda_numarasi) {
    auto it = findByOdaNumarasi(oda_numarasi);
    if (it != odalar.end()) {
        return *it;
    }
    return std::nullopt;
}

bool InMemoryOdaRepository::guncelle(const Oda& oda) {
    auto it = findByOdaNumarasi(oda.getOdaNumarasi());
    if (it != odalar.end()) {
        *it = oda;
        return true;
    }
    return false;
}

bool InMemoryOdaRepository::sil(int oda_numarasi) {
    auto it = findByOdaNumarasi(oda_numarasi);
    if (it != odalar.end()) {
        odalar.erase(it);
        return true;
    }
    return false;
}

std::vector<Oda> InMemoryOdaRepository::tumOdalariGetir() {
    return odalar;
}

std::vector<Oda> InMemoryOdaRepository::musaitOdalariGetir() {
    std::vector<Oda> musait_odalar;
    
    std::copy_if(odalar.begin(), odalar.end(), std::back_inserter(musait_odalar),
                 [](const Oda& oda) { return oda.isMusait(); });
    
    return musait_odalar;
}

std::vector<Oda> InMemoryOdaRepository::rezerveOdalariGetir() {
    std::vector<Oda> rezerve_odalar;
    
    std::copy_if(odalar.begin(), odalar.end(), std::back_inserter(rezerve_odalar),
                 [](const Oda& oda) { return !oda.isMusait(); });
    
    return rezerve_odalar;
}

bool InMemoryOdaRepository::odaVarMi(int oda_numarasi) {
    return findByOdaNumarasi(oda_numarasi) != odalar.end();
}

std::vector<Oda>::iterator InMemoryOdaRepository::findByOdaNumarasi(int oda_numarasi) {
    return std::find_if(odalar.begin(), odalar.end(),
                        [oda_numarasi](const Oda& oda) {
                            return oda.getOdaNumarasi() == oda_numarasi;
                        });
}
