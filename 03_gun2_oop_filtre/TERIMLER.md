# Gün 2 Terimleri | Teknik Sözlük

İkinci günün bütün terimlerinin kısa ve doğrudan tanımları. Metafor ya da hikâye yok; onları isteyen [SOZLUK.md](SOZLUK.md) sayfasına bakabilir. Her terimin yanındaki numara, [ders notunda](README.md) anlatıldığı bölümü gösteriyor.

Her tanımın altında kısa bir kod örneği var. Örneklerin çoğu bu klasördeki gerçek koddan ([01_oop_temelleri](01_oop_temelleri), [02_filtre_kutuphanesi](02_filtre_kutuphanesi)) alındı ya da onun sadeleştirilmiş hâli. `...` ile gösterilen yerler örnekle ilgisi olmayan, atlanmış kısımlardır.

Terimler konularına göre gruplandı. Her grubun içinde, öğrenme sırasına göre dizildiler.

---

## Sınıf Temelleri

**Sınıf** (*class*) · 1.1
Veriyi (üye değişkenler) ve o veri üzerinde çalışan fonksiyonları (üye fonksiyonlar) tek bir tipte toplayan kullanıcı tanımlı tip.

```cpp
class Sayac {
public:
    void artir();
    int deger() const;
private:
    int deger_ = 0;
    int ust_sinir_ = 0;
};
```

**Nesne** (*object*) · 1.1
Bir sınıf tipinden oluşturulmuş, bellekte yer kaplayan somut örnek.

```cpp
Sayac a;      // Sayac sınıf, a nesne
Sayac b;      // aynı sınıftan ikinci, bağımsız bir nesne
```

**Üye değişken** (*member variable, data member*) · 1.1
Sınıfın içinde tanımlanan değişken. Her nesnenin kendi kopyası vardır. Bu repoda sonuna `_` konuyor.

```cpp
class RingBuffer {
    ...
private:
    size_t kapasite_;   // her RingBuffer nesnesinin kendi kapasite_ değeri var
    float* veri_;
};
```

**Üye fonksiyon** (*member function, method*) · 1.1
Sınıfın içinde tanımlanan fonksiyon. Bir nesne üzerinden çağrılır ve o nesnenin üyelerine doğrudan erişir. Makine kodu bütün nesneler için ortaktır.

```cpp
a.artir();            // sayac_artir(&a) yerine
b.artir();            // aynı fonksiyon, b'nin verisi üzerinde
```

**Erişim belirteci** (*access specifier*) · 1.2
Bir üyeye kimin erişebileceğini belirler:

| Belirteç | Sınıfın kendisi | Türetilmiş sınıflar | Dışarısı |
|---|:---:|:---:|:---:|
| `public` | ✓ | ✓ | ✓ |
| `protected` | ✓ | ✓ | ✗ |
| `private` | ✓ | ✗ | ✗ |

```cpp
class Sensor {
public:
    const std::string& ad() const;   // herkes çağırabilir
protected:
    int okuma_sayisi_;               // türetilmiş sınıflar erişebilir
private:
    int gizli_;                      // yalnızca Sensor'ün kendisi
};
```

**`struct` ile `class` farkı** · 1.2
Tek fark varsayılan erişimdir: `struct` üyeleri varsayılan olarak `public`, `class` üyeleri `private` olur. Gelenek olarak `struct` yalnızca veri taşıyan basit yapılar için kullanılır.

```cpp
struct A { int x; };   // x public
class  B { int x; };   // x private

A a; a.x = 1;          // derlenir
B b; b.x = 1;          // DERLENMEZ
```

**`this`** · 1.3
Her (statik olmayan) üye fonksiyona gizlice geçirilen, fonksiyonun çağrıldığı nesnenin adresini tutan pointer.

```cpp
void Sayac::artir()
{
    deger_++;           // derleyici bunu this->deger_++ olarak anlar
}

void adresimi_yaz() const { std::cout << this << "\n"; }   // a.adresimi_yaz() → &a
```

**`const` üye fonksiyon** (*const member function*) · 1.4
İmzasının sonunda `const` olan, nesnenin üyelerini değiştirmeyeceğini garanti eden fonksiyon. `const` bir nesne ya da `const` referans üzerinden yalnızca bu fonksiyonlar çağrılabilir.

```cpp
size_t adet() const { return adet_; }     // değiştirmiyor, const olabilir
void   ekle(float deger);                 // değiştiriyor, const olamaz

void f(const RingBuffer& rb)
{
    rb.adet();          // derlenir
    rb.ekle(1.0f);      // DERLENMEZ
}
```

**`static` üye fonksiyon** (*static member function*) · 5.7
Bir nesneye bağlı olmayan, `this` pointer'ı almayan üye fonksiyon. Sınıf adıyla çağrılır.

