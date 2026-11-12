// 06_sanal_fonksiyonlar.cpp
// virtual, override, saf sanal fonksiyon, soyut sınıf ve sanal yıkıcı
//
// Derleme: g++ -std=c++17 -Wall -Wextra 06_sanal_fonksiyonlar.cpp -o sanal

#include <iostream>

// Soyut sınıf: en az bir saf sanal (= 0) fonksiyonu var, kendisinden nesne oluşturulamaz.
// Bir "sözleşme" gibi düşünün: Sensor olmak isteyen herkes bu fonksiyonları yazmak zorunda.
class Sensor {
public:
    virtual ~Sensor() = default;           // SANAL YIKICI. Aşağıdaki nota bakın.

    virtual double oku(int ham_kod) = 0;   // saf sanal: gövdesi yok
    virtual const char* birim() const = 0;

    // Sanal ama saf değil: varsayılan bir davranışı var, isteyen değiştirir.
    virtual const char* ad() const { return "isimsiz sensor"; }

    // Sanal değil: herkes için aynı. İçeride sanal fonksiyonları çağırıyor.
    void rapor(int ham_kod)
    {
        double deger = oku(ham_kod);       // hangi oku()? Nesnenin gerçek tipine göre!
        std::cout << "  " << ad() << ": " << deger << " " << birim() << "\n";
    }
};

class SicaklikSensoru : public Sensor {
public:
    double oku(int ham_kod) override { return ham_kod * 3.3 / 4095.0 * 100.0; }
    const char* birim() const override { return "C"; }
    const char* ad() const override { return "sicaklik"; }
    ~SicaklikSensoru() override { std::cout << "  [SicaklikSensoru yikici]\n"; }
};

class IsikSensoru : public Sensor {
public:
    double oku(int ham_kod) override { return ham_kod * 1000.0 / 4095.0; }
    const char* birim() const override { return "lux"; }
    // ad() yazmadık, Sensor'deki varsayılanı kullanır.

    // override yazmanın faydası: imzada hata yaparsak derleyici yakalar.
    // const char* birim() override { ... }   // DERLENMEZ: const unutuldu, hiçbir şeyi ezmiyor
};

class GerilimSensoru : public Sensor {
public:
    double oku(int ham_kod) override { return ham_kod * 3.3 / 4095.0; }
    const char* birim() const override { return "V"; }
    const char* ad() const override { return "gerilim"; }
};

int main()
{
    // Sensor s;   // DERLENMEZ: soyut sınıftan nesne olmaz

    SicaklikSensoru t;
    IsikSensoru i;
    GerilimSensoru g;

    // Çok biçimlilik (polymorphism): farklı tipteki nesneleri aynı dizide tutup
    // hepsine aynı soruyu soruyoruz, her biri kendi bildiği gibi cevap veriyor.
    Sensor* sensorler[] = {&t, &i, &g};

    int ham_kodlar[] = {310, 2048, 1241};

    std::cout << "taban sinif pointer'lari uzerinden:\n";
    for (int k = 0; k < 3; k++) {
        sensorler[k]->rapor(ham_kodlar[k]);
    }

    // Yeni bir sensör tipi eklemek için bu döngüye DOKUNMAMIZ gerekmiyor.
    // Final projesindeki filtre zinciri tam olarak bu fikir üzerine kurulu.

    // ---------------------------------------------------------------
    // SANAL YIKICI NEDEN ŞART?
    // Heap'teki türetilmiş nesneyi taban pointer'ı üzerinden siliyoruz:
    std::cout << "\nheap nesnesini taban pointer'i ile silme:\n";
    Sensor* p = new SicaklikSensoru;
    delete p;
    // Sensor'deki yıkıcı virtual olmasaydı sadece ~Sensor çalışırdı,
    // ~SicaklikSensoru hiç çağrılmazdı (tanımsız davranış). O sınıf bellek
    // tutuyor olsaydı sızardı. Kural: sanal fonksiyonu olan sınıfın yıkıcısı da sanal olsun.
    // ---------------------------------------------------------------

    std::cout << "\nmain bitiyor\n";
    return 0;
}
