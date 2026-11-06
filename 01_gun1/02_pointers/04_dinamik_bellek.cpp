// 04_dinamik_bellek.cpp
// Stack ve heap, new ve delete

#include <iostream>

double* sinyal_olustur(int n)
{
    // double tampon[n];   // stack'te yerel dizi: fonksiyon bitince yok olur, adresini döndüremeyiz
    double* tampon = new double[n];   // heap: biz silene kadar yaşar
    for (int i = 0; i < n; i++) {
        tampon[i] = i * 0.5;
    }
    return tampon;
}

int main()
{
    int n;
    std::cout << "Kac ornek? ";
    std::cin >> n;

    double* sinyal = sinyal_olustur(n);
    for (int i = 0; i < n; i++) {
        std::cout << sinyal[i] << " ";
    }
    std::cout << "\n";

    delete[] sinyal;     // new[] ile alınan, delete[] ile geri verilir
    sinyal = nullptr;    // sarkan (dangling) pointer bırakmayalım

    // BELLEK SIZINTISI örneği (yapmayın!):
    // for (int i = 0; i < 1000000; i++) {
    //     int* p = new int[1000];
    //     // delete[] p unutuldu, her turda 4 KB kayboluyor
    // }

    return 0;
}