```cpp
class IIRAlcakGeciren : public Filtre {
public:
    static float alfa_hesapla(float fc, float fs);
    ...
};

float a = IIRAlcakGeciren::alfa_hesapla(10.0f, 1000.0f);   // nesne gerekmiyor
```

**`static constexpr` üye** · 5.5
Bütün nesneler için ortak, derleme anında bilinen sabit.

```cpp
class MedyanFiltre : public Filtre {
public:
    static constexpr size_t MAKS_PENCERE = 31;
private:
    float siralama_[MAKS_PENCERE];      // dizi boyutu olarak kullanılabilir
};
```

---

## Nesnenin Ömrü

**Yapıcı** (*constructor*) · 1.5
Nesne oluşturulurken otomatik çağrılan, sınıfla aynı adı taşıyan, dönüş tipi olmayan özel fonksiyon. Görevi nesneyi geçerli bir başlangıç durumuna getirmektir. Aşırı yüklenebilir.

```cpp
Adc(double vref, unsigned bit) : vref_(vref), bit_(bit)
{
    if (bit_ < 1 || bit_ > 16) bit_ = 12;    // geçersiz nesne doğmasın
    if (vref_ <= 0.0) vref_ = 3.3;
}

Adc a(3.3, 10);    // yapıcı burada çalışır
```

**Varsayılan yapıcı** (*default constructor*) · 1.5
Parametresiz çağrılabilen yapıcı. Sınıfa hiç yapıcı yazılmazsa derleyici boş bir tane üretir; tek bir yapıcı bile yazılırsa üretmez.

```cpp
class RingBuffer3 { public: RingBuffer3() : bas_(0), adet_(0) {} ... };
RingBuffer3 rb;        // varsayılan yapıcı

class Adc2 { public: Adc2(double vref, unsigned bit); };
Adc2 x;                // DERLENMEZ: varsayılan yapıcı yok
```

**Yetki devreden yapıcı** (*delegating constructor*) · 1.5
İşini aynı sınıfın başka bir yapıcısına devreden yapıcı.

```cpp
Adc() : Adc(3.3, 12) {}     // parametresiz yapıcı, işi iki parametreliye bırakıyor
```

**İlk değer listesi** (*member initializer list*) · 1.5
Yapıcı imzasından sonra `:` ile başlayan, üyeleri gövde çalışmadan önce kuran liste. Referans üyeler, `const` üyeler ve varsayılan yapıcısı olmayan nesne üyeler yalnızca burada kurulabilir.

```cpp
HareketliOrtalama::HareketliOrtalama(size_t pencere)
    : gecmis_(pencere),     // RingBuffer'ın varsayılan yapıcısı yok, burada kurulmak zorunda
      toplam_(0.0)
{
}
```

**Üye kurulum sırası** (*member initialization order*) · 5.3
Üyeler, ilk değer listesindeki yazım sırasına göre değil, **sınıf tanımındaki bildirim sırasına** göre kurulur. GCC'de `-Wreorder` uyarısı (`-Wall` içinde) sıra uyuşmazlığını bildirir.

```cpp
class RingBuffer {
    size_t kapasite_;   // 1. kurulur
    float* veri_;       // 2. kurulur
    ...
};

RingBuffer::RingBuffer(size_t kapasite)
    : kapasite_(kapasite == 0 ? 1 : kapasite),
      veri_(new float[kapasite_])          // kapasite_ hazır, güvenli
{ ... }
// veri_ sınıfta önce bildirilseydi, kapasite_ henüz çöp değerdeyken okunurdu.
```

**Yıkıcı** (*destructor*) · 1.6
Nesnenin ömrü bittiğinde otomatik çağrılan, `~SinifAdi()` biçiminde yazılan, parametre almayan fonksiyon. Her sınıfın tam bir yıkıcısı vardır.

```cpp
RingBuffer::~RingBuffer()
{
    delete[] veri_;
}
```

**Otomatik ömür** (*automatic storage duration*) · 1.6
Stack'teki yerel nesnelerin ömrü. Nesne tanımlandığı blok `}` ile bitince yıkılır. Aynı bloktaki nesneler oluşturulma sırasının tersine yıkılır.

```cpp
{
    Adc b;            // 1. kurulur
    Adc c(5.0, 99);   // 2. kurulur
}                     // önce c, sonra b yıkılır
```

**Dinamik ömür** (*dynamic storage duration*) · 1.6
`new` ile heap'te oluşturulan nesnelerin ömrü. Nesne yalnızca `delete` çağrıldığında yıkılır; çağrılmazsa bellek sızar.

```cpp
Adc* p = new Adc(2.5, 8);    // yapıcı çalışır
...
delete p;                    // yıkıcı tam burada çalışır
```

**Statik ömür** (*static storage duration*) · 1.6
Global ve `static` nesnelerin ömrü. Program başlarken kurulur, program biterken yıkılır.

```cpp
float temiz[1000];                 // global: main'den önce kurulur, program bitince gider

int main()
{
    static float cikislar[5][1000];   // static yerel: ilk geçişte kurulur, program bitince gider
}
```

