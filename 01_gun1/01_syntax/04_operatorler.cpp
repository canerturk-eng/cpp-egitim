// 04_operatorler.cpp
// Aritmetik, karşılaştırma, mantıksal, artırma ve azaltma, bileşik atama, öncelik
//
// Derleme: g++ -std=c++17 -Wall -Wextra 04_operatorler.cpp -o operatorler

#include <iostream>

int main()
{
    // Aritmetik
    int a = 17, b = 5;
    std::cout << "a + b = " << a + b << "\n";
    std::cout << "a - b = " << a - b << "\n";
    std::cout << "a * b = " << a * b << "\n";
    std::cout << "a / b = " << a / b << "   (tam sayi bolmesi)\n";
    std::cout << "a % b = " << a % b << "   (bolumden kalan)\n\n";

    // Karşılaştırma: sonuç her zaman bool
    std::cout << "a > b  : " << (a > b)  << "\n";
    std::cout << "a <= b : " << (a <= b) << "\n";
    std::cout << "a == b : " << (a == b) << "\n";
    std::cout << "a != b : " << (a != b) << "\n\n";

    // Mantıksal: VE, VEYA, DEĞİL
    double volt = 3.1;
    bool aralikta = (volt >= 3.0) && (volt <= 3.6);    // ikisi de doğruysa doğru
    bool uyari    = (volt < 3.0) || (volt > 3.6);      // biri doğruysa doğru
    std::cout << "aralikta: " << aralikta << ", uyari: " << uyari
              << ", !aralikta: " << !aralikta << "\n\n";

    // i++ ile ++i farkı: ikisi de artırır, fark ifadenin verdiği değerdedir
    int i = 5;
    int x = i++;      // x eski değeri alır, sonra i artar
    std::cout << "x = i++  sonrasi: x = " << x << ", i = " << i << "\n";

    int j = 5;
    int y = ++j;      // önce j artar, y yeni değeri alır
    std::cout << "y = ++j  sonrasi: y = " << y << ", j = " << j << "\n";

    int k = 5;
    k--;              // azaltma da aynı mantıkla çalışır
    std::cout << "k-- sonrasi: k = " << k << "\n\n";

    // Bileşik atama
    int toplam = 10;
    toplam += 5;      // toplam = toplam + 5
    std::cout << "+= 5 : " << toplam << "\n";
    toplam -= 3;
    std::cout << "-= 3 : " << toplam << "\n";
    toplam *= 2;
    std::cout << "*= 2 : " << toplam << "\n";
    toplam /= 4;
    std::cout << "/= 4 : " << toplam << "\n";
    toplam %= 4;
    std::cout << "%= 4 : " << toplam << "\n\n";

    // Öncelik: çarpma ve bölme toplamadan evvel yapılır
    std::cout << "2 + 3 * 4   = " << 2 + 3 * 4   << "\n";
    std::cout << "(2 + 3) * 4 = " << (2 + 3) * 4 << "\n";
    std::cout << "10 - 4 - 3  = " << 10 - 4 - 3  << "   (soldan saga)\n";

    return 0;
}
