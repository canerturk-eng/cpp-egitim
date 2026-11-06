// filtre.hpp
// ARAYÜZ DOSYASI: Bu dosyayı DEĞİŞTİRMEYİN.
#pragma once
#include <cstddef>

constexpr size_t ORTALAMA_MAKS_PENCERE = 64;
constexpr size_t MEDYAN_MAKS_PENCERE   = 31;

// Küçükten büyüğe yerinde (in place) sıralar. Derste yazdığımız insertion sort.
// dizi nullptr ise bir şey yapmaz.
void insertion_sort(float* dizi, size_t n);

// NEDENSEL hareketli ortalama:
//   cikis[i] = giris[max(0, i − pencere + 1) .. i] aralığının ortalaması
// Yani ilk örneklerde pencere henüz dolmadığı için eldeki kadar örneğin ortalaması alınır.
// Geçersiz durumda (nullptr, pencere 0 ya da ORTALAMA_MAKS_PENCERE üstü) false döner.
bool hareketli_ortalama(const float* giris, float* cikis, size_t n, size_t pencere);

// MERKEZİ medyan filtre:
//   cikis[i] = giris[i − k .. i + k] aralığının medyanı   (k = pencere / 2)
// Dizinin dışına taşan indeksler en yakın kenar örneğiyle doldurulur
// (i − k < 0 ise giris[0], i + k >= n ise giris[n − 1] kullanılır).
// Geçersiz durumda (nullptr, giris == cikis, pencere çift, 0 ya da
// MEDYAN_MAKS_PENCERE üstü) false döner.
bool medyan_filtre(const float* giris, float* cikis, size_t n, size_t pencere);
