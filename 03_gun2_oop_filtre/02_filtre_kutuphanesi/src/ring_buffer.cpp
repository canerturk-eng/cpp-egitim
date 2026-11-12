// ring_buffer.cpp
#include "ring_buffer.hpp"

RingBuffer::RingBuffer(size_t kapasite)
    : kapasite_(kapasite == 0 ? 1 : kapasite),
      veri_(new float[kapasite_]),    // kapasite_ yukarıda kuruldu, kullanabiliriz
      bas_(0),
      adet_(0)
{
    for (size_t i = 0; i < kapasite_; i++) {
        veri_[i] = 0.0f;
    }
}

RingBuffer::~RingBuffer()
{
    delete[] veri_;
}

void RingBuffer::ekle(float deger)
{
    if (dolu()) {
        veri_[bas_] = deger;                  // en eskinin yerine yaz
        bas_ = (bas_ + 1) % kapasite_;        // en eski artık bir sonraki
    } else {
        veri_[(bas_ + adet_) % kapasite_] = deger;
        adet_++;
    }
}

float RingBuffer::bak(size_t gecmis) const
{
    if (gecmis >= adet_) {
        return 0.0f;
    }
    // En yeni örnek bas_ + adet_ - 1 konumunda, oradan gecmis kadar geri gidiyoruz.
    return veri_[(bas_ + adet_ - 1 - gecmis) % kapasite_];
}

void RingBuffer::temizle()
{
    bas_ = 0;
    adet_ = 0;
}
