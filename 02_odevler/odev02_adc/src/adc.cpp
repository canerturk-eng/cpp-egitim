// adc.cpp
// Fonksiyonları burada dolduracaksınız. (void) satırlarını kendi kodunuzu yazınca silin.
#include "adc.hpp"
#include <cmath>

double lsb_hesapla(double vref, unsigned bit)
{
    // TODO
    (void)vref; (void)bit;
    return 0.0;
}

uint16_t adc_kodla(double volt, double vref, unsigned bit)
{
    // TODO
    (void)volt; (void)vref; (void)bit;
    return 0;
}

double adc_coz(uint16_t kod, double vref, unsigned bit)
{
    // TODO
    (void)kod; (void)vref; (void)bit;
    return 0.0;
}

void sinus_uret(double* tampon, size_t n, double frekans, double fs,
                double genlik, double ofset)
{
    // TODO
    (void)tampon; (void)n; (void)frekans; (void)fs; (void)genlik; (void)ofset;
}

double rms_hata(const double* a, const double* b, size_t n)
{
    // TODO
    (void)a; (void)b; (void)n;
    return 0.0;
}

double sqnr_db(const double* orijinal, const double* kuantize, size_t n)
{
    // TODO
    (void)orijinal; (void)kuantize; (void)n;
    return 0.0;
}
