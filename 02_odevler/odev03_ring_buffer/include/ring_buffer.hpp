// ring_buffer.hpp
// ARAYÜZ DOSYASI: Bu dosyayı DEĞİŞTİRMEYİN.
#pragma once
#include <cstddef>

constexpr size_t RB_KAPASITE = 8;

// Dairesel tampon. Alanların anlamı:
//   veri : örneklerin tutulduğu sabit boyutlu dizi
//   bas  : EN ESKİ örneğin indeksi
//   adet : tamponda şu an kaç örnek olduğu
// Yeni örneğin yazılacağı yer: (bas + adet) % RB_KAPASITE
struct RingBuffer {
    float  veri[RB_KAPASITE];
    size_t bas;
    size_t adet;
};

// Tamponu boşaltır (bas = 0, adet = 0). rb nullptr ise bir şey yapmaz.
void rb_baslat(RingBuffer* rb);

// Sona ekler. Tampon doluysa EKLEMEZ ve false döner.
bool rb_ekle(RingBuffer* rb, float deger);

// Sona ekler. Tampon doluysa EN ESKİ örneğin üzerine yazar (filtrelerde bu kullanılır).
void rb_ekle_ezerek(RingBuffer* rb, float deger);

// En eski örneği *deger içine koyup tampondan çıkarır. Boşsa false döner.
bool rb_cikar(RingBuffer* rb, float* deger);

// Çıkarmadan bakar. gecmis = 0 en YENİ örnek, 1 bir önceki, ...
// gecmis >= adet ise false döner.
bool rb_bak(const RingBuffer* rb, size_t gecmis, float* deger);

size_t rb_adet(const RingBuffer* rb);   // nullptr için 0
bool   rb_bos(const RingBuffer* rb);    // nullptr için true
bool   rb_dolu(const RingBuffer* rb);   // nullptr için false
