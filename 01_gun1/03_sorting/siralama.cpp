// siralama.cpp
// Derste birlikte doldurduğumuz sıralama çalışması
//
// Derleme: g++ -std=c++17 -Wall -Wextra -O2 siralama.cpp -o siralama
//
// GÖREVLER
//   [Birlikte] bubble_sort
//   [Siz]      selection_sort   (9 dk)
//   [Siz]      insertion_sort   (9 dk)
//   [Birlikte] medyan           (gürültülü sensör verisinden)

#include <iostream>
#include <vector>
#include <algorithm>   // std::sort, std::is_sorted
#include <chrono>
#include <random>

// Derste yazdığımız swap. Pointer ile çalışıyor.
void degistir(int* a, int* b)
{
    int gecici = *a;
    *a = *b;
    *b = gecici;
}

// ==================================================================
// 1) BUBBLE SORT: Komşuları karşılaştır, büyük olanı sağa it.
//    Her turun sonunda en büyük eleman en sona "kabarcık gibi" çıkar.
// ==================================================================
void bubble_sort(int* dizi, int n)
{
    for (int tur = 0; tur < n - 1; tur++) {
        bool degisti = false;
        for (int j = 0; j < n - 1 - tur; j++) {
            if (dizi[j] > dizi[j + 1]) {
                degistir(&dizi[j], &dizi[j + 1]);
                degisti = true;
            }
        }
        if (!degisti) break;   // hiç değişim olmadıysa dizi zaten sıralı
    }
}

// ==================================================================
// 2) SELECTION SORT: Kalanların en küçüğünü bul, başa koy.
//    İPUCU: i. turda [i, n) aralığındaki en küçüğün İNDEKSİNİ bul,
//           sonra dizi[i] ile yer değiştir.
// ==================================================================
void selection_sort(int* dizi, int n)
{
    // TODO: burayı siz dolduracaksınız
    (void)dizi; (void)n;
}

// ==================================================================
// 3) INSERTION SORT: Elinizdeki iskambil kâğıtlarını dizer gibi.
//    Soldaki kısım hep sıralı. Yeni elemanı alıp doğru yere kaydırarak yerleştir.
//    İPUCU: anahtar = dizi[i], sonra solundaki büyükleri bir sağa kaydır.
// ==================================================================
void insertion_sort(int* dizi, int n)
{
    // TODO: burayı siz dolduracaksınız
    (void)dizi; (void)n;
}

// ==================================================================
// 4) MEDYAN: Sırala, ortadakini al.
//    Ortalama, tek bir dev gürültü sıçramasından çok etkilenir. Medyan etkilenmez.
//    12 Kasım'daki filtre projesinin ilk tuğlası bu.
// ==================================================================
int medyan(const int* dizi, int n)
{
    std::vector<int> kopya(dizi, dizi + n);   // orijinali bozmayalım
    insertion_sort(kopya.data(), n);
    return kopya[n / 2];
}

// ==================================================================
// Yardımcılar
// ==================================================================
void yazdir(const int* dizi, int n)
{
    for (int i = 0; i < n; i++) std::cout << dizi[i] << " ";
    std::cout << "\n";
}

using SiralamaFonk = void (*)(int*, int);   // fonksiyon pointer'ı! bonus konu

void sure_olc(const char* ad, SiralamaFonk fonk, std::vector<int> veri)   // kopya alıyor
{
    auto bas = std::chrono::high_resolution_clock::now();
    fonk(veri.data(), static_cast<int>(veri.size()));
    auto son = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(son - bas).count();
    bool dogru = std::is_sorted(veri.begin(), veri.end());
    std::cout << "  " << ad << ": " << ms << " ms  " << (dogru ? "[OK]" : "[YANLIS]") << "\n";
}

void std_sort_sarmal(int* dizi, int n) { std::sort(dizi, dizi + n); }

int main()
{
    // Küçük deneme
    int a[] = {5, 2, 9, 1, 7, 3};
    int b[] = {5, 2, 9, 1, 7, 3};
    int c[] = {5, 2, 9, 1, 7, 3};
    bubble_sort(a, 6);    std::cout << "bubble   : "; yazdir(a, 6);
    selection_sort(b, 6); std::cout << "selection: "; yazdir(b, 6);
    insertion_sort(c, 6); std::cout << "insertion: "; yazdir(c, 6);

    // Gürültülü sensör
    // Sıcaklık sensörü 25 civarı ölçüyor, ama arada bir parazit (spike) yiyor
    int sensor[] = {25, 26, 24, 25, 999, 25, 26, 24, 25};
    int n = sizeof(sensor) / sizeof(sensor[0]);
    long toplam = 0;
    for (int x : sensor) toplam += x;
    std::cout << "\nSensor ortalama: " << static_cast<double>(toplam) / n << "\n";
    std::cout << "Sensor medyan  : " << medyan(sensor, n) << "\n";

    // Hız karşılaştırması
    const int N = 20000;
    std::vector<int> rastgele(N);
    std::mt19937 uretec(2026);
    std::uniform_int_distribution<int> dagilim(0, 100000);
    for (int& x : rastgele) x = dagilim(uretec);

    std::cout << "\n" << N << " eleman siralama suresi:\n";
    sure_olc("bubble   ", bubble_sort,     rastgele);
    sure_olc("selection", selection_sort,  rastgele);
    sure_olc("insertion", insertion_sort,  rastgele);
    sure_olc("std::sort", std_sort_sarmal, rastgele);

    return 0;
}
