// test_register.cpp
// Bu dosyayı DEĞİŞTİRMEYİN. Kendi testlerinizi eklemek isterseniz ayrı bir dosyada yapın.
#include "register.hpp"
#include "mini_test.hpp"

int main()
{
    BOLUM("bit_set / bit_clear / bit_toggle / bit_read");
    uint32_t r = 0;
    bit_set(&r, 0);   KONTROL(r == 0x00000001u);
    bit_set(&r, 31);  KONTROL(r == 0x80000001u);
    bit_set(&r, 31);  KONTROL(r == 0x80000001u);          // iki kere set etmek bir şey değiştirmemeli
    bit_clear(&r, 0); KONTROL(r == 0x80000000u);
    bit_clear(&r, 5); KONTROL(r == 0x80000000u);          // zaten 0 olan bit
    bit_toggle(&r, 4); KONTROL(r == 0x80000010u);
    bit_toggle(&r, 4); KONTROL(r == 0x80000000u);
    KONTROL(bit_read(&r, 31) == true);
    KONTROL(bit_read(&r, 30) == false);

    BOLUM("Sinir disi bit numaralari");
    r = 0x12345678u;
    bit_set(&r, 32);    KONTROL(r == 0x12345678u);
    bit_clear(&r, 40);  KONTROL(r == 0x12345678u);
    bit_toggle(&r, 99); KONTROL(r == 0x12345678u);
    KONTROL(bit_read(&r, 32) == false);

    BOLUM("nullptr guvenligi");
    bit_set(nullptr, 3); bit_clear(nullptr, 3); bit_toggle(nullptr, 3);
    KONTROL(bit_read(nullptr, 3) == false);
    KONTROL(alan_yaz(nullptr, 0, 4, 1) == false);
    KONTROL(alan_oku(nullptr, 0, 4) == 0u);

    BOLUM("alan_yaz / alan_oku");
    r = 0xFFFFFFFFu;
    KONTROL(alan_yaz(&r, 8, 4, 0x0) == true);
    KONTROL(r == 0xFFFFF0FFu);                             // sadece 8..11 sıfırlandı
    KONTROL(alan_yaz(&r, 8, 4, 0xA) == true);
    KONTROL(r == 0xFFFFFAFFu);
    KONTROL(alan_oku(&r, 8, 4) == 0xAu);
    KONTROL(alan_oku(&r, 0, 8) == 0xFFu);

    BOLUM("alan_yaz gecersiz durumlar (register degismemeli)");
    r = 0xCAFEBABEu;
    KONTROL(alan_yaz(&r, 0, 4, 16) == false);              // 16, 4 bite sığmaz
    KONTROL(alan_yaz(&r, 30, 4, 1) == false);              // 30 + 4 > 32
    KONTROL(alan_yaz(&r, 0, 0, 0) == false);               // genişlik 0
    KONTROL(r == 0xCAFEBABEu);
    KONTROL(alan_oku(&r, 30, 4) == 0u);

    BOLUM("32 bit genislik kenar durumu");
    r = 0;
    KONTROL(alan_yaz(&r, 0, 32, 0xDEADBEEFu) == true);
    KONTROL(r == 0xDEADBEEFu);
    KONTROL(alan_oku(&r, 0, 32) == 0xDEADBEEFu);
    KONTROL(alan_yaz(&r, 31, 1, 0) == true);
    KONTROL(r == 0x5EADBEEFu);

    BOLUM("Senaryo: STM32 GPIO MODER (her pin 2 bit)");
    // 00 giris, 01 cikis, 10 alternatif, 11 analog
    uint32_t moder = 0xA8000000u;                          // reset değeri
    volatile uint32_t* GPIOA_MODER = &moder;
    KONTROL(alan_yaz(GPIOA_MODER, 5 * 2, 2, 0b01) == true); // PA5 çıkış (LED)
    KONTROL(alan_yaz(GPIOA_MODER, 0 * 2, 2, 0b11) == true); // PA0 analog (ADC)
    KONTROL(moder == 0xA8000403u);
    KONTROL(alan_oku(GPIOA_MODER, 5 * 2, 2) == 0b01u);
    KONTROL(alan_oku(GPIOA_MODER, 15 * 2, 2) == 0b10u);

    BOLUM("bit_say");
    KONTROL(bit_say(0u) == 0u);
    KONTROL(bit_say(0xFFFFFFFFu) == 32u);
    KONTROL(bit_say(0x80000001u) == 2u);
    KONTROL(bit_say(0xF0F0F0F0u) == 16u);

    return sonuc_bildir();
}
