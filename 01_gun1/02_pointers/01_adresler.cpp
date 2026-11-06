// 01_adresler.cpp
// Her değişkenin bir adresi vardır

#include <iostream>

int main()
{
    int    sayi  = 42;
    double volt  = 3.3;
    char   harf  = 'Y';

    std::cout << "sayi  degeri: " << sayi << "   adresi: " << &sayi << "\n";
    std::cout << "volt  degeri: " << volt << "  adresi: " << &volt << "\n";
    std::cout << "harf  degeri: " << harf << "    adresi: " << static_cast<void*>(&harf) << "\n\n";

    // Pointer: adres tutan değişken
    int* p = &sayi;     // p, sayi'nin adresini tutuyor
    std::cout << "p           = " << p   << "   (sayi'nin adresi)\n";
    std::cout << "*p          = " << *p  << "   (o adresteki deger)\n";
    std::cout << "&p          = " << &p  << "   (pointer'in kendi adresi!)\n";
    std::cout << "sizeof(p)   = " << sizeof(p) << " bayt\n\n";

    // Pointer üzerinden yazmak, orijinali değiştirir
    *p = 100;
    std::cout << "*p = 100 yaptik, sayi = " << sayi << "\n\n";

    // nullptr: hiçbir yeri göstermeyen pointer
    int* bos = nullptr;
    if (bos == nullptr) {
        std::cout << "bos pointer'i dereference ETMIYORUZ, yoksa program coker\n";
    }

    return 0;
}
