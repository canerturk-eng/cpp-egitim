// hamming.hpp
// ARAYÜZ DOSYASI: Bu dosyayı DEĞİŞTİRMEYİN.
#pragma once
#include <cstdint>

// BİT DÜZENİ (README'deki tabloya bakın)
//   veri4 : bit0 = d1, bit1 = d2, bit2 = d3, bit3 = d4   (üst 4 bit yok sayılır)
//   kod7  : bit(k − 1) Hamming konumu k'yı tutar, k = 1 .. 7
//           konum: 1   2   3   4   5   6   7
//           bit  : p1  p2  d1  p3  d2  d3  d4
//           (bit7 kullanılmaz, kodlarken 0 olmalı, çözerken yok sayılır)

// 4 bitlik veriyi 7 bitlik Hamming kod sözcüğüne çevirir.
uint8_t hamming_kodla(uint8_t veri4);

// 7 bitlik kod sözcüğünü çözer, tek bitlik hatayı düzeltir ve 4 bitlik veriyi döner.
// hata_konumu nullptr değilse içine sendromu yazar: 0 hata yok, 1 ile 7 arası hatalı konum.
uint8_t hamming_coz(uint8_t kod7, unsigned* hata_konumu);

// İki bayt arasında kaç bitin farklı olduğu (Hamming mesafesi).
unsigned hamming_mesafesi(uint8_t a, uint8_t b);
