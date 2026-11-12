// 04_raii.cpp
// RAII: Kaynağı yapıcıda al, yıkıcıda geri ver. delete'i unutmak imkânsız hâle gelsin.
//
// Derleme: g++ -std=c++17 -Wall -Wextra 04_raii.cpp -o raii

#include <iostream>
#include <cstddef>

// ==================================================================
// ELLE YÖNETİM: Her çıkış kapısında delete[] yazmayı hatırlamak zorundayız.
// ==================================================================
bool olcum_eski(size_t n, bool sensor_hatasi)
{
    float* tampon = new float[n];

    if (sensor_hatasi) {
        std::cout << "  sensor hatasi, erken donuyoruz\n";
        return false;            // SIZINTI: tampon hiç silinmedi!
    }

    for (size_t i = 0; i < n; i++) tampon[i] = 0.0f;
    delete[] tampon;
    return true;
}

// ==================================================================
// RAII: Belleği bir nesneye emanet ediyoruz. Nesne ölünce bellek de gider.
// ==================================================================
class DinamikTampon {
public:
    explicit DinamikTampon(size_t n)       // explicit: aşağıdaki nota bakın
        : veri_(new float[n]), boyut_(n)
    {
        for (size_t i = 0; i < boyut_; i++) veri_[i] = 0.0f;
        std::cout << "  [yapici] " << boyut_ << " float alindi\n";
    }

    ~DinamikTampon()
    {
        delete[] veri_;
        std::cout << "  [yikici] " << boyut_ << " float geri verildi\n";
    }

    // Kopyalamayı yasaklıyoruz. Neden? main'in sonundaki açıklamaya bakın.
    DinamikTampon(const DinamikTampon&) = delete;
    DinamikTampon& operator=(const DinamikTampon&) = delete;

    float& operator[](size_t i) { return veri_[i]; }   // t[3] yazabilelim diye
    size_t boyut() const { return boyut_; }

private:
    float* veri_;
    size_t boyut_;
};

bool olcum_yeni(size_t n, bool sensor_hatasi)
{
    DinamikTampon tampon(n);

    if (sensor_hatasi) {
        std::cout << "  sensor hatasi, erken donuyoruz\n";
        return false;            // tampon'un yıkıcısı yine de çalışır, sızıntı yok
    }

    for (size_t i = 0; i < tampon.boyut(); i++) tampon[i] = 1.0f;
    return true;
}

int main()
{
    std::cout << "eski yol:\n";
    olcum_eski(1000, true);      // 4000 bayt kayboldu, kimse fark etmedi

    std::cout << "\nRAII ile, hatasiz:\n";
    olcum_yeni(1000, false);

    std::cout << "\nRAII ile, hatali:\n";
    olcum_yeni(1000, true);

    // ---------------------------------------------------------------
    // KOPYALAMA NEDEN TEHLİKELİ?
    // Kopyalamayı yasaklamasaydık derleyici bizim yerimize bir kopyalama yazardı.
    // O kopyalama üyeleri tek tek kopyalar: veri_ POINTER'ı kopyalanır, gösterdiği
    // bellek değil. İki nesne aynı belleği gösterir ve ikisi de ölürken delete[] der.
    //
    //   DinamikTampon a(10);
    //   DinamikTampon b = a;   // şu an DERLENMEZ, iyi ki derlenmez
    //   // main bitince: b siler, sonra a AYNI belleği tekrar siler -> çift silme, çökme
    //
    // Kural: Yıkıcı yazmanız gerekiyorsa kopyalamayı da düşünmeniz gerekir
    // (üçler kuralı / rule of three). Ya düzgün bir kopyalama yazın ya da = delete deyin.
    // ---------------------------------------------------------------

    // explicit olmasaydı şu satır sessizce derlenirdi ve 50 elemanlı bir tampon açardı:
    // DinamikTampon t = 50;

    std::cout << "\nmain bitiyor\n";
    return 0;
}
