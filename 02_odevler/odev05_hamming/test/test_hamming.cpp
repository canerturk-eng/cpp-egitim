// test_hamming.cpp
// Bu dosyayı DEĞİŞTİRMEYİN.
#include "hamming.hpp"
#include "mini_test.hpp"

int main()
{
    BOLUM("Bilinen kod sozcukleri");
    KONTROL(hamming_kodla(0x0) == 0x00);
    KONTROL(hamming_kodla(0xB) == 0x55);   // d1=1 d2=1 d3=0 d4=1  ->  p1=1 p2=0 p3=0
    KONTROL(hamming_kodla(0xF) == 0x7F);
    KONTROL(hamming_kodla(0x1) == 0x07);   // d1=1  ->  p1=1 p2=1 p3=0
    KONTROL(hamming_kodla(0xF0) == 0x00);  // üst bitler yok sayılmalı

    BOLUM("Kodla ve coz (hatasiz, 16 veri)");
    bool hepsi = true;
    for (unsigned v = 0; v < 16; v++) {
        uint8_t k = hamming_kodla((uint8_t)v);
        unsigned hk = 99;
        if (k & 0x80) hepsi = false;
        if (hamming_coz(k, &hk) != v || hk != 0) hepsi = false;
    }
    KONTROL(hepsi);

    BOLUM("Tek bit hatasi duzeltme (16 veri x 7 konum)");
    hepsi = true;
    for (unsigned v = 0; v < 16; v++) {
        uint8_t k = hamming_kodla((uint8_t)v);
        for (unsigned konum = 1; konum <= 7; konum++) {
            uint8_t bozuk = k ^ (uint8_t)(1u << (konum - 1));
            unsigned hk = 0;
            if (hamming_coz(bozuk, &hk) != v || hk != konum) {
                std::printf("  veri %u, konum %u: duzeltilemedi\n", v, konum);
                hepsi = false;
            }
        }
    }
    KONTROL(hepsi);

    BOLUM("Kenar durumlar");
    KONTROL(hamming_coz(hamming_kodla(0x9), nullptr) == 0x9);   // nullptr çökmemeli
    KONTROL(hamming_coz(hamming_kodla(0x6) | 0x80, nullptr) == 0x6); // bit7 yok sayılmalı

    BOLUM("Minimum mesafe 3 olmali");
    unsigned en_kucuk = 99;
    for (unsigned a = 0; a < 16; a++)
        for (unsigned b = a + 1; b < 16; b++) {
            unsigned m = hamming_mesafesi(hamming_kodla((uint8_t)a), hamming_kodla((uint8_t)b));
            if (m < en_kucuk) en_kucuk = m;
        }
    KONTROL(en_kucuk == 3);
    KONTROL(hamming_mesafesi(0x00, 0xFF) == 8);
    KONTROL(hamming_mesafesi(0xA5, 0xA5) == 0);

    return sonuc_bildir();
}
