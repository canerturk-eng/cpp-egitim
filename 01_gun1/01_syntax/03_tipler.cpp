// 03_tipler.cpp
// Temel tipler, std::string, tip dönüşümleri, sabit genişlikli tipler, taşma
//
// Derleme: g++ -std=c++17 -Wall -Wextra 03_tipler.cpp -o tipler

#include <iostream>
#include <string>    // std::string için
#include <cstdint>   // uint8_t, int16_t, uint32_t ...

int main()
{
    // Temel tipler ve bellekte kapladıkları yer
    bool   led_acik = true;
    char   harf     = 'Y';     // tek karakter, tek tırnak
    int    adet     = 42;
    float  gerilim  = 3.3f;    // sondaki f bunun float olduğunu söyler
    double frekans  = 1000.5;

    std::cout << "bool   : " << led_acik << "   " << sizeof(bool)   << " bayt\n";
    std::cout << "char   : " << harf     << "   " << sizeof(char)   << " bayt\n";
    std::cout << "int    : " << adet     << "   " << sizeof(int)    << " bayt\n";
    std::cout << "float  : " << gerilim  << "   " << sizeof(float)  << " bayt\n";
    std::cout << "double : " << frekans  << "   " << sizeof(double) << " bayt\n\n";

    // std::string: metin tutan tip, çift tırnak
    std::string kulup = "YONGA";
    std::string selam = "Merhaba " + kulup;     // + metinleri birleştirir
    std::cout << selam << "\n";
    std::cout << "Uzunluk: " << selam.size() << ", ilk harf: " << selam[0] << "\n\n";

    // Tip dönüşümleri
    double olcum = 3.9;
    int kesilmis = static_cast<int>(olcum);     // virgülden sonrası atılır, yuvarlanmaz
    std::cout << "3.9 int'e donunce: " << kesilmis << "\n";
    std::cout << "'A' harfinin sayi karsiligi: " << static_cast<int>('A') << "\n";

    // Tam sayı bölmesi tuzağı
    int a = 7, b = 2;
    std::cout << "7 / 2 (int)    = " << a / b << "\n";
    std::cout << "7 / 2 (double) = " << static_cast<double>(a) / b << "\n";

    // Metin ile sayı arasında dönüşüm
    int metinden = std::stoi("42");
    std::string sayidan = std::to_string(1024);
    std::cout << "stoi: " << metinden + 1 << ", to_string: " << sayidan + " ornek" << "\n\n";

    // Gömülü dünyada boyutun kesin olması gerekir.
    // 12 bitlik bir ADC çıktısı uint16_t içine sığar, 32 bitlik bir register uint32_t'dir.
    uint8_t  adc8  = 255;
    int16_t  ses   = -32768;
    uint32_t reg   = 0xDEADBEEF;

    std::cout << "uint8_t  max : " << +adc8 << "\n";   // + koymazsak karakter olarak basar!
    std::cout << "int16_t  min : " << ses   << "\n";
    std::cout << "uint32_t reg : 0x" << std::hex << reg << std::dec << "\n\n";

    // Taşma (overflow) örneği: işaretsiz tipler başa sarar
    uint8_t sayac = 250;
    for (int i = 0; i < 10; i++) {
        sayac++;
    }
    std::cout << "250 + 10 (uint8_t) = " << +sayac << "   (neden 4?)\n\n";

    // const ve auto
    const double PI = 3.14159265358979;
    auto ornekleme = 1e3;        // derleyici double olduğunu kendisi anlar
    auto ornek_sayisi = 1024;    // int
    std::cout << "PI = " << PI << ", fs = " << ornekleme
              << " Hz, N = " << ornek_sayisi << "\n";

    return 0;
}
