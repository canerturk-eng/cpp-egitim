// test_ring_buffer.cpp
// Bu dosyayı DEĞİŞTİRMEYİN.
#include "ring_buffer.hpp"
#include "mini_test.hpp"

int main()
{
    RingBuffer rb;
    float x = -1.0f;

    BOLUM("Baslangic");
    rb.bas = 5; rb.adet = 3;          // çöp değerler
    rb_baslat(&rb);
    KONTROL(rb.bas == 0 && rb.adet == 0);
    KONTROL(rb_bos(&rb) && !rb_dolu(&rb) && rb_adet(&rb) == 0);
    KONTROL(rb_cikar(&rb, &x) == false);
    KONTROL(rb_bak(&rb, 0, &x) == false);

    BOLUM("FIFO sirasi");
    KONTROL(rb_ekle(&rb, 1.0f));
    KONTROL(rb_ekle(&rb, 2.0f));
    KONTROL(rb_ekle(&rb, 3.0f));
    KONTROL(rb_adet(&rb) == 3);
    KONTROL(rb_cikar(&rb, &x) && x == 1.0f);
    KONTROL(rb_cikar(&rb, &x) && x == 2.0f);
    KONTROL(rb_adet(&rb) == 1);

    BOLUM("Doluluk ve sarma (wrap around)");
    rb_baslat(&rb);
    for (int i = 0; i < (int)RB_KAPASITE; i++) KONTROL(rb_ekle(&rb, (float)i));
    KONTROL(rb_dolu(&rb));
    KONTROL(rb_ekle(&rb, 99.0f) == false);           // dolu, eklememeli
    KONTROL(rb_cikar(&rb, &x) && x == 0.0f);
    KONTROL(rb_cikar(&rb, &x) && x == 1.0f);
    KONTROL(rb_ekle(&rb, 100.0f));                   // indeks başa sarmalı
    KONTROL(rb_ekle(&rb, 101.0f));
    KONTROL(rb_dolu(&rb));
    for (int i = 2; i < (int)RB_KAPASITE; i++) KONTROL(rb_cikar(&rb, &x) && x == (float)i);
    KONTROL(rb_cikar(&rb, &x) && x == 100.0f);
    KONTROL(rb_cikar(&rb, &x) && x == 101.0f);
    KONTROL(rb_bos(&rb));
    KONTROL(rb.bas < RB_KAPASITE);                   // bas her zaman geçerli bir indeks olmalı

    BOLUM("rb_ekle_ezerek ve rb_bak");
    rb_baslat(&rb);
    for (int i = 1; i <= 20; i++) rb_ekle_ezerek(&rb, (float)i);  // 20 örnek, son 8'i kalmalı
    KONTROL(rb_dolu(&rb) && rb_adet(&rb) == RB_KAPASITE);
    KONTROL(rb_bak(&rb, 0, &x) && x == 20.0f);       // en yeni
    KONTROL(rb_bak(&rb, 1, &x) && x == 19.0f);
    KONTROL(rb_bak(&rb, 7, &x) && x == 13.0f);       // en eski
    KONTROL(rb_bak(&rb, 8, &x) == false);
    KONTROL(rb_adet(&rb) == RB_KAPASITE);            // bakmak hiçbir şeyi değiştirmemeli
    KONTROL(rb_cikar(&rb, &x) && x == 13.0f);

    BOLUM("Uzun sure calisma (indeks tasmasi olmamali)");
    rb_baslat(&rb);
    bool hep_dogru = true;
    for (int i = 0; i < 100000; i++) {
        rb_ekle_ezerek(&rb, (float)i);
        float yeni;
        if (!rb_bak(&rb, 0, &yeni) || yeni != (float)i) hep_dogru = false;
        if (rb.bas >= RB_KAPASITE) hep_dogru = false;
    }
    KONTROL(hep_dogru);

    BOLUM("nullptr guvenligi");
    rb_baslat(nullptr);
    rb_ekle_ezerek(nullptr, 1.0f);
    KONTROL(rb_ekle(nullptr, 1.0f) == false);
    KONTROL(rb_cikar(nullptr, &x) == false);
    KONTROL(rb_cikar(&rb, nullptr) == false);
    KONTROL(rb_bak(nullptr, 0, &x) == false);
    KONTROL(rb_adet(nullptr) == 0 && rb_bos(nullptr) && !rb_dolu(nullptr));

    return sonuc_bildir();
}
