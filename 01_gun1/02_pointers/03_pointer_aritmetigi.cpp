// 03_pointer_aritmetigi.cpp
// Diziler bellekte yan yana durur, pointer ile üzerlerinde gezebiliriz

#include <iostream>
#include <cstdint>

int main()
{
    int16_t ornekler[5] = {100, 200, 300, 400, 500};

    std::cout << "Elemanlarin adresleri (int16_t = 2 bayt):\n";
    for (int i = 0; i < 5; i++) {
        std::cout << "  ornekler[" << i << "] @ " << &ornekler[i]
                  << " = " << ornekler[i] << "\n";
    }

    // Dizinin adı, ilk elemanın adresine dönüşür
    int16_t* p = ornekler;
    std::cout << "\np       = " << p << "\n";
    std::cout << "p + 1   = " << p + 1 << "   (1 degil 2 bayt ilerledi!)\n";

    // arr[i] ile *(arr + i) birebir aynı şeydir
    std::cout << "ornekler[3]      = " << ornekler[3] << "\n";
    std::cout << "*(ornekler + 3)  = " << *(ornekler + 3) << "\n\n";

    // Pointer ile dizi üzerinde gezinmek
    int toplam = 0;
    for (int16_t* q = ornekler; q != ornekler + 5; q++) {
        toplam += *q;
    }
    std::cout << "Toplam = " << toplam << "\n";

    // Diziyi fonksiyona verince boyutu kaybolur, o yüzden n'i ayrıca veririz
    std::cout << "sizeof(ornekler) = " << sizeof(ornekler) << " bayt\n";
    std::cout << "sizeof(p)        = " << sizeof(p) << " bayt\n";

    return 0;
}
