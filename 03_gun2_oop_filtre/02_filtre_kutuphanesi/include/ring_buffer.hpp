// ring_buffer.hpp
// Ödev 03'teki struct ve rb_ fonksiyonlarının sınıf hâli.
// Farkı: kapasite derleme anında değil, nesne oluşturulurken belirleniyor.
// Bellek yapıcıda alınıyor, yıkıcıda geri veriliyor (RAII).
#pragma once
#include <cstddef>

class RingBuffer {
public:
    // kapasite 0 verilirse 1 kabul edilir (% 0 ile bölme hatasına düşmemek için).
    explicit RingBuffer(size_t kapasite);
    ~RingBuffer();

    // Kopyalama yasak: iki nesne aynı belleği silmesin (04_raii.cpp).
    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;

    // Sona ekler. Doluysa en eski örneğin üzerine yazar (filtrelerde hep bu lazım).
    void ekle(float deger);

    // gecmis = 0 en yeni, 1 bir önceki, ...
    // gecmis >= adet() ise 0.0f döner. Bu bilerek böyle: FIR filtre başlangıçta
    // "henüz gelmemiş" örnekleri sıfır kabul eder, bu davranış onun işini kolaylaştırıyor.
    float bak(size_t gecmis) const;

    void temizle();

    size_t adet() const     { return adet_; }
    size_t kapasite() const { return kapasite_; }
    bool   bos() const      { return adet_ == 0; }
    bool   dolu() const     { return adet_ == kapasite_; }

private:
    // DİKKAT: Üyeler ilk değer listesinde BURADAKİ sırayla kurulur.
    // veri_ kapasite_'yi kullandığı için kapasite_ önce gelmeli.
    size_t kapasite_;
    float* veri_;
    size_t bas_;     // en eski örneğin indeksi
    size_t adet_;
};
