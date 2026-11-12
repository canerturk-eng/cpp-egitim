// hareketli_ortalama.cpp
#include "filtre.hpp"

HareketliOrtalama::HareketliOrtalama(size_t pencere)
    : gecmis_(pencere),      // RingBuffer'ın yapıcısını çağırıyoruz, 0 gelirse kendisi 1 yapar
      toplam_(0.0)
{
}

float HareketliOrtalama::isle(float x)
{
    // Her seferinde penceredeki bütün örnekleri toplamak yerine toplamı güncelliyoruz:
    // pencereden düşen örneği çıkar, yeni geleni ekle. Pencere 64 de olsa 2 işlem.
    if (gecmis_.dolu()) {
        toplam_ -= gecmis_.bak(gecmis_.adet() - 1);   // en eski örnek
    }
    gecmis_.ekle(x);
    toplam_ += x;
    return static_cast<float>(toplam_ / gecmis_.adet());
}

void HareketliOrtalama::sifirla()
{
    gecmis_.temizle();
    toplam_ = 0.0;
}
