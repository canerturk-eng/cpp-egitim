// sinyal.hpp
// Filtreleri sınamak için test sinyali üreteci (Ödev 02'deki sinus_uret'in sınıf hâli)
// ve ölçüm fonksiyonları.
#pragma once
#include <cstddef>
#include <random>

class SinyalUreteci {
public:
    // fs: örnekleme frekansı (Hz). tohum: aynı tohum her seferinde aynı gürültüyü üretir,
    // böylece testler ve deneyler tekrarlanabilir olur.
    explicit SinyalUreteci(float fs, unsigned tohum = 2026);

    // tampon[i] = ofset + genlik * sin(2*pi * frekans * i / fs)
    void sinus(float* tampon, size_t n, float frekans, float genlik, float ofset = 0.0f) const;

    // Her örneğe ortalaması 0, standart sapması std_sapma olan Gauss gürültüsü ekler.
    void gurultu_ekle(float* tampon, size_t n, float std_sapma);

    // Her örnek 'olasilik' ihtimalle +genlik ya da -genlik kadar sıçrar.
    // Kopuk kablo, ESD ya da bit hatası gibi ani bozulmaların modeli.
    void durtu_ekle(float* tampon, size_t n, float olasilik, float genlik);

    float fs() const { return fs_; }

private:
    float fs_;
    std::mt19937 uretec_;    // standart kütüphanenin rastgele sayı üreteci
};

// sqrt( ortalama( (a[i] - b[i])^2 ) ),  i = atla .. n-1
// atla: baştaki geçici rejimi (filtre henüz oturmamışken) hesaba katmamak için.
// nullptr ya da atla >= n ise 0.0 döner.
float rms_fark(const float* a, const float* b, size_t n, size_t atla = 0);

// rms_fark ile aynı, lakin cikis'i 'gecikme' örnek geri kaydırarak karşılaştırır:
//   referans[i]  <->  cikis[i + gecikme]
// Filtrenin geciktirdiği sinyali, gecikmeyi telafi ederek ölçmek için.
float rms_fark_gecikmeli(const float* referans, const float* cikis, size_t n,
                         size_t gecikme, size_t atla = 0);
