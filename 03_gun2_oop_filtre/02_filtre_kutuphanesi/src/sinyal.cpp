// sinyal.cpp
#include "sinyal.hpp"
#include <cmath>

SinyalUreteci::SinyalUreteci(float fs, unsigned tohum)
    : fs_(fs > 0.0f ? fs : 1.0f), uretec_(tohum)
{
}

void SinyalUreteci::sinus(float* tampon, size_t n, float frekans, float genlik, float ofset) const
{
    if (tampon == nullptr) return;
    const double PI = 3.14159265358979323846;
    for (size_t i = 0; i < n; i++) {
        tampon[i] = static_cast<float>(ofset + genlik * std::sin(2.0 * PI * frekans * i / fs_));
    }
}

void SinyalUreteci::gurultu_ekle(float* tampon, size_t n, float std_sapma)
{
    if (tampon == nullptr || std_sapma <= 0.0f) return;
    std::normal_distribution<float> dagilim(0.0f, std_sapma);
    for (size_t i = 0; i < n; i++) {
        tampon[i] += dagilim(uretec_);
    }
}

void SinyalUreteci::durtu_ekle(float* tampon, size_t n, float olasilik, float genlik)
{
    if (tampon == nullptr || olasilik <= 0.0f) return;
    std::uniform_real_distribution<float> zar(0.0f, 1.0f);
    for (size_t i = 0; i < n; i++) {
        if (zar(uretec_) < olasilik) {
            tampon[i] += (zar(uretec_) < 0.5f) ? genlik : -genlik;
        }
    }
}

// ------------------------------------------------------------------

float rms_fark_gecikmeli(const float* referans, const float* cikis, size_t n,
                         size_t gecikme, size_t atla)
{
    if (referans == nullptr || cikis == nullptr || gecikme >= n) return 0.0f;
    size_t son = n - gecikme;          // cikis[i + gecikme] dizinin dışına taşmasın
    if (atla >= son) return 0.0f;

    double toplam = 0.0;
    for (size_t i = atla; i < son; i++) {
        double fark = static_cast<double>(referans[i]) - cikis[i + gecikme];
        toplam += fark * fark;
    }
    return static_cast<float>(std::sqrt(toplam / (son - atla)));
}

float rms_fark(const float* a, const float* b, size_t n, size_t atla)
{
    return rms_fark_gecikmeli(a, b, n, 0, atla);
}
