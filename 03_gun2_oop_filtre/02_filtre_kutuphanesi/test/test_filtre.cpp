// test_filtre.cpp
// Filtre kütüphanesinin testleri. Çalıştırmak için: make test
#include "mini_test.hpp"
#include "filtre.hpp"
#include "sinyal.hpp"
#include <cstring>

static void test_ring_buffer()
{
    BOLUM("RingBuffer");

    RingBuffer rb(3);
    KONTROL(rb.kapasite() == 3);
    KONTROL(rb.bos());
    KONTROL(!rb.dolu());
    KONTROL(rb.bak(0) == 0.0f);          // boşken 0 döner

    rb.ekle(1.0f);
    rb.ekle(2.0f);
    KONTROL(rb.adet() == 2);
    KONTROL(rb.bak(0) == 2.0f);          // en yeni
    KONTROL(rb.bak(1) == 1.0f);
    KONTROL(rb.bak(2) == 0.0f);          // henüz yok

    rb.ekle(3.0f);
    rb.ekle(4.0f);                       // dolu, 1'in üzerine yazar
    KONTROL(rb.dolu());
    KONTROL(rb.adet() == 3);
    KONTROL(rb.bak(0) == 4.0f);
    KONTROL(rb.bak(1) == 3.0f);
    KONTROL(rb.bak(2) == 2.0f);

    for (int i = 5; i <= 20; i++) rb.ekle(static_cast<float>(i));   // birkaç tur dönsün
    KONTROL(rb.bak(0) == 20.0f);
    KONTROL(rb.bak(2) == 18.0f);

    rb.temizle();
    KONTROL(rb.bos());
    KONTROL(rb.bak(0) == 0.0f);

    RingBuffer sifir(0);                 // 0 kapasite 1'e çekilir
    KONTROL(sifir.kapasite() == 1);
    sifir.ekle(7.0f);
    sifir.ekle(8.0f);
    KONTROL(sifir.bak(0) == 8.0f);
    KONTROL(sifir.adet() == 1);
}

static void test_hareketli_ortalama()
{
    BOLUM("HareketliOrtalama");

    HareketliOrtalama f(3);
    YAKIN(f.isle(1.0f), 1.0, 1e-6);      // eldeki: {1}
    YAKIN(f.isle(2.0f), 1.5, 1e-6);      // {1, 2}
    YAKIN(f.isle(3.0f), 2.0, 1e-6);      // {1, 2, 3}
    YAKIN(f.isle(4.0f), 3.0, 1e-6);      // {2, 3, 4}
    YAKIN(f.isle(5.0f), 4.0, 1e-6);      // {3, 4, 5}
    KONTROL(f.gecikme() == 1);

    f.sifirla();
    YAKIN(f.isle(10.0f), 10.0, 1e-6);    // geçmiş gerçekten unutuldu mu?

    // Sabit girişte çıkış da sabit kalmalı (uzun sürede toplam kaymamalı)
    HareketliOrtalama g(8);
    float y = 0.0f;
    for (int i = 0; i < 10000; i++) y = g.isle(0.1f);
    YAKIN(y, 0.1, 1e-5);
}

