// register.hpp
// ARAYÜZ DOSYASI: Bu dosyayı DEĞİŞTİRMEYİN. Testler bu imzalara göre yazıldı.
#pragma once
#include <cstdint>

// n. biti 1 yapar. n > 31 ya da reg == nullptr ise hiçbir şey yapmaz.
void bit_set(volatile uint32_t* reg, unsigned n);

// n. biti 0 yapar. n > 31 ya da reg == nullptr ise hiçbir şey yapmaz.
void bit_clear(volatile uint32_t* reg, unsigned n);

// n. biti tersler. n > 31 ya da reg == nullptr ise hiçbir şey yapmaz.
void bit_toggle(volatile uint32_t* reg, unsigned n);

// n. biti okur. n > 31 ya da reg == nullptr ise false döner.
bool bit_read(const volatile uint32_t* reg, unsigned n);

// [konum, konum + genislik) aralığındaki bit alanına deger yazar, diğer bitlere dokunmaz.
// Geçersiz durumda (reg nullptr, genislik 0, konum + genislik > 32,
// deger genislik bite sığmıyor) register'ı DEĞİŞTİRMEDEN false döner.
bool alan_yaz(volatile uint32_t* reg, unsigned konum, unsigned genislik, uint32_t deger);

// [konum, konum + genislik) aralığındaki bit alanını okur, sağa yaslanmış olarak döner.
// Geçersiz durumda 0 döner.
uint32_t alan_oku(const volatile uint32_t* reg, unsigned konum, unsigned genislik);

// Değerdeki 1 olan bitlerin sayısını döner (popcount).
unsigned bit_say(uint32_t deger);
