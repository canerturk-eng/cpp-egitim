// test_adc.cpp
// Bu dosyayı DEĞİŞTİRMEYİN.
#include "adc.hpp"
#include "mini_test.hpp"

constexpr size_t N = 4096;
static double sinyal[N];
static double geri[N];

static double ornek_sqnr(unsigned bit)
{
    const double vref = 3.3;
    // Tam ölçek sinüs (kırpılmasın diye genliği çok az kısıyoruz)
    sinus_uret(sinyal, N, 37.0, 4096.0, vref / 2 * 0.999, vref / 2);
    for (size_t i = 0; i < N; i++) {
        geri[i] = adc_coz(adc_kodla(sinyal[i], vref, bit), vref, bit);
    }
    return sqnr_db(sinyal, geri, N);
}

int main()
{
    BOLUM("lsb_hesapla");
    YAKIN(lsb_hesapla(3.3, 12), 3.3 / 4096, 1e-12);
    YAKIN(lsb_hesapla(5.0, 8), 5.0 / 256, 1e-12);
    KONTROL(lsb_hesapla(3.3, 0) == 0.0);
    KONTROL(lsb_hesapla(3.3, 17) == 0.0);
    KONTROL(lsb_hesapla(-1.0, 8) == 0.0);

    BOLUM("adc_kodla");
    KONTROL(adc_kodla(0.0, 3.3, 12) == 0);
    KONTROL(adc_kodla(2.0, 4.0, 12) == 2048);      // yarı ölçek, tam orta kod
    KONTROL(adc_kodla(1.0, 3.3, 12) == 1241);      // floor(1.0 / 0.000805664) = 1241
    KONTROL(adc_kodla(3.3, 3.3, 12) == 4095);      // tam vref, en büyük koda sıkışmalı
    KONTROL(adc_kodla(5.0, 3.3, 12) == 4095);      // aşırı voltaj
    KONTROL(adc_kodla(-0.7, 3.3, 12) == 0);        // negatif voltaj
    KONTROL(adc_kodla(2.5, 5.0, 1) == 1);
    KONTROL(adc_kodla(1.0, 3.3, 16) == 19859);
    KONTROL(adc_kodla(1.0, 3.3, 0) == 0);
    KONTROL(adc_kodla(1.0, 3.3, 20) == 0);

    BOLUM("adc_coz");
    YAKIN(adc_coz(0, 3.3, 12), 0.5 * 3.3 / 4096, 1e-12);
    YAKIN(adc_coz(2048, 3.3, 12), 2048.5 * 3.3 / 4096, 1e-12);
    KONTROL(adc_coz(10, 3.3, 0) == 0.0);

    BOLUM("Kodla ve coz: hata yarim LSB'yi gecmemeli");
    {
        double en_kotu = 0.0;
        for (int i = 0; i <= 1000; i++) {
            double v = 3.3 * i / 1000.0 * 0.9999;
            double h = std::fabs(adc_coz(adc_kodla(v, 3.3, 10), 3.3, 10) - v);
            if (h > en_kotu) en_kotu = h;
        }
        KONTROL(en_kotu <= lsb_hesapla(3.3, 10) / 2 + 1e-12);
    }

    BOLUM("sinus_uret");
    {
        double t[8];
        sinus_uret(t, 8, 1.0, 8.0, 2.0, 1.0);       // fs = 8 f, çeyrek periyotta tepe
        YAKIN(t[0], 1.0, 1e-9);
        YAKIN(t[2], 3.0, 1e-9);
        YAKIN(t[4], 1.0, 1e-9);
        YAKIN(t[6], -1.0, 1e-9);
        sinus_uret(nullptr, 8, 1.0, 8.0, 1.0, 0.0); // çökmemeli
    }

    BOLUM("rms_hata");
    {
        double a[4] = {1, 2, 3, 4};
        double b[4] = {1, 2, 3, 6};
        YAKIN(rms_hata(a, b, 4), 1.0, 1e-12);         // sqrt(4 / 4)
        YAKIN(rms_hata(a, a, 4), 0.0, 1e-12);
        KONTROL(rms_hata(a, b, 0) == 0.0);
        KONTROL(rms_hata(nullptr, b, 4) == 0.0);
    }

    BOLUM("SQNR: her bit yaklasik 6 dB kazandirmali (6.02 N + 1.76)");
    {
        double s8  = ornek_sqnr(8);
        double s12 = ornek_sqnr(12);
        std::printf("  8 bit  SQNR = %.2f dB (teori %.2f)\n", s8,  6.02 * 8  + 1.76);
        std::printf("  12 bit SQNR = %.2f dB (teori %.2f)\n", s12, 6.02 * 12 + 1.76);
        YAKIN(s8,  6.02 * 8  + 1.76, 1.5);
        YAKIN(s12, 6.02 * 12 + 1.76, 1.5);
        YAKIN(s12 - s8, 4 * 6.02, 1.5);
    }

    return sonuc_bildir();
}