---

## Kapsülleme ve Dosya Düzeni

**Kapsülleme** (*encapsulation*) · 2.1
Bir sınıfın iç verisini `private` yapıp dışarıya yalnızca işlem yapan bir arayüz sunması. Amacı değişmezleri korumaktır.

```cpp
RingBuffer rb(8);
rb.ekle(3.0f);       // arayüz üzerinden: kurallar korunuyor
rb.adet_ = 50;       // DERLENMEZ: iç veriye doğrudan erişim yok
```

**Değişmez** (*class invariant*) · 2.2
Bir nesne için her public fonksiyon çağrısından önce ve sonra doğru olması gereken koşul. Yapıcı değişmezi kurar, üye fonksiyonlar korur.

```cpp
// RingBuffer değişmezi: bas_ < kapasite_  ve  adet_ <= kapasite_
void RingBuffer::ekle(float deger)
{
    if (dolu()) {                         // adet_ kapasiteyi geçemez
        veri_[bas_] = deger;
        bas_ = (bas_ + 1) % kapasite_;    // bas_ kapasitenin altında kalır
    } else {
        veri_[(bas_ + adet_) % kapasite_] = deger;
        adet_++;
    }
}
```

**Arayüz** (*interface*) · 2.2
Bir sınıfın dışarıdan kullanılabilen public üyelerinin bütünü. Sınıfın ne yaptığını tanımlar, nasıl yaptığını gizler.

```cpp
class RingBuffer {
public:                              // arayüz: ne yapıyor
    void   ekle(float deger);
    float  bak(size_t gecmis) const;
    void   temizle();
    size_t adet() const;
private:                             // gerçekleme: nasıl yapıyor
    ...
};
```

**Getter ve setter** (*accessor, mutator*) · 2.2
Bir üyeyi okuyan ve yazan public fonksiyonlar. Her üyeye koşulsuz bir setter açmak kapsüllemeyi fiilen ortadan kaldırır.

```cpp
size_t adet() const { return adet_; }     // getter: sorun yok
void set_adet(size_t a) { adet_ = a; }    // setter: değişmezi bozar, YAZMAYIN
```

**Referans** (*reference*) · 0, 2.3
Var olan bir nesneye takılan ikinci isim. Kurulduktan sonra başka bir nesneye bağlanamaz ve `nullptr` olamaz.

```cpp
bool cikar(float& deger);    // çağıranın değişkenine doğrudan yazar

float x;
rb.cikar(x);                 // &x yazmaya gerek yok, x değişir
```

**`const` referans** (*const reference*) · 2.3
Nesneyi kopyalamadan, değiştirmeme garantisiyle geçirmek için kullanılan parametre biçimi.

```cpp
void yazdir(const RingBuffer& rb)
{
    for (size_t i = 0; i < rb.adet(); i++) std::cout << rb.bak(i) << " ";
}
```

**Parametre geçirme kuralı** · 2.3
Küçük tipler değerle, okunacak nesneler `const T&` ile, değiştirilecek nesneler `T&` ile geçirilir.

```cpp
float isle(float x);                          // küçük tip: değerle
void  yazdir(const RingBuffer& rb);           // okunacak nesne: const&
bool  cikar(float& deger);                    // değiştirilecek: &
```

**Başlık dosyası** (*header, `.hpp`*) · 2.4
Sınıf tanımlarını ve fonksiyon bildirimlerini içeren, `#include` ile eklenen dosya.

```cpp
// ring_buffer.hpp
#pragma once
#include <cstddef>

class RingBuffer {
public:
    explicit RingBuffer(size_t kapasite);
    void ekle(float deger);
    ...
};
```

**Kaynak dosyası** (*source, `.cpp`*) · 2.4
Fonksiyon gövdelerini içeren, ayrı ayrı derlenen dosya. Üye fonksiyon gövdeleri sınıf adıyla nitelenir.

```cpp
// ring_buffer.cpp
#include "ring_buffer.hpp"

void RingBuffer::temizle()
{
    bas_ = 0;
    adet_ = 0;
}
```

**Kapsam çözümleme operatörü** (*scope resolution operator, `::`*) · 2.4
Bir adın hangi sınıfa ya da isim alanına ait olduğunu belirtir.

```cpp
void RingBuffer::ekle(float deger) { ... }   // RingBuffer sınıfının ekle'si
std::cout << "merhaba";                      // std isim alanının cout'u
size_t m = MedyanFiltre::MAKS_PENCERE;       // sınıfın statik sabiti
```

**`#pragma once`** · 2.4
Bir başlık dosyasının aynı derleme birimine birden fazla eklenmesini engelleyen yönerge.

```cpp
// filtre.hpp
#pragma once
#include "ring_buffer.hpp"    // ring_buffer.hpp başka yerden de eklense tek kez işlenir
```

