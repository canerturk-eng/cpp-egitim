// 02_swap_uc_yol.cpp
// Aynı işi üç farklı şekilde yazalım ve hangisinin çalıştığını görelim

#include <iostream>

void swap_deger(int a, int b)       // KOPYALAR değişir, işe yaramaz
{
    int gecici = a;
    a = b;
    b = gecici;
}

void swap_pointer(int* a, int* b)   // C usulü: adresleri al, adresteki değeri değiştir
{
    int gecici = *a;
    *a = *b;
    *b = gecici;
}

void swap_referans(int& a, int& b)  // C++ usulü: aynı işin daha temiz hali
{
    int gecici = a;
    a = b;
    b = gecici;
}

int main()
{
    int x = 1, y = 2;

    swap_deger(x, y);
    std::cout << "swap_deger    sonrasi: x=" << x << " y=" << y << "\n";

    swap_pointer(&x, &y);
    std::cout << "swap_pointer  sonrasi: x=" << x << " y=" << y << "\n";

    swap_referans(x, y);
    std::cout << "swap_referans sonrasi: x=" << x << " y=" << y << "\n";

    return 0;
}
