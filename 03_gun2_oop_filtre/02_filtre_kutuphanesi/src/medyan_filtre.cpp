// medyan_filtre.cpp
#include "filtre.hpp"

namespace {   // isimsiz namespace: bu fonksiyon yalnızca bu dosyadan görünür

// Gün 1'de yazdığımız insertion sort. Küçük pencerelerde hızlı ve yeterli.
void insertion_sort(float* dizi, size_t n)
{
    for (size_t i = 1; i < n; i++) {
        float anahtar = dizi[i];
        size_t j = i;
        while (j > 0 && dizi[j - 1] > anahtar) {
            dizi[j] = dizi[j - 1];
            j--;
        }
        dizi[j] = anahtar;
    }
}

size_t pencere_duzelt(size_t pencere)
{
    if (pencere == 0) pencere = 1;
    if (pencere % 2 == 0) pencere++;
    if (pencere > MedyanFiltre::MAKS_PENCERE) pencere = MedyanFiltre::MAKS_PENCERE;
    return pencere;
}

}   // namespace

MedyanFiltre::MedyanFiltre(size_t pencere)
    : gecmis_(pencere_duzelt(pencere))
{
}

float MedyanFiltre::isle(float x)
{
    gecmis_.ekle(x);

    // Ring buffer'ın kendisini sıralarsak zaman sırası bozulur, o yüzden kopyasını sıralıyoruz.
    size_t n = gecmis_.adet();
    for (size_t i = 0; i < n; i++) {
        siralama_[i] = gecmis_.bak(i);
    }
    insertion_sort(siralama_, n);

    // Pencere doluyken n hep tek. Başlangıçta pencere dolmamışken çift olabilir,
    // o zaman ortadaki iki örneğin ortalamasını alıyoruz.
    if (n % 2 == 1) {
        return siralama_[n / 2];
    }
    return 0.5f * (siralama_[n / 2 - 1] + siralama_[n / 2]);
}

void MedyanFiltre::sifirla()
{
    gecmis_.temizle();
}