static void test_medyan()
{
    BOLUM("MedyanFiltre");

    MedyanFiltre f(3);
    YAKIN(f.isle(1.0f), 1.0, 1e-6);      // {1}
    YAKIN(f.isle(100.0f), 50.5, 1e-6);   // {1, 100}: çift adet, ortadaki ikisinin ortalaması
    YAKIN(f.isle(2.0f), 2.0, 1e-6);      // {1, 100, 2} -> 2   (sıçrama yok oldu)
    YAKIN(f.isle(3.0f), 3.0, 1e-6);      // {100, 2, 3} -> 3
    YAKIN(f.isle(4.0f), 3.0, 1e-6);      // {2, 3, 4}   -> 3
    KONTROL(f.gecikme() == 1);

    // Pencere düzeltmeleri
    MedyanFiltre cift(4);                // 5'e çıkmalı
    KONTROL(cift.gecikme() == 2);
    MedyanFiltre buyuk(1000);            // MAKS_PENCERE'ye inmeli
    KONTROL(buyuk.gecikme() == MedyanFiltre::MAKS_PENCERE / 2);
    MedyanFiltre sifir(0);               // 1 olmalı, girişi aynen geçirmeli
    YAKIN(sifir.isle(-3.5f), -3.5, 1e-6);

    // Tek bir sıçrama 5'lik medyandan hiç geçmemeli
    MedyanFiltre m5(5);
    float giris[10] = {1, 1, 1, 1, 1, 50, 1, 1, 1, 1};
    float cikis[10];
    m5.isle_dizi(giris, cikis, 10);
    bool sicrama_yok = true;
    for (int i = 0; i < 10; i++) {
        if (cikis[i] != 1.0f) sicrama_yok = false;
    }
    KONTROL(sicrama_yok);

    f.sifirla();
    YAKIN(f.isle(7.0f), 7.0, 1e-6);
}

static void test_fir()
{
    BOLUM("FIRFiltre");

    // Dürtü yanıtı katsayıların kendisi olmalı
    float b[3] = {0.5f, 0.25f, 0.25f};
    FIRFiltre f(b, 3);
    KONTROL(f.katsayi_sayisi() == 3);
    YAKIN(f.isle(1.0f), 0.5, 1e-6);
    YAKIN(f.isle(0.0f), 0.25, 1e-6);
    YAKIN(f.isle(0.0f), 0.25, 1e-6);
    YAKIN(f.isle(0.0f), 0.0, 1e-6);
    KONTROL(f.gecikme() == 1);

    // Katsayılar kopyalanmış olmalı: dizinin aslını değiştirmek filtreyi etkilememeli
    b[0] = 100.0f;
    f.sifirla();
    YAKIN(f.isle(1.0f), 0.5, 1e-6);

    // Basamak girişi: başlangıçta geçmiş sıfır kabul ediliyor
    FIRFiltre ort(nullptr, 0);           // geçersiz girdi -> {1.0}
    KONTROL(ort.katsayi_sayisi() == 1);
    YAKIN(ort.isle(4.2f), 4.2, 1e-6);

    float iki[2] = {0.5f, 0.5f};
    FIRFiltre g(iki, 2);
    YAKIN(g.isle(2.0f), 1.0, 1e-6);      // 0.5*2 + 0.5*0
    YAKIN(g.isle(2.0f), 2.0, 1e-6);
}

static void test_fir_tasarim()
{
    BOLUM("fir_alcak_geciren_tasarla");

    float k[31];
    KONTROL(fir_alcak_geciren_tasarla(k, 31, 50.0f, 1000.0f));

    double toplam = 0.0;
    for (int i = 0; i < 31; i++) toplam += k[i];
    YAKIN(toplam, 1.0, 1e-5);            // DC kazancı 1

    bool simetrik = true;
    for (int i = 0; i < 31; i++) {
        if (std::fabs(k[i] - k[30 - i]) > 1e-6f) simetrik = false;
    }
    KONTROL(simetrik);
    KONTROL(k[15] > k[14] && k[15] > k[16]);   // tepe ortada

    KONTROL(!fir_alcak_geciren_tasarla(nullptr, 31, 50.0f, 1000.0f));
    KONTROL(!fir_alcak_geciren_tasarla(k, 0, 50.0f, 1000.0f));
    KONTROL(!fir_alcak_geciren_tasarla(k, 31, 600.0f, 1000.0f));   // Nyquist üstü
    KONTROL(!fir_alcak_geciren_tasarla(k, 31, 50.0f, 0.0f));

    // Tasarlanan filtre yüksek frekansı gerçekten bastırıyor mu?
    FIRFiltre f(k, 31);
    SinyalUreteci u(1000.0f);
    float yuksek[500], cikis[500];
    u.sinus(yuksek, 500, 300.0f, 1.0f);
    f.isle_dizi(yuksek, cikis, 500);
    float sifir[500] = {};
    KONTROL(rms_fark(sifir, cikis, 500, 100) < 0.05f);   // 300 Hz neredeyse yok olmalı
}

