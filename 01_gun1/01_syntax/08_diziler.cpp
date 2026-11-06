// 08_diziler.cpp
// Dizi tanımlama, indeksleme, dizi üzerinde gezinme, sınır dışına çıkma tehlikesi
//
// Derleme: g++ -std=c++17 -Wall -Wextra 08_diziler.cpp -o diziler

#include <iostream>

int main()
{
    // Beş tane int yan yana. İndeksler 0, 1, 2, 3, 4
    int olcumler[5] = {12, 15, 11, 19, 14};

    std::cout << "Ilk eleman : " << olcumler[0] << "\n";
    std::cout << "Son eleman : " << olcumler[4] << "\n";

    // İndeksle okuduğumuz gibi yazabiliriz de
    olcumler[2] = 21;
    std::cout << "Yeni olcumler[2] : " << olcumler[2] << "\n\n";

    // Eksik verilen elemanlar 0 olur
    int sifirli[4] = {7};
    std::cout << "sifirli[0] = " << sifirli[0] << ", sifirli[3] = " << sifirli[3] << "\n\n";

    // Dizi üzerinde for ile gezmek. Koşulun i < 5 olmasına dikkat, i <= 5 değil!
    for (int i = 0; i < 5; i++) {
        std::cout << "olcumler[" << i << "] = " << olcumler[i] << "\n";
    }

    // Eleman sayısını elle yazmak yerine hesaplatabiliriz
    int n = sizeof(olcumler) / sizeof(olcumler[0]);
    std::cout << "Eleman sayisi: " << n << "\n";

    // Range based for: her elemanı sırayla x içine kopyalar
    int en_buyuk = olcumler[0];
    for (int x : olcumler) {
        if (x > en_buyuk) {
            en_buyuk = x;
        }
    }
    std::cout << "En buyuk olcum: " << en_buyuk << "\n";

    // TEHLİKE: geçerli indeksler 0 ile 4 arası. Aşağıdaki satır başka bir verinin
    // alanına girer. Program çökebilir, çökmeyebilir, saçma bir değer okuyabilir.
    // Bu yüzden yorum satırında bırakıyoruz.
    // std::cout << olcumler[5] << "\n";

    return 0;
}