**İsimsiz isim alanı** (*anonymous namespace*) · 5.5
İçindeki adlar yalnızca o `.cpp` dosyasından görünür.

```cpp
// medyan_filtre.cpp
namespace {
void insertion_sort(float* dizi, size_t n) { ... }   // başka dosyalar bunu göremez
}
```

---

## Kaynak Yönetimi

**Bellek sızıntısı** (*memory leak*) · 3.1
`new` ile alınan belleğin hiç `delete` edilmemesi. Program çalıştıkça kullanılan bellek artar.

```cpp
bool olcum_eski(size_t n, bool sensor_hatasi)
{
    float* tampon = new float[n];
    if (sensor_hatasi) return false;     // SIZINTI: tampon silinmedi
    delete[] tampon;
    return true;
}
```

**RAII** (*Resource Acquisition Is Initialization*) · 3.2
Bir kaynağın ömrünü bir nesnenin ömrüne bağlama tekniği: kaynak yapıcıda alınır, yıkıcıda bırakılır. Yıkıcı her çıkış yolunda çalıştığı için kaynak sızmaz.

```cpp
class DinamikTampon {
public:
    explicit DinamikTampon(size_t n) : veri_(new float[n]) {}   // al
    ~DinamikTampon() { delete[] veri_; }                        // bırak
private:
    float* veri_;
};

bool olcum_yeni(size_t n, bool sensor_hatasi)
{
    DinamikTampon tampon(n);
    if (sensor_hatasi) return false;     // yıkıcı yine çalışır, sızıntı yok
    return true;
}
```

**Kopyalama yapıcısı** (*copy constructor*) · 3.3
Bir nesneyi aynı tipteki başka bir nesneden kuran yapıcı.

```cpp
DinamikTampon(const DinamikTampon& diger);   // bildirimi

DinamikTampon b = a;                         // çağrıldığı yer
```

**Kopyalayarak atama operatörü** (*copy assignment operator*) · 3.3
Var olan bir nesneye başka bir nesneyi atayan operatör.

```cpp
DinamikTampon& operator=(const DinamikTampon& diger);   // bildirimi

b = a;                                                  // çağrıldığı yer (b zaten var)
```

**Yüzeysel kopya** (*shallow copy*) · 3.3
Derleyicinin ürettiği varsayılan kopyalama: üyeleri tek tek kopyalar. Pointer üyelerde gösterilen bellek değil adres kopyalanır.

```cpp
// Derleyicinin ürettiği kopyalama aşağı yukarı şunu yapar:
DinamikTampon(const DinamikTampon& d) : veri_(d.veri_), boyut_(d.boyut_) {}
// a.veri_ ve b.veri_ artık AYNI belleği gösteriyor.
```

**Derin kopya** (*deep copy*) · 3.3
Pointer'ın gösterdiği belleği de yeni bir alana kopyalayan, elle yazılmış kopyalama.

```cpp
DinamikTampon(const DinamikTampon& d)
    : veri_(new float[d.boyut_]), boyut_(d.boyut_)    // yeni bellek
{
    for (size_t i = 0; i < boyut_; i++) veri_[i] = d.veri_[i];   // içeriği kopyala
}
```

**Çift silme** (*double free*) · 3.3
Aynı belleğin iki kez `delete` edilmesi. Tanımsız davranıştır; ekseriya program çöker.

```cpp
{
    DinamikTampon a(10);
    DinamikTampon b = a;    // yüzeysel kopya (kopyalama yasaklanmamışsa)
}                           // b siler, sonra a aynı belleği tekrar siler → çift silme
```

**Tanımsız davranış** (*undefined behavior*) · 3.3, 4.6
Dil standardının sonucunu tanımlamadığı durum. Program çökebilir, yanlış çalışabilir ya da doğru çalışıyormuş gibi görünebilir.

```cpp
float* p = new float[4];
delete[] p;
p[0] = 1.0f;      // silinmiş belleğe yazmak: tanımsız davranış
```

**Üçler kuralı** (*rule of three*) · 3.3
Bir sınıf yıkıcı, kopyalama yapıcısı ya da kopyalayarak atama operatöründen birini elle tanımlıyorsa büyük ihtimalle üçünü de tanımlamalı (ya da yasaklamalıdır).

```cpp
class FIRFiltre : public Filtre {
public:
    ~FIRFiltre() override;                            // 1. yıkıcı
    FIRFiltre(const FIRFiltre&) = delete;             // 2. kopyalama yapıcısı
    FIRFiltre& operator=(const FIRFiltre&) = delete;  // 3. kopyalayarak atama
    ...
};
```

**Silinmiş fonksiyon** (*deleted function, `= delete`*) · 3.3
Kullanılması derleme hatası veren fonksiyon. Kopyalamayı yasaklamak için kullanılır.