static void test_iir()
{
    BOLUM("IIRAlcakGeciren");

    IIRAlcakGeciren f(0.5f);
    YAKIN(f.isle(1.0f), 0.5, 1e-6);
    YAKIN(f.isle(1.0f), 0.75, 1e-6);
    YAKIN(f.isle(1.0f), 0.875, 1e-6);
    KONTROL(f.gecikme() == 1);

    f.sifirla();
    YAKIN(f.isle(1.0f), 0.5, 1e-6);

    IIRAlcakGeciren buyuk(5.0f);         // 1'e sıkışmalı
    YAKIN(buyuk.alfa(), 1.0, 1e-6);
    YAKIN(buyuk.isle(3.0f), 3.0, 1e-6);  // alfa = 1: filtre yok
    IIRAlcakGeciren negatif(-1.0f);
    KONTROL(negatif.alfa() > 0.0f);

    float a = IIRAlcakGeciren::alfa_hesapla(10.0f, 1000.0f);
    YAKIN(a, 0.0609, 1e-3);
    KONTROL(IIRAlcakGeciren::alfa_hesapla(10.0f, 0.0f) == 1.0f);

    // Uzun süre sabit giriş verince çıkış girişe oturmalı
    IIRAlcakGeciren g(a);
    float y = 0.0f;
    for (int i = 0; i < 2000; i++) y = g.isle(2.0f);
    YAKIN(y, 2.0, 1e-4);
}

static void test_zincir()
{
    BOLUM("FiltreZinciri");

    float iki[1] = {2.0f};
    FIRFiltre a(iki, 1);                 // her örneği 2 ile çarpar
    FIRFiltre b(iki, 1);
    FiltreZinciri z;
    KONTROL(z.ekle(&a));
    KONTROL(z.ekle(&b));
    KONTROL(!z.ekle(nullptr));
    KONTROL(!z.ekle(&z));                // kendisini ekleyemez
    KONTROL(z.adet() == 2);
    YAKIN(z.isle(1.5f), 6.0, 1e-6);

    FiltreZinciri bos;
    YAKIN(bos.isle(3.0f), 3.0, 1e-6);    // boş zincir girişi aynen geçirir

    // Gecikmeler toplanır
    HareketliOrtalama ho(5);             // 2
    MedyanFiltre md(7);                  // 3
    FiltreZinciri z2;
    z2.ekle(&ho);
    z2.ekle(&md);
    KONTROL(z2.gecikme() == 5);

    // Zincir de bir Filtre olduğu için başka bir zincire eklenebilir
    FiltreZinciri dis;
    KONTROL(dis.ekle(&z));
    YAKIN(dis.isle(1.0f), 4.0, 1e-6);

    // Dolu zincir
    FiltreZinciri dolu;
    for (size_t i = 0; i < FiltreZinciri::MAKS; i++) KONTROL(dolu.ekle(&a));
    KONTROL(!dolu.ekle(&a));

    // sifirla bütün halkaları sıfırlamalı
    HareketliOrtalama h1(3);
    FiltreZinciri z3;
    z3.ekle(&h1);
    z3.isle(9.0f);
    z3.sifirla();
    YAKIN(z3.isle(1.0f), 1.0, 1e-6);
}

