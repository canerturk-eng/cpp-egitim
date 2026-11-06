// 11_derleyici_hatalari.cpp
// En sık karşılaşılan derleyici hatalarını kendi elinizle üretin.
// Bu hâliyle hatasız derlenir. Sırayla aşağıdaki değişiklikleri yapın, derleyin,
// mesajı okuyun, sonra değişikliği geri alın.
//
// Derleme: g++ -std=c++17 -Wall -Wextra 11_derleyici_hatalari.cpp -o hatalar

#include <iostream>   // HATA 4: bu satırın başına // koyun

int kare(int x)
{
    return x * x;
}

int main()
{
    int sayac = 5;

    // HATA 1: sıradaki satırın başındaki // işaretini silin (noktalı virgül eksik)
    // int eksik = 1

    // HATA 2: sıradaki satırın başındaki // işaretini silin (tanımsız değişken)
    // std::cout << sayc << "\n";

    // HATA 3: sıradaki satırın başındaki // işaretini silin (tip uyuşmazlığı)
    // int metin = "merhaba";

    // BONUS: sıradaki satırın başındaki // işaretini silin (eksik parametre)
    // std::cout << kare() << "\n";

    if (sayac > 0) {
        std::cout << "kare: " << kare(sayac) << "\n";
    }   // HATA 5: bu satırdaki kapanış parantezini silin

    return 0;
}
