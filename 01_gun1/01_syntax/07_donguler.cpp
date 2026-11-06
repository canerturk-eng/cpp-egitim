// 07_donguler.cpp
// for, while, do while, break, continue ve iç içe döngü
//
// Derleme: g++ -std=c++17 -Wall -Wextra 07_donguler.cpp -o donguler

#include <iostream>

int main()
{
    // for: kaç tur döneceğini biliyorsak
    // README'deki iz tablosunun aynısını ekrana basıyoruz
    int toplam = 0;
    for (int i = 1; i <= 4; i++) {
        toplam += i;
        std::cout << "i = " << i << ", toplam = " << toplam << "\n";
    }
    std::cout << "for bitti, toplam = " << toplam << "\n\n";

    // while: bir koşul sağlandığı sürece, kaç tur süreceğini bilmiyorsak
    int deger = 1, adim = 0;
    while (deger < 20) {
        deger *= 2;
        adim++;
        std::cout << "adim = " << adim << ", deger = " << deger << "\n";
    }
    std::cout << adim << " adimda " << deger << " oldu\n\n";

    // do while: gövde en az bir kere çalışır, koşula sonra bakılır
    int n = 100;
    do {
        std::cout << "do while govdesi calisti, n = " << n << "\n";
        n++;
    } while (n < 10);      // koşul baştan yanlıştı, yine de bir tur döndü
    std::cout << "\n";

    // break: döngüden hemen çık
    for (int i = 1; i <= 10; i++) {
        if (i == 4) {
            break;
        }
        std::cout << "break ornegi, i = " << i << "\n";
    }
    std::cout << "\n";

    // continue: bu turun kalanını atla, sıradaki tura geç
    for (int i = 1; i <= 5; i++) {
        if (i % 2 == 0) {
            continue;
        }
        std::cout << "continue ornegi, i = " << i << "\n";
    }
    std::cout << "\n";

    // İç içe döngü: dıştaki her tur için içteki baştan sona döner
    for (int satir = 1; satir <= 2; satir++) {
        for (int sutun = 1; sutun <= 3; sutun++) {
            std::cout << satir << "x" << sutun << "=" << satir * sutun << "  ";
        }
        std::cout << "\n";
    }

    return 0;
}
