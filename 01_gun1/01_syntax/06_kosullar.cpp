// 06_kosullar.cpp
// if, else if, else, switch ve ternary operatörü
//
// Derleme: g++ -std=c++17 -Wall -Wextra 06_kosullar.cpp -o kosullar

#include <iostream>

int main()
{
    // if, else if, else: yukarıdan aşağıya bakılır, ilk doğru olan çalışır, gerisi atlanır
    double pil = 3.45;
    if (pil > 4.0) {
        std::cout << "Pil dolu\n";
    } else if (pil > 3.3) {
        std::cout << "Pil normal\n";
    } else {
        std::cout << "Pil zayif, sarja tak\n";
    }

    // switch: bir durum makinesinin (FSM) en ilkel hali
    int durum = 2;
    switch (durum) {
        case 0:  std::cout << "BEKLE\n";   break;
        case 1:  std::cout << "OLC\n";     break;
        case 2:  std::cout << "GONDER\n";  break;
        default: std::cout << "HATA\n";    break;   // hiçbir case tutmazsa buraya gelir
    }

    // Birden fazla case aynı işi yapabilir
    char komut = 'k';
    switch (komut) {
        case 'b':
        case 'B':
            std::cout << "Basla\n";
            break;
        case 'k':
        case 'K':
            std::cout << "Kapat\n";
            break;
        default:
            std::cout << "Bilinmeyen komut\n";
            break;
    }

    // Ternary: kosul ? doğruysa_bu : yanlışsa_bu
    int sayi = 7;
    std::cout << sayi << (sayi % 2 == 0 ? " cift\n" : " tek\n");

    double giris = 5.2;
    double kirpilmis = (giris > 3.3) ? 3.3 : giris;   // 3.3 V üstünü kırp
    std::cout << "Kirpilmis gerilim: " << kirpilmis << " V\n";

    return 0;
}