```cpp
RingBuffer(const RingBuffer&) = delete;
RingBuffer& operator=(const RingBuffer&) = delete;

RingBuffer a(8);
RingBuffer b = a;     // DERLENMEZ: use of deleted function
```

**`= default`** · 4.6
Derleyiciye bir özel fonksiyonun varsayılan hâlini üretmesini açıkça söyler.

```cpp
class Filtre {
public:
    virtual ~Filtre() = default;    // gövdeyi derleyici yazsın, ama sanal olsun
};
```

**`explicit`** · 3.4
Tek parametreli bir yapıcının örtük tip dönüşümünde kullanılmasını engeller.

```cpp
explicit DinamikTampon(size_t n);

DinamikTampon t(50);       // derlenir: açıkça yapıcı çağrısı
DinamikTampon u = 50;      // DERLENMEZ: örtük dönüşüm yasak
```

**Bileşim** (*composition, has-a*) · 3.5
Bir sınıfın başka bir sınıfın nesnesini üye olarak içermesi. Üye nesneler, içeren nesne yıkılırken otomatik yıkılır.

```cpp
class HareketliOrtalama : public Filtre {
    ...
private:
    RingBuffer gecmis_;    // HareketliOrtalama bir RingBuffer'a sahip
    double toplam_;
};
// Yıkıcı yazılmadı: gecmis_ kendi belleğini kendisi iade ediyor.
```

**Sahiplik** (*ownership*) · 5.8
Bir kaynağı silmekten hangi nesnenin sorumlu olduğu.

```cpp
FIRFiltre::~FIRFiltre() { delete[] b_; }   // FIRFiltre katsayıların sahibi: siliyor

class FiltreZinciri : public Filtre {
    Filtre* filtreler_[MAKS];              // sahibi DEĞİL: yıkıcısında silmiyor
};
```

---

## Kalıtım ve Çok Biçimlilik

**Kalıtım** (*inheritance, is-a*) · 4.1
Bir sınıfın (türetilmiş) başka bir sınıfın (taban) üyelerini devralması. Public kalıtımda türetilmiş nesne, taban tipin beklendiği her yerde kullanılabilir.

```cpp
class SicaklikSensoru : public Sensor { ... };

SicaklikSensoru oda("oda", -0.5);
oda.ad();               // Sensor'den miras, SicaklikSensoru'da yazılmadı
```

**Taban sınıf** (*base class*) ve **türetilmiş sınıf** (*derived class*) · 4.1
Kalıtımda üyelerini devreden sınıf taban, devralan sınıf türetilmiş sınıftır.

```cpp
class Filtre { ... };                          // taban sınıf
class MedyanFiltre : public Filtre { ... };    // türetilmiş sınıf
```

**Kurulum ve yıkım sırası** (kalıtımda) · 4.1
Kurulumda önce taban sınıf, sonra türetilmiş sınıfın üyeleri, en son türetilmiş sınıfın yapıcı gövdesi çalışır. Yıkım tam tersi sıradadır. Taban sınıfın yapıcısı parametre istiyorsa ilk değer listesinde çağrılır.

```cpp
SicaklikSensoru(const std::string& ad, double ofset)
    : Sensor(ad),          // 1. taban sınıf
      ofset_(ofset)        // 2. kendi üyeleri
{ }                        // 3. gövde
// Yıkım: ~SicaklikSensoru, sonra ~Sensor
```

**Statik bağlama** (*static binding, early binding*) · 4.2
Hangi fonksiyonun çağrılacağına derleme anında, pointer ya da referansın tipine bakarak karar verilmesi. Sanal olmayan fonksiyonlar böyle çağrılır.

```cpp
// birim() sanal DEĞİL
Sensor* s = &oda;      // oda bir SicaklikSensoru
s->birim();            // Sensor::birim çağrılır, "?" döner
```

**Dinamik bağlama** (*dynamic binding, late binding*) · 4.3
Hangi fonksiyonun çağrılacağına çalışma anında, nesnenin gerçek tipine bakarak karar verilmesi. Sanal fonksiyonlar böyle çağrılır.

```cpp
// birim() sanal
Sensor* s = &oda;
s->birim();            // SicaklikSensoru::birim çağrılır, "C" döner
```

**Sanal fonksiyon** (*virtual function*) · 4.3
Taban sınıfta `virtual` ile bildirilen, türetilmiş sınıflarda yeniden tanımlanabilen ve taban pointer ya da referans üzerinden çağrıldığında dinamik bağlanan fonksiyon.

```cpp
class Sensor {
public:
    virtual const char* ad() const { return "isimsiz sensor"; }
};
```

**`override`** · 4.3
Bir fonksiyonun taban sınıftaki sanal bir fonksiyonu ezdiğini belirten belirteç. İmza uyuşmazsa derleme hatası verir.

```cpp
const char* birim() const override { return "C"; }   // doğru
const char* birim() override { return "C"; }         // DERLENMEZ: const eksik, hiçbir şeyi ezmiyor
```

