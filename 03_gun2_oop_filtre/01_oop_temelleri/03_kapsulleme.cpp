// 03_kapsulleme.cpp
// Ödev 03'teki ring buffer'ı sınıfa çeviriyoruz. İç yapı gizli, dışarıya sadece arayüz açık.
//
// Derleme: g++ -std=c++17 -Wall -Wextra 03_kapsulleme.cpp -o kapsulleme

#include <iostream>
#include <cstddef>

class RingBuffer {
public:
    static constexpr size_t KAPASITE = 8;

    RingBuffer() : bas_(0), adet_(0) {}

    // Doluysa eklemez, false döner (Ödev 03'teki rb_ekle)
    bool ekle(float deger)
    {
        if (dolu()) return false;
        veri_[(bas_ + adet_) % KAPASITE] = deger;
        adet_++;
        return true;
    }

    // Doluysa en eskinin üzerine yazar (rb_ekle_ezerek)
    void ekle_ezerek(float deger)
    {
        if (dolu()) {
            veri_[bas_] = deger;              // en eskinin yerine yaz
            bas_ = (bas_ + 1) % KAPASITE;     // en eski artık bir sonraki
        } else {
            ekle(deger);
        }
    }

    bool cikar(float& deger)                  // pointer yerine referans
    {
        if (bos()) return false;
        deger = veri_[bas_];
        bas_ = (bas_ + 1) % KAPASITE;
        adet_--;
        return true;
    }

    // gecmis = 0 en yeni örnek
    bool bak(size_t gecmis, float& deger) const
    {
        if (gecmis >= adet_) return false;
        deger = veri_[(bas_ + adet_ - 1 - gecmis) % KAPASITE];
        return true;
    }

    size_t adet() const { return adet_; }
    bool   bos()  const { return adet_ == 0; }
    bool   dolu() const { return adet_ == KAPASITE; }

private:
    // Bu üç alanın birbiriyle tutarlı olması gerekiyor (değişmez / invariant):
    //   bas_ < KAPASITE  ve  adet_ <= KAPASITE
    // Dışarıdan kimse bunları elle değiştiremediği için tutarlılığı yalnızca
    // yukarıdaki fonksiyonlar korumak zorunda. Hata ararken bakacağımız yer de belli.
    float  veri_[KAPASITE];
    size_t bas_;
    size_t adet_;
};

// const referans: kopyalamadan al, değiştirmeyeceğim diye söz ver.
// Bu yüzden içeride yalnızca const üye fonksiyonları çağırabiliriz.
void yazdir(const RingBuffer& rb)
{
    std::cout << "  [" << rb.adet() << " ornek, yeniden eskiye] ";
    for (size_t i = 0; i < rb.adet(); i++) {
        float x;
        rb.bak(i, x);
        std::cout << x << " ";
    }
    std::cout << "\n";
    // rb.ekle(1.0f);   // DERLENMEZ: ekle const değil
}

int main()
{
    RingBuffer rb;

    for (int i = 1; i <= 10; i++) {
        bool eklendi = rb.ekle(i * 1.0f);
        if (!eklendi) std::cout << "  " << i << " eklenemedi, tampon dolu\n";
    }
    yazdir(rb);

    float x;
    rb.cikar(x);
    std::cout << "cikarilan: " << x << "\n";

    rb.ekle_ezerek(100.0f);
    rb.ekle_ezerek(200.0f);   // dolu, en eskinin üzerine yazar
    yazdir(rb);

    // rb.adet_ = 50;   // DERLENMEZ. Struct olsaydı derlenirdi ve tampon bozulurdu.

    return 0;
}
