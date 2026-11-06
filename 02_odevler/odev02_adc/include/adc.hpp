// adc.hpp
// ARAYÜZ DOSYASI: Bu dosyayı DEĞİŞTİRMEYİN.
#pragma once
#include <cstdint>
#include <cstddef>

// Bir LSB'nin volt karşılığı: vref / 2^bit
// bit 1 ile 16 arasında değilse ya da vref <= 0 ise 0.0 döner.
double lsb_hesapla(double vref, unsigned bit);

// Analog voltajı dijital koda çevirir: kod = floor(volt / LSB)
// Sonuç [0, 2^bit − 1] aralığına sıkıştırılır (clamp).
// bit 1 ile 16 arasında değilse ya da vref <= 0 ise 0 döner.
uint16_t adc_kodla(double volt, double vref, unsigned bit);

// Dijital kodu tekrar voltaja çevirir. Basamağın ORTASINI döndürür: (kod + 0.5) * LSB
// Geçersiz bit ya da vref için 0.0 döner.
double adc_coz(uint16_t kod, double vref, unsigned bit);

// tampon[i] = ofset + genlik * sin(2π * frekans * i / fs)   (i = 0 .. n−1)
// tampon nullptr ya da fs <= 0 ise hiçbir şey yapmaz.
void sinus_uret(double* tampon, size_t n, double frekans, double fs,
                double genlik, double ofset);

// İki dizi arasındaki RMS fark: sqrt( (1/n) * Σ (a[i] − b[i])² )
// n == 0 ya da nullptr gelirse 0.0 döner.
double rms_hata(const double* a, const double* b, size_t n);

// Sinyal / kuantizasyon gürültüsü oranı (dB):
// 10 * log10( Σ (x[i] − ort(x))²  /  Σ (x[i] − xq[i])² )
// Gürültü gücü 0 ise ya da girdi geçersizse 0.0 döner.
double sqnr_db(const double* orijinal, const double* kuantize, size_t n);