**vtable** (*virtual table*) · 4.3
Sanal fonksiyonu olan her sınıf için derleyicinin oluşturduğu, sanal fonksiyonların adreslerini tutan tablo. Her nesne bu tabloyu gösteren gizli bir pointer (*vptr*) taşır.

```cpp
class Duz   { int x; };
class Sanal { int x; virtual void f() {} };

sizeof(Duz);     // 4
sizeof(Sanal);   // 16 (64 bit sistemde): 8 bayt gizli vptr + int + hizalama
```

**Saf sanal fonksiyon** (*pure virtual function*) · 4.4
Gövdesi olmayan, `= 0` ile biten sanal fonksiyon. Somut olmak isteyen her türetilmiş sınıf onu tanımlamak zorundadır.

```cpp
class Filtre {
public:
    virtual float isle(float x) = 0;
    virtual void sifirla() = 0;
    virtual const char* ad() const = 0;
};
```

**Soyut sınıf** (*abstract class*) · 4.4
En az bir saf sanal fonksiyonu olan sınıf. Doğrudan nesnesi oluşturulamaz; yalnızca pointer ya da referans tipi olarak ve taban sınıf olarak kullanılır.

```cpp
Filtre f;               // DERLENMEZ: cannot declare variable 'f' to be of abstract type
Filtre* p = &medyan;    // derlenir: soyut tipe pointer serbest
```

**Somut sınıf** (*concrete class*) · 4.4
Bütün saf sanal fonksiyonları tanımlanmış, nesnesi oluşturulabilen sınıf.

```cpp
class IIRAlcakGeciren : public Filtre {
public:
    float isle(float x) override;
    void sifirla() override;
    const char* ad() const override { return "IIR alcak geciren"; }
};

IIRAlcakGeciren iir(0.1f);     // derlenir
```

**Çok biçimlilik** (*polymorphism*) · 4.5
Farklı tipteki nesnelerin ortak bir taban tip üzerinden aynı arayüzle kullanılması ve her birinin kendi davranışını göstermesi.

```cpp
Filtre* filtreler[] = {&ortalama, &medyan, &fir, &iir, &zincir};
for (Filtre* f : filtreler) {
    f->isle_dizi(gurultulu, cikis, N);    // her biri kendi isle()'siyle çalışır
    std::printf("%s\n", f->ad());
}
```

**Şablon yöntem kalıbı** (*template method pattern*) · 4.5, 5.2
Taban sınıftaki sanal olmayan bir fonksiyonun işin iskeletini kurup ayrıntılar için sanal fonksiyonları çağırması.

```cpp
void Filtre::isle_dizi(const float* giris, float* cikis, size_t n)   // sanal değil
{
    if (giris == nullptr || cikis == nullptr) return;
    for (size_t i = 0; i < n; i++) {
        cikis[i] = isle(giris[i]);                                   // sanal
    }
}
```

**Sanal yıkıcı** (*virtual destructor*) · 4.6
Taban sınıfta `virtual` bildirilen yıkıcı. Türetilmiş bir nesne taban pointer üzerinden `delete` edildiğinde türetilmiş sınıfın yıkıcısının da çalışmasını sağlar.

```cpp
class Filtre {
public:
    virtual ~Filtre() = default;
};

Filtre* p = new FIRFiltre(katsayilar, 3);
delete p;    // önce ~FIRFiltre (katsayılar silinir), sonra ~Filtre
```

**Bileşik kalıbı** (*composite pattern*) · 5.8
Bir nesne grubunun, gruptaki tek bir nesneyle aynı arayüze sahip olması.

```cpp
FiltreZinciri ic;
ic.ekle(&medyan);
ic.ekle(&ortalama);

FiltreZinciri dis;
dis.ekle(&ic);         // zincir de bir Filtre, başka zincire eklenebilir
dis.ekle(&iir);
```

---

## Dijital Sinyal İşleme

**Örnekleme frekansı** (*sampling frequency, fs*) · 5
Sürekli bir sinyalden saniyede alınan örnek sayısı (Hz).

```cpp
constexpr float FS = 1000.0f;    // saniyede 1000 örnek
SinyalUreteci uretec(FS);
// i. örneğin zamanı: t = i / FS saniye
```

**Nyquist frekansı** · 5.6
Örnekleme frekansının yarısı (`fs / 2`). Bu frekansın üstündeki bileşenler doğru temsil edilemez.

```cpp
fir_alcak_geciren_tasarla(k, 31, 400.0f, 1000.0f);   // true:  400 < 500
fir_alcak_geciren_tasarla(k, 31, 600.0f, 1000.0f);   // false: 600 >= fs/2
```

**Beyaz gürültü** (*white noise*) · 5
Her frekansta eşit güce sahip rastgele gürültü.

```cpp
std::normal_distribution<float> dagilim(0.0f, std_sapma);   // ortalama 0, Gauss
for (size_t i = 0; i < n; i++) tampon[i] += dagilim(uretec_);
```

