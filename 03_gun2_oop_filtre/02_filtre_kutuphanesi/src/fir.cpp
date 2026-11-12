// fir.cpp
#include "filtre.hpp"
#include <cmath>

FIRFiltre::FIRFiltre(const float* katsayilar, size_t n)
    : n_((katsayilar == nullptr || n == 0) ? 1 : n),   // önce n_ kurulur (bildirim sırası!)
      b_(new float[n_]),                              // sonra n_ kullanılarak b_
      gecmis_(n_)                                     // en son geçmiş tamponu
{
    if (katsayilar == nullptr || n == 0) {
        b_[0] = 1.0f;
        return;
    }
    for (size_t k = 0; k < n_; k++) {
        b_[k] = katsayilar[k];
    }
}

FIRFiltre::~FIRFiltre()
{
    delete[] b_;
    // gecmis_ bir nesne, onun yıkıcısı bizden sonra kendiliğinden çalışır. Bize bir şey düşmüyor.
}

float FIRFiltre::isle(float x)
{
    gecmis_.ekle(x);

    // y[n] = Σ b[k] * x[n-k]. bak(k) henüz gelmemiş örnekler için 0 döndürüyor.
    double y = 0.0;
    for (size_t k = 0; k < n_; k++) {
        y += b_[k] * gecmis_.bak(k);
    }
    return static_cast<float>(y);
}

void FIRFiltre::sifirla()
{
    gecmis_.temizle();
}

// ------------------------------------------------------------------

bool fir_alcak_geciren_tasarla(float* katsayilar, size_t n, float fc, float fs)
{
    if (katsayilar == nullptr || n == 0 || fs <= 0.0f || fc <= 0.0f || fc >= fs / 2.0f) {
        return false;
    }

    const double PI = 3.14159265358979323846;
    const double fn = static_cast<double>(fc) / fs;        // normalize kesim frekansı
    const double orta = (n - 1) / 2.0;
    double toplam = 0.0;

    for (size_t i = 0; i < n; i++) {
        double m = i - orta;

        // İdeal alçak geçirenin dürtü yanıtı: sinc fonksiyonu
        double h = (m == 0.0) ? 2.0 * fn : std::sin(2.0 * PI * fn * m) / (PI * m);

        // Sonsuz uzun sinc'i kesince oluşan dalgalanmayı yumuşatmak için Hamming penceresi
        double w = (n == 1) ? 1.0 : 0.54 - 0.46 * std::cos(2.0 * PI * i / (n - 1));

        katsayilar[i] = static_cast<float>(h * w);
        toplam += h * w;
    }

    // DC kazancı tam 1 olsun: sabit bir sinyal filtreden aynen geçmeli.
    for (size_t i = 0; i < n; i++) {
        katsayilar[i] = static_cast<float>(katsayilar[i] / toplam);
    }
    return true;
}