static void test_cok_bicimlilik()
{
    BOLUM("Cok bicimlilik");

    float b[1] = {1.0f};
    HareketliOrtalama ho(3);
    MedyanFiltre md(3);
    FIRFiltre fir(b, 1);
    IIRAlcakGeciren iir(0.5f);
    FiltreZinciri z;

    Filtre* hepsi[] = {&ho, &md, &fir, &iir, &z};
    const char* adlar[] = {"Hareketli ortalama", "Medyan", "FIR", "IIR alcak geciren", "Zincir"};
    for (int i = 0; i < 5; i++) {
        KONTROL(std::strcmp(hepsi[i]->ad(), adlar[i]) == 0);
    }

    // isle_dizi, isle'yi tek tek çağırmakla aynı sonucu vermeli
    float giris[6] = {1, 5, 2, 8, 3, 3};
    float dizi_cikis[6];
    float tek_cikis[6];
    for (Filtre* f : hepsi) {
        f->sifirla();
        f->isle_dizi(giris, dizi_cikis, 6);
        f->sifirla();
        for (int i = 0; i < 6; i++) tek_cikis[i] = f->isle(giris[i]);
        KONTROL(std::memcmp(dizi_cikis, tek_cikis, sizeof(tek_cikis)) == 0);
    }

    // Yerinde filtreleme (giris == cikis)
    float yerinde[6] = {1, 5, 2, 8, 3, 3};
    ho.sifirla();
    ho.isle_dizi(yerinde, yerinde, 6);
    YAKIN(yerinde[2], (1 + 5 + 2) / 3.0, 1e-6);
    YAKIN(yerinde[5], (8 + 3 + 3) / 3.0, 1e-6);

    ho.isle_dizi(nullptr, dizi_cikis, 6);   // çökmemeli

    // Heap'teki nesneyi taban pointer'ı ile silmek (sanal yıkıcı sayesinde güvenli)
    Filtre* p = new FIRFiltre(b, 1);
    YAKIN(p->isle(2.0f), 2.0, 1e-6);
    delete p;
    KONTROL(true);
}

static void test_sinyal()
{
    BOLUM("SinyalUreteci ve rms_fark");

    SinyalUreteci u(100.0f);
    float s[100];
    u.sinus(s, 100, 25.0f, 2.0f, 1.0f);   // çeyrek periyotta tepe
    YAKIN(s[0], 1.0, 1e-5);
    YAKIN(s[1], 3.0, 1e-5);
    YAKIN(s[2], 1.0, 1e-4);
    YAKIN(s[3], -1.0, 1e-4);

    // Aynı tohum, aynı gürültü
    SinyalUreteci u1(100.0f, 7), u2(100.0f, 7);
    float a[50] = {}, b[50] = {};
    u1.gurultu_ekle(a, 50, 1.0f);
    u2.gurultu_ekle(b, 50, 1.0f);
    KONTROL(std::memcmp(a, b, sizeof(a)) == 0);

    float x[4] = {1, 2, 3, 4};
    float y[4] = {1, 2, 3, 6};
    YAKIN(rms_fark(x, y, 4), 1.0, 1e-6);          // sqrt(4 / 4)
    YAKIN(rms_fark(x, y, 4, 3), 2.0, 1e-6);       // sadece son örnek
    KONTROL(rms_fark(nullptr, y, 4) == 0.0f);
    KONTROL(rms_fark(x, y, 4, 4) == 0.0f);

    float kayik[4] = {0, 1, 2, 3};                // x'in bir örnek gecikmiş hâli
    YAKIN(rms_fark_gecikmeli(x, kayik, 4, 1), 0.0, 1e-6);
    KONTROL(rms_fark_gecikmeli(x, kayik, 4, 4) == 0.0f);

    // Dürtü olasılığı 1 ise her örnek tam genlik kadar oynamalı
    float d[20] = {};
    u.durtu_ekle(d, 20, 1.0f, 3.0f);
    bool hepsi = true;
    for (float v : d) {
        if (std::fabs(v) != 3.0f) hepsi = false;
    }
    KONTROL(hepsi);
}

int main()
{
    test_ring_buffer();
    test_hareketli_ortalama();
    test_medyan();
    test_fir();
    test_fir_tasarim();
    test_iir();
    test_zincir();
    test_cok_bicimlilik();
    test_sinyal();
    return sonuc_bildir();
}