**Dürtü gürültüsü** (*impulse noise*) · 5
Nadiren ortaya çıkan, büyük genlikli ani sıçramalar.

```cpp
uretec.durtu_ekle(gurultulu, N, 0.01f, 3.0f);   // her örnek %1 ihtimalle ±3 sıçrar
```

**Akış işleme** (*stream processing*) · 5.2
Sinyalin tamamını beklemeden, her yeni örneği geldiği anda işleyip bir çıkış örneği üretme.

```cpp
// Mikrodenetleyicide her ADC kesmesinde:
void adc_kesmesi()
{
    float x = adc_oku();
    float y = filtre.isle(x);    // tek örnek girer, tek örnek çıkar
    dac_yaz(y);
}
```

**Nedensel filtre** (*causal filter*) · 5.5
Çıkışı yalnızca şimdiki ve geçmiş girişlere bağlı olan filtre.

```cpp
// y[n] yalnızca x[n], x[n-1], x[n-2] kullanıyor
float y = (x_n + x_n1 + x_n2) / 3.0f;
```

**Merkezi filtre** (*non-causal, centered filter*) · 5.5
Çıkışı gelecekteki girişlere de bağlı olan filtre. Yalnızca kaydedilmiş veride uygulanabilir.

```cpp
// y[i], gelecekteki x[i+1]'i de kullanıyor: canlı veride mümkün değil
cikis[i] = (giris[i - 1] + giris[i] + giris[i + 1]) / 3.0f;
```

**Ring buffer** (*circular buffer*) · 5.3
Sabit kapasiteli, sonu başına bağlıymış gibi kullanılan dizi. Doluyken yeni örnek en eskinin yerine yazılır.

```cpp
RingBuffer rb(3);
rb.ekle(1); rb.ekle(2); rb.ekle(3);
rb.ekle(4);           // dolu: 1'in üzerine yazar
rb.bak(0);            // 4 (en yeni)
rb.bak(2);            // 2 (en eski)
```

**Hareketli ortalama** (*moving average*) · 5.4
Son N örneğin aritmetik ortalaması. Toplam her adımda güncellenerek N'den bağımsız sürede hesaplanır.

```cpp
float HareketliOrtalama::isle(float x)
{
    if (gecmis_.dolu()) toplam_ -= gecmis_.bak(gecmis_.adet() - 1);   // çıkan
    gecmis_.ekle(x);
    toplam_ += x;                                                     // giren
    return static_cast<float>(toplam_ / gecmis_.adet());
}
```

**Medyan filtre** (*median filter*) · 5.5
Son N örneğin medyanını (sıralandığında ortadaki değer) veren doğrusal olmayan filtre.

```cpp
MedyanFiltre m(3);
m.isle(1.0f);      // {1}         → 1
m.isle(100.0f);    // {1, 100}    → 50.5
m.isle(2.0f);      // {1, 100, 2} → 2   (sıçrama yok oldu)
```

**FIR filtre** (*finite impulse response*) · 5.6
Çıkışı son N girişin ağırlıklı toplamı olan filtre: `y[n] = Σ b[k]·x[n−k]`.

```cpp
float FIRFiltre::isle(float x)
{
    gecmis_.ekle(x);
    double y = 0.0;
    for (size_t k = 0; k < n_; k++) y += b_[k] * gecmis_.bak(k);
    return static_cast<float>(y);
}
```

**Katsayılar** (*coefficients, taps*) · 5.6
FIR filtredeki `b[k]` ağırlıkları. Katsayı sayısına filtre uzunluğu (*tap* sayısı) denir.

```cpp
float b[3] = {0.25f, 0.5f, 0.25f};   // 3 tap'lik yumuşatıcı
FIRFiltre f(b, 3);
f.katsayi_sayisi();                  // 3
```

**Dürtü yanıtı** (*impulse response*) · 5.6
Filtreye tek bir birim dürtü (bir `1`, ardından sıfırlar) verildiğinde alınan çıkış. FIR filtrede dürtü yanıtı katsayıların kendisidir.

```cpp
float b[3] = {0.5f, 0.25f, 0.25f};
FIRFiltre f(b, 3);
f.isle(1.0f);   // 0.5
f.isle(0.0f);   // 0.25
f.isle(0.0f);   // 0.25
f.isle(0.0f);   // 0     (N örnek sonra bitti)
```

**Pencereli sinc yöntemi** (*windowed-sinc method*) · 5.6
İdeal alçak geçiren filtrenin sonsuz uzunluktaki sinc dürtü yanıtını N örnekte kesip bir pencere fonksiyonuyla çarparak FIR katsayısı tasarlama yöntemi.

```cpp
double m = i - (n - 1) / 2.0;
double h = (m == 0.0) ? 2.0 * fn : std::sin(2.0 * PI * fn * m) / (PI * m);   // sinc
katsayilar[i] = h * w;                                                         // pencereyle çarp
```

