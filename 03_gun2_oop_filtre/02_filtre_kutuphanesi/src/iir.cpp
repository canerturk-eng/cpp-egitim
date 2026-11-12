// iir.cpp
#include "filtre.hpp"
#include <cmath>

IIRAlcakGeciren::IIRAlcakGeciren(float alfa)
    : alfa_(alfa), y_(0.0f)
{
    // alfa <= 0 olursa çıkış hiç değişmez, alfa > 1 olursa filtre salınır. İkisine de izin yok.
    if (!(alfa_ > 0.0f)) alfa_ = 1e-6f;   // !(a > 0) yazımı NaN'ı da yakalar
    if (alfa_ > 1.0f) alfa_ = 1.0f;
}

float IIRAlcakGeciren::alfa_hesapla(float fc, float fs)
{
    if (fs <= 0.0f || fc <= 0.0f) {
        return 1.0f;
    }
    const double PI = 3.14159265358979323846;
    return static_cast<float>(1.0 - std::exp(-2.0 * PI * fc / fs));
}

float IIRAlcakGeciren::isle(float x)
{
    // Çıkış, yeni örneğe doğru alfa oranında yaklaşıyor.
    // alfa küçükse yavaş yaklaşır (güçlü filtre), büyükse hızlı (zayıf filtre).
    y_ = y_ + alfa_ * (x - y_);
    return y_;
}

void IIRAlcakGeciren::sifirla()
{
    y_ = 0.0f;
}

size_t IIRAlcakGeciren::gecikme() const
{
    // Düşük frekanslı bir sinyal için grup gecikmesi yaklaşık (1 - alfa) / alfa örnek.
    return static_cast<size_t>((1.0f - alfa_) / alfa_ + 0.5f);
}
