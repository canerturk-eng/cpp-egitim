// 02_degiskenler.cpp
// Değişken tanımlama, ilk değer verme, isimlendirme, = ile == farkı
//
// Derleme: g++ -std=c++17 -Wall -Wextra 02_degiskenler.cpp -o degiskenler

#include <iostream>

int main()
{
    // Tanımlama: bellekte int büyüklüğünde bir kutu açtık, içi henüz belirsiz
    int sayac;
    // Atama: kutunun içine 3 koyduk
    sayac = 3;

    // Tanımlama ve ilk değer verme aynı satırda. Tavsiye edilen yol bu.
    int adet = 10;

    std::cout << "sayac = " << sayac << ", adet = " << adet << "\n";

    // Atama sağdan sola okunur: önce sağ taraf hesaplanır, sonra sola yazılır
    adet = adet + 1;
    std::cout << "adet + 1 sonrasi adet = " << adet << "\n";

    // İsimlendirme: harf, rakam ve alt çizgi serbest. Rakamla başlayamaz.
    int ornek_sayisi = 1024;      // bu repoda kullandığımız stil
    int ornekSayisi = 512;        // bu da geçerli, lakin farklı bir değişkendir
    std::cout << "ornek_sayisi = " << ornek_sayisi
              << ", ornekSayisi = " << ornekSayisi << "\n";

    // Aşağıdakiler derlenmez, başlarındaki yorum işaretini silip deneyin:
    // int 2kanal = 5;            // rakamla başlıyor
    // int ornek sayisi = 5;      // boşluk içeriyor
    // int return = 5;            // return dilin ayrılmış kelimesi

    // const: bir kere değer verilir, bir daha değişmez
    const double VREF = 3.3;
    std::cout << "VREF = " << VREF << "\n";
    // VREF = 5.0;                // derlenmez, deneyin

    // = atamadır, == karşılaştırmadır
    int a = 5;
    int b = 7;
    bool esit_mi = (a == b);      // a ve b değişmez, cevap esit_mi içine yazılır
    std::cout << "a == b sonucu: " << esit_mi << "   (0 yanlis, 1 dogru)\n";

    a = b;                        // a artık 7 oldu, b olduğu gibi kaldı
    esit_mi = (a == b);
    std::cout << "a = b yaptiktan sonra a == b sonucu: " << esit_mi << "\n";

    return 0;
}
