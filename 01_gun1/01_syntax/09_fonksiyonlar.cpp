// 09_fonksiyonlar.cpp
// Fonksiyon tanımı, çağırma, parametre, dönüş değeri, void, kapsam, bildirim,
// değerle ve referansla aktarım
//
// Derleme: g++ -std=c++17 -Wall -Wextra 09_fonksiyonlar.cpp -o fonksiyonlar

#include <iostream>
#include <cmath>     // std::log10, std::sqrt ...

// Bildirim (prototype): tanımı aşağıda olan fonksiyonu derleyiciye önceden tanıtır
int kare(int x);

// Basit bir fonksiyon: bir double alır, bir double döner
double guc_db(double guc_orani)
{
    return 10.0 * std::log10(guc_orani);
}

// void: geri bir şey döndürmez, işini yapar ve biter
void baslik_yaz(int no)
{
    std::cout << "=== Bolum " << no << " ===\n";
}

// Yerel değişken: fonksiyon her çağrıldığında baştan oluşur, bitince yok olur
void yerel_sayac()
{
    int sayac = 0;
    sayac++;
    std::cout << "yerel sayac = " << sayac << "\n";
}

// Değerle aktarım: x'in KOPYASI gelir, orijinal değişmez
void iki_katina_cikar_deger(int x)
{
    x = x * 2;
}

// Referansla aktarım: x orijinalin TA KENDİSİDİR (başka bir ismi)
void iki_katina_cikar_referans(int& x)
{
    x = x * 2;
}

// Bir fonksiyondan iki sonuç döndürmenin klasik yolu: referans parametre
void min_max_bul(const int dizi[], int n, int& en_kucuk, int& en_buyuk)
{
    en_kucuk = en_buyuk = dizi[0];
    for (int i = 1; i < n; i++) {
        if (dizi[i] < en_kucuk) en_kucuk = dizi[i];
        if (dizi[i] > en_buyuk) en_buyuk = dizi[i];
    }
}

int main()
{
    baslik_yaz(1);
    std::cout << "kare(6) = " << kare(6) << "\n";
    std::cout << "100 kat guc = " << guc_db(100.0) << " dB\n\n";

    baslik_yaz(2);
    yerel_sayac();
    yerel_sayac();    // yine 1 basar, zira sayac her çağrıda sıfırdan doğuyor

    // Kapsam: bir değişken, tanımlandığı süslü parantez bloğunun içinde yaşar
    int dis = 1;
    {
        int ic = 2;
        std::cout << "blok icinde dis + ic = " << dis + ic << "\n";
    }
    // std::cout << ic;   // derlenmez, ic artık yok
    std::cout << "\n";

    baslik_yaz(3);
    int a = 5;
    iki_katina_cikar_deger(a);
    std::cout << "Degerle sonra a = " << a << "   (degismedi!)\n";

    iki_katina_cikar_referans(a);
    std::cout << "Referansla sonra a = " << a << "\n\n";

    baslik_yaz(4);
    int veriler[] = {7, -3, 12, 0, 5};
    int kucuk, buyuk;
    min_max_bul(veriler, 5, kucuk, buyuk);
    std::cout << "min = " << kucuk << ", max = " << buyuk << "\n";

    return 0;
}

// kare fonksiyonunun tanımı main'in altında. Yukarıdaki bildirim sayesinde main onu tanıyor.
int kare(int x)
{
    return x * x;
}