**Hamming penceresi** (*Hamming window*) · 5.6
`w[i] = 0.54 − 0.46·cos(2πi / (N−1))`. Sinc'i kesmenin oluşturduğu dalgalanmayı azaltmak için kullanılır.

```cpp
double w = 0.54 - 0.46 * std::cos(2.0 * PI * i / (n - 1));
```

**DC kazancı** (*DC gain*) · 5.6
Filtrenin sabit (0 Hz) bir girişi kaç katına çıkardığı. FIR'da katsayıların toplamına eşittir.

```cpp
for (size_t i = 0; i < n; i++) katsayilar[i] /= toplam;   // toplam 1 olsun
// Artık sabit 2.0 girişi, filtre oturduktan sonra 2.0 olarak çıkar.
```

**IIR filtre** (*infinite impulse response*) · 5.7
Çıkışı girişlerin yanında önceki çıkışlara da bağlı olan (geri beslemeli) filtre. Dürtü yanıtı teorik olarak sonsuza kadar sürer.

```cpp
IIRAlcakGeciren f(0.5f);
f.isle(1.0f);   // 0.5
f.isle(0.0f);   // 0.25
f.isle(0.0f);   // 0.125   ... yarılanarak sürer, hiç tam sıfır olmaz
```

**Birinci dereceden alçak geçiren IIR** · 5.7
`y[n] = y[n−1] + α·(x[n] − y[n−1])`. Analog RC alçak geçiren devresinin ayrık zaman karşılığıdır. Kesim frekansından `α = 1 − e^(−2π·fc/fs)` ile hesaplanır.

```cpp
float IIRAlcakGeciren::isle(float x)
{
    y_ = y_ + alfa_ * (x - y_);
    return y_;
}
```

**Kesim frekansı** (*cutoff frequency, fc*) · 5.6, 5.7
Alçak geçiren bir filtrenin geçirdiği ve bastırdığı frekansları ayıran sınır frekansı.

```cpp
fir_alcak_geciren_tasarla(katsayilar, 61, 15.0f, FS);            // FIR, fc = 15 Hz
IIRAlcakGeciren iir(IIRAlcakGeciren::alfa_hesapla(10.0f, FS));   // IIR, fc = 10 Hz
```

**Gecikme** (*delay, group delay*) · 5.9
Filtre çıkışının girişe göre kaç örnek geriden geldiği.

```cpp
size_t gecikme() const override { return (n_ - 1) / 2; }                  // FIR: 61 tap → 30
size_t gecikme() const override { return gecmis_.kapasite() / 2; }        // Medyan: 15 → 7
```

**Doğrusal faz** (*linear phase*) · 5.6
Bütün frekansların aynı süre geciktirilmesi. Simetrik katsayılı FIR filtreler doğrusal fazlıdır, IIR filtreler değildir.

```cpp
// Simetrik katsayılar: b[k] == b[N-1-k]
float b[5] = {0.1f, 0.2f, 0.4f, 0.2f, 0.1f};   // her frekans tam 2 örnek gecikir
```

**Filtre zinciri** (*cascade*) · 5.8
Filtrelerin arka arkaya bağlanması; her filtrenin çıkışı bir sonrakinin girişidir. Toplam gecikme, halkaların gecikmelerinin toplamıdır.

```cpp
MedyanFiltre zincir_medyan(5);           // gecikme 2
HareketliOrtalama zincir_ortalama(21);   // gecikme 10
FiltreZinciri zincir;
zincir.ekle(&zincir_medyan);
zincir.ekle(&zincir_ortalama);
zincir.gecikme();                        // 12
```

**RMS hata** (*root mean square error*) · 5.9
İki dizi arasındaki farkın karelerinin ortalamasının karekökü: `√( (1/n)·Σ (a[i] − b[i])² )`.

```cpp
float x[4] = {1, 2, 3, 4};
float y[4] = {1, 2, 3, 6};
rms_fark(x, y, 4);     // sqrt((0 + 0 + 0 + 4) / 4) = 1.0
```

**Geçici rejim** (*transient*) · 5.9
Filtrenin başlangıçta geçmişi henüz dolmamışken ürettiği, kalıcı davranışını yansıtmayan çıkış.

```cpp
constexpr size_t ATLA = 100;
rms_fark(temiz, cikis, N, ATLA);   // ilk 100 örnek ölçüme katılmıyor
```

**Rastgele sayı tohumu** (*seed*) · 5.9
Sözde rastgele sayı üretecinin başlangıç değeri. Aynı tohum her çalıştırmada aynı diziyi üretir.

```cpp
SinyalUreteci u1(100.0f, 7), u2(100.0f, 7);
u1.gurultu_ekle(a, 50, 1.0f);
u2.gurultu_ekle(b, 50, 1.0f);    // a ve b bit bit aynı
```
