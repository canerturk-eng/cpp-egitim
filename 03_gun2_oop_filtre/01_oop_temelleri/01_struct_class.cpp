// 01_struct_class.cpp
// struct'tan class'a: üye fonksiyonlar, public ve private, this
//
// Derleme: g++ -std=c++17 -Wall -Wextra 01_struct_class.cpp -o struct_class

#include <iostream>

// ==================================================================
// ESKİ YOL (Ödev 03'teki gibi): veri bir yerde, fonksiyonlar başka yerde.
// Fonksiyon hangi sayaç üzerinde çalışacağını pointer ile öğreniyor.
// ==================================================================
struct SayacStruct {
    int deger;
    int ust_sinir;    // mikrodenetleyicideki zamanlayıcının "auto reload" register'ı gibi
};

void sayac_artir(SayacStruct* s)
{
    // Donanımdaki gibi eşitliğe bakıyoruz: sınıra ulaştıysa başa dön.
    if (s->deger == s->ust_sinir) {
        s->deger = 0;
    } else {
        s->deger++;
    }
}

// ==================================================================
// YENİ YOL: veri ve o veriyi kullanan fonksiyonlar tek çatı altında.
// ==================================================================
class Sayac {
public:                       // dışarıdan herkes kullanabilir
    void ayarla(int ust_sinir)
    {
        deger_ = 0;
        ust_sinir_ = ust_sinir;
    }

    void artir()
    {
        if (deger_ == ust_sinir_) {     // aslında this->deger_ == this->ust_sinir_
            deger_ = 0;
        } else {
            deger_++;
        }
    }

    // Sonundaki const: "bu fonksiyon nesneyi değiştirmez" sözü.
    int deger() const { return deger_; }

    void adresimi_yaz() const
    {
        std::cout << "  this = " << this << "\n";
    }

private:                      // yalnızca sınıfın kendi fonksiyonları dokunabilir
    int deger_ = 0;           // sondaki _ bir gelenek: "bu bir üye değişken"
    int ust_sinir_ = 0;
};

int main()
{
    std::cout << "--- struct ile ---\n";
    SayacStruct s = {0, 3};
    for (int i = 0; i < 5; i++) {
        sayac_artir(&s);
        std::cout << s.deger << " ";
    }
    std::cout << "\n";

    s.deger = 1000;   // Kimse bizi durdurmuyor!
    sayac_artir(&s);
    sayac_artir(&s);
    std::cout << "deger = 1000 yazdiktan sonra: " << s.deger << "   (artik asla 0'a donmeyecek)\n";

    std::cout << "\n--- class ile ---\n";
    Sayac a;
    a.ayarla(3);
    for (int i = 0; i < 5; i++) {
        a.artir();                    // a'yı parametre olarak vermiyoruz, a.artir() diyoruz
        std::cout << a.deger() << " ";
    }
    std::cout << "\n";

    // a.deger_ = 1000;   // DERLENMEZ: 'int Sayac::deger_' is private within this context

    // Aynı sınıftan iki nesne, iki ayrı veri. Fonksiyonlar ortak, veri değil.
    Sayac b;
    b.ayarla(10);
    b.artir();
    b.artir();
    std::cout << "a = " << a.deger() << ", b = " << b.deger() << "\n";

    // Üye fonksiyon, hangi nesne üzerinden çağrıldığını this pointer'ı ile bilir.
    std::cout << "\n&a = " << &a << "\n";
    a.adresimi_yaz();
    std::cout << "&b = " << &b << "\n";
    b.adresimi_yaz();

    return 0;
}
