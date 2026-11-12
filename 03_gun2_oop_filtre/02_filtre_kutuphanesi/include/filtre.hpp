// filtre.hpp
// Bütün filtrelerin uyduğu ortak arayüz ve kütüphanedeki filtre sınıfları.
//
// Hepsi "akış" (streaming) mantığıyla çalışır: her isle() çağrısında BİR örnek
// alır, BİR örnek döndürür. Geçmiş örnekleri nesnenin kendisi hatırlar.
// Bir mikrodenetleyicide ADC kesmesi her geldiğinde isle() çağırdığınızı düşünün.
#pragma once
#include <cstddef>
#include "ring_buffer.hpp"

// ==================================================================
// Soyut taban sınıf
// ==================================================================
class Filtre {
public:
    virtual ~Filtre() = default;

    // Yeni örneği al, filtrelenmiş örneği döndür.
    virtual float isle(float x) = 0;

    // Geçmişi unut, nesne ilk oluşturulduğu hâle dönsün.
    virtual void sifirla() = 0;

    virtual const char* ad() const = 0;

    // Filtrenin çıkışı girişin kaç örnek gerisinden geliyor (yaklaşık).
    // Saf sanal değil: varsayılanı 0, gecikmesi olan filtre kendisi söyler.
    virtual size_t gecikme() const { return 0; }

    // Bir diziyi baştan sona filtreler. Sanal değil, zira her filtre için aynı:
    // sadece isle()'yi sırayla çağırıyor. giris == cikis olabilir (yerinde filtreleme).
    // nullptr gelirse bir şey yapmaz.
    void isle_dizi(const float* giris, float* cikis, size_t n);
};

// ==================================================================
// Nedensel hareketli ortalama (Ödev 04'teki hareketli_ortalama'nın sınıf hâli).
// Pencere henüz dolmadıysa eldeki örneklerin ortalamasını verir.
// ==================================================================
class HareketliOrtalama : public Filtre {
public:
    explicit HareketliOrtalama(size_t pencere);   // pencere 0 ise 1 kabul edilir

    float isle(float x) override;
    void sifirla() override;
    const char* ad() const override { return "Hareketli ortalama"; }
    size_t gecikme() const override { return (gecmis_.kapasite() - 1) / 2; }

private:
    RingBuffer gecmis_;
    double toplam_;     // penceredeki örneklerin toplamı, her adımda güncellenir
};

// ==================================================================
// Nedensel medyan filtre: son 'pencere' örneğin medyanı.
// Ödev 04'teki merkezi medyan, geleceği bilmek zorundaydı. Gerçek zamanlı çalışırken
// gelecek örnekleri bilemeyiz, bu yüzden sadece geçmişe bakıyoruz. Bedeli: pencere/2 örnek gecikme.
// ==================================================================
class MedyanFiltre : public Filtre {
public:
    static constexpr size_t MAKS_PENCERE = 31;

    // Pencere çiftse bir artırılır, 0 ise 1, MAKS_PENCERE üstüyse MAKS_PENCERE olur.
    explicit MedyanFiltre(size_t pencere);

    float isle(float x) override;
    void sifirla() override;
    const char* ad() const override { return "Medyan"; }
    size_t gecikme() const override { return gecmis_.kapasite() / 2; }

private:
    RingBuffer gecmis_;
    float siralama_[MAKS_PENCERE];   // sıralama için çalışma alanı (heap'e gerek yok)
};

// ==================================================================
// FIR (sonlu dürtü yanıtlı) filtre:
//   y[n] = b[0]*x[n] + b[1]*x[n-1] + ... + b[N-1]*x[n-N+1]
// Henüz gelmemiş geçmiş örnekler sıfır kabul edilir.
// ==================================================================
class FIRFiltre : public Filtre {
public:
    // Katsayılar kopyalanır, dizinin kendisini saklamayız.
    // katsayilar nullptr ya da n == 0 ise tek katsayılı {1.0} filtre olur (girişi aynen geçirir).
    FIRFiltre(const float* katsayilar, size_t n);
    ~FIRFiltre() override;

    FIRFiltre(const FIRFiltre&) = delete;
    FIRFiltre& operator=(const FIRFiltre&) = delete;

    float isle(float x) override;
    void sifirla() override;
    const char* ad() const override { return "FIR"; }
    size_t gecikme() const override { return (n_ - 1) / 2; }   // simetrik katsayılar için

    size_t katsayi_sayisi() const { return n_; }

private:
    size_t     n_;
    float*     b_;
    RingBuffer gecmis_;
};

// Pencereli sinc yöntemiyle alçak geçiren FIR katsayıları tasarlar (Hamming penceresi).
// fc: kesim frekansı, fs: örnekleme frekansı (ikisi de Hz). Katsayıların toplamı 1 olur,
// yani DC kazancı 1'dir. n tek sayı olursa filtre simetrik ve gecikmesi tam (n-1)/2 olur.
// Geçersiz girdide (nullptr, n == 0, fs <= 0, fc <= 0 ya da fc >= fs/2) false döner.
bool fir_alcak_geciren_tasarla(float* katsayilar, size_t n, float fc, float fs);

// ==================================================================
// Birinci dereceden IIR (sonsuz dürtü yanıtlı) alçak geçiren filtre:
//   y[n] = y[n-1] + alfa * (x[n] - y[n-1])
// Geçmişi tek bir sayıda (y[n-1]) tutar, yani bellek derdi yok. y[-1] = 0 kabul edilir.
// RC devresinin ayrık zamandaki karşılığı.
// ==================================================================
class IIRAlcakGeciren : public Filtre {
public:
    // alfa (0, 1] aralığına sıkıştırılır. alfa = 1 filtre yok demek, küçüldükçe filtre güçlenir.
    explicit IIRAlcakGeciren(float alfa);

    // Kesim frekansından alfa hesaplar: alfa = 1 - exp(-2*pi*fc/fs)
    static float alfa_hesapla(float fc, float fs);

    float isle(float x) override;
    void sifirla() override;
    const char* ad() const override { return "IIR alcak geciren"; }
    size_t gecikme() const override;   // düşük frekanslar için yaklaşık (1 - alfa) / alfa

    float alfa() const { return alfa_; }

private:
    float alfa_;
    float y_;
};

// ==================================================================
// Filtre zinciri: başka filtreleri arka arkaya bağlar. Kendisi de bir Filtre!
// Yani bir zinciri başka bir zincirin içine koymak da mümkün.
// Filtrelerin SAHİBİ DEĞİLDİR: onları silmez, sadece pointer'larını tutar.
// Eklenen filtreler zincirden önce ölmemeli.
// ==================================================================
class FiltreZinciri : public Filtre {
public:
    static constexpr size_t MAKS = 8;

    // nullptr, zincirin kendisi ya da zincir doluysa eklemez, false döner.
    bool ekle(Filtre* f);

    float isle(float x) override;
    void sifirla() override;
    const char* ad() const override { return "Zincir"; }
    size_t gecikme() const override;   // halkaların gecikmelerinin toplamı

    size_t adet() const { return adet_; }

private:
    Filtre* filtreler_[MAKS] = {};
    size_t  adet_ = 0;
};
