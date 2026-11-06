// 05_girdi_cikti.cpp
// std::cout, std::cin, std::getline, std::endl ile "\n" farkı
//
// Derleme: g++ -std=c++17 -Wall -Wextra 05_girdi_cikti.cpp -o girdi_cikti

#include <iostream>
#include <string>

int main()
{
    // << ile istediğimiz kadar parçayı art arda basabiliriz
    int kanal = 3;
    double volt = 1.65;
    std::cout << "Kanal " << kanal << " : " << volt << " V\n";

    // Ters bölü n alt satıra geçer, ters bölü t bir sekme boşluğu bırakır
    std::cout << "kanal\tvolt\n";
    std::cout << kanal << "\t" << volt << "\n";

    // std::endl alt satıra geçer ve tamponu boşaltır (flush), "\n" sadece alt satıra geçer
    std::cout << "Bu satir endl ile bitti" << std::endl;
    std::cout << "Bu satir ters bolu n ile bitti\n\n";

    // >> klavyeden okur ve boşlukta durur
    std::string isim;
    std::cout << "Adin ne? ";
    std::cin >> isim;
    std::cout << "Hos geldin " << isim << "\n";

    // Art arda birden fazla değer okunabilir, aralarına boşluk ya da Enter koyun
    int yas = 0;
    double boy = 0.0;
    std::cout << "Yas ve boy (ornek: 20 1.75): ";
    std::cin >> yas >> boy;

    // Sayı beklerken harf girilirse okuma başarısız olur, bunu kontrol edebiliriz
    if (!std::cin) {
        std::cout << "Sayi girmen gerekiyordu.\n";
        return 1;
    }
    std::cout << yas << " yasinda, boyu " << boy << " m\n";

    // Boşluk içeren bir satırın tamamını okumak için std::getline
    // >> işleminden kalan Enter karakterini önce atlamamız gerekir, yoksa getline boş döner
    std::cin.ignore(1000, '\n');
    std::string tam_ad;
    std::cout << "Ad ve soyad: ";
    std::getline(std::cin, tam_ad);
    std::cout << "Kayit: " << tam_ad << "\n";

    return 0;
}
