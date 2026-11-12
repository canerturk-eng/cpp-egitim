// 02_yapici_yikici.cpp
// Yapıcı (constructor), yıkıcı (destructor), ilk değer listesi ve nesnenin ömrü
//
// Derleme: g++ -std=c++17 -Wall -Wextra 02_yapici_yikici.cpp -o yapici_yikici

#include <iostream>
#include <cstdint>

// Ödev 02'deki ADC fonksiyonları, bu sefer bir sınıfın içinde.
// vref ve bit sayısını her çağrıda tekrar tekrar vermek yerine nesne bunları hatırlıyor.
class Adc {
public:
    // Yapıcı: sınıfla aynı isimde, dönüş tipi yok. Nesne doğarken bir kez çalışır.
    // ':' ile başlayan kısım ilk değer listesi. Üyeler gövde çalışmadan önce burada kurulur.
    Adc(double vref, unsigned bit)
        : vref_(vref), bit_(bit), lsb_(0.0)
    {
        // Geçersiz bir ADC nesnesi hiç doğmasın diye ayarları burada düzeltiyoruz.
        if (bit_ < 1 || bit_ > 16) {
            bit_ = 12;
        }
        if (vref_ <= 0.0) {
            vref_ = 3.3;
        }
        lsb_ = vref_ / (1u << bit_);
        std::cout << "  [yapici] " << bit_ << " bit ADC, LSB = " << lsb_ * 1000.0 << " mV\n";
    }

    // Parametresiz yapıcı. İşi diğer yapıcıya devrediyor (delegating constructor).
    Adc() : Adc(3.3, 12) {}

    // Yıkıcı: başında ~ var, parametre almaz. Nesne ölürken bir kez çalışır.
    ~Adc()
    {
        std::cout << "  [yikici] " << bit_ << " bit ADC yok oldu\n";
    }

    uint16_t kodla(double volt) const
    {
        uint32_t en_buyuk = (1u << bit_) - 1;
        if (volt <= 0.0) return 0;
        uint32_t kod = static_cast<uint32_t>(volt / lsb_);
        if (kod > en_buyuk) kod = en_buyuk;
        return static_cast<uint16_t>(kod);
    }

    double coz(uint16_t kod) const { return (kod + 0.5) * lsb_; }

    unsigned bit() const { return bit_; }

private:
    double   vref_;
    unsigned bit_;
    double   lsb_;
};

int main()
{
    std::cout << "main basladi\n";

    Adc a(3.3, 10);
    std::cout << "1.65 V -> kod " << a.kodla(1.65) << " -> " << a.coz(a.kodla(1.65)) << " V\n";

    {   // iç blok: buradaki nesneler bloğun sonunda ölür
        std::cout << "blok basladi\n";
        Adc b;               // parametresiz yapıcı
        Adc c(5.0, 99);      // 99 bit saçma, yapıcı 12'ye düzeltecek
        std::cout << "blok bitiyor\n";
    }   // <- önce c, sonra b yıkılır (doğuşun tersi sırayla)

    // Heap'teki nesne: new ile doğar, delete ile ölür. Blok bitince kendiliğinden ölmez!
    Adc* p = new Adc(2.5, 8);
    std::cout << "p->bit() = " << p->bit() << "\n";
    delete p;                // yıkıcı tam burada çalışır

    std::cout << "main bitiyor\n";
    return 0;
}   // <- a burada yıkılır
