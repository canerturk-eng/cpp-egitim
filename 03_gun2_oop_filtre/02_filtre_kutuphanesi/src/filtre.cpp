// filtre.cpp
// Taban sınıfın sanal olmayan fonksiyonu ve filtre zinciri
#include "filtre.hpp"

void Filtre::isle_dizi(const float* giris, float* cikis, size_t n)
{
    if (giris == nullptr || cikis == nullptr) {
        return;
    }
    for (size_t i = 0; i < n; i++) {
        // isle() sanal: bu satır HareketliOrtalama için de FIR için de aynı,
        // hangi isle()'nin çalışacağına nesnenin gerçek tipi karar veriyor.
        cikis[i] = isle(giris[i]);
    }
}

// ------------------------------------------------------------------

bool FiltreZinciri::ekle(Filtre* f)
{
    if (f == nullptr || f == this || adet_ >= MAKS) {
        return false;
    }
    filtreler_[adet_] = f;
    adet_++;
    return true;
}

float FiltreZinciri::isle(float x)
{
    // Her halkanın çıkışı bir sonrakinin girişi
    for (size_t i = 0; i < adet_; i++) {
        x = filtreler_[i]->isle(x);
    }
    return x;
}

void FiltreZinciri::sifirla()
{
    for (size_t i = 0; i < adet_; i++) {
        filtreler_[i]->sifirla();
    }
}

size_t FiltreZinciri::gecikme() const
{
    size_t toplam = 0;
    for (size_t i = 0; i < adet_; i++) {
        toplam += filtreler_[i]->gecikme();
    }
    return toplam;
}
