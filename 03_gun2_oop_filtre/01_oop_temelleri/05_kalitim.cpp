// 05_kalitim.cpp
// Kalıtım: ortak olanı bir taban sınıfa koy, farklı olanı türetilmiş sınıfa.
//
// Derleme: g++ -std=c++17 -Wall -Wextra 05_kalitim.cpp -o kalitim

#include <iostream>
#include <string>

// Her sensörün bir adı ve okunma sayısı var. Bu ortak kısım.
class Sensor {
public:
    explicit Sensor(const std::string& ad) : ad_(ad), okuma_sayisi_(0)
    {
        std::cout << "  [Sensor yapici] " << ad_ << "\n";
    }

    ~Sensor() { std::cout << "  [Sensor yikici] " << ad_ << "\n"; }

    const std::string& ad() const { return ad_; }
    int okuma_sayisi() const { return okuma_sayisi_; }

    // Taban sınıfın kendi versiyonu. Türetilmiş sınıflar bunu "gizleyecek".
    const char* birim() const { return "?"; }

protected:            // dışarıya kapalı, ama türetilmiş sınıflar erişebilir
    std::string ad_;
    int okuma_sayisi_;
};

// "SicaklikSensoru bir Sensor'dür" (is-a ilişkisi)
class SicaklikSensoru : public Sensor {
public:
    // Önce taban sınıfın yapıcısı çalışmak zorunda, ilk değer listesinde onu çağırıyoruz.
    SicaklikSensoru(const std::string& ad, double ofset)
        : Sensor(ad), ofset_(ofset)
    {
        std::cout << "  [SicaklikSensoru yapici]\n";
    }

    ~SicaklikSensoru() { std::cout << "  [SicaklikSensoru yikici]\n"; }

    // Ham ADC kodunu santigrata çeviren basit bir model: 10 mV / °C
    double oku(int ham_kod)
    {
        okuma_sayisi_++;                         // protected olduğu için erişebiliyoruz
        double volt = ham_kod * 3.3 / 4095.0;
        return volt * 100.0 + ofset_;
    }

    const char* birim() const { return "C"; }

private:
    double ofset_;
};

class IsikSensoru : public Sensor {
public:
    explicit IsikSensoru(const std::string& ad) : Sensor(ad) {}

    double oku(int ham_kod)
    {
        okuma_sayisi_++;
        return ham_kod * 1000.0 / 4095.0;        // 0..1000 lux
    }

    const char* birim() const { return "lux"; }
};

int main()
{
    std::cout << "nesneler kuruluyor:\n";
    SicaklikSensoru oda("oda", -0.5);
    IsikSensoru pencere("pencere");

    std::cout << "\nokumalar:\n";
    std::cout << "  " << oda.ad() << ": " << oda.oku(300) << " " << oda.birim() << "\n";
    std::cout << "  " << oda.ad() << ": " << oda.oku(310) << " " << oda.birim() << "\n";
    std::cout << "  " << pencere.ad() << ": " << pencere.oku(2048) << " " << pencere.birim() << "\n";
    std::cout << "  " << oda.ad() << " " << oda.okuma_sayisi() << " kez okundu\n";

    // oda.ofset_ = 0;          // DERLENMEZ: private
    // oda.okuma_sayisi_ = 0;   // DERLENMEZ: protected, dışarıdan erişilemez

    // ----- Bir sonraki dosyanın konusu -----
    // Türetilmiş nesneyi taban sınıf pointer'ıyla gösterebiliriz:
    Sensor* s = &oda;
    std::cout << "\nSensor* uzerinden birim: " << s->birim() << "   (C bekliyorduk!)\n";
    // Derleyici sadece pointer'ın tipine (Sensor*) bakıp Sensor::birim'i çağırdı.
    // Gerçek nesnenin tipine bakmasını istiyorsak 'virtual' gerekiyor.

    std::cout << "\nmain bitiyor (yikim sirasina dikkat):\n";
    return 0;
}
