# Gün 2 | Nesne Yönelimli Programlama ve Dijital Filtre Projesi

**Tarih:** 12 Kasım 2026
**Süre:** En fazla 4 saat (aralar dahil)

Bu sayfa ikinci günün ders notudur. Birinci günde dili ve belleği tanıdık, hafta boyunca da ödevlerde register'larla, ADC'lerle, ring buffer'la ve filtrelerle uğraştınız. Bugün bu parçaları **sınıflar** yardımıyla bir araya getirip küçük ama gerçek bir **dijital filtre kütüphanesi** yazacağız.

Bugünün asıl derdi yeni bir sözdizimi öğrenmek değil, kodu nasıl **düzenlediğimiz**. Yazacağımız şeylerin çoğunu zaten biliyorsunuz. Değişen, onları nereye ve hangi kurallarla koyduğumuz.

## Günün Akışı

| Süre | Bölüm | Klasör | Hedef |
|:---:|---|---|---|
| 10 dk | Açılış ve kısa tekrar | | Pointer, `->`, `new` ve `delete` hafızamızı tazelemek |
| 35 dk | Sınıflar ve nesneler | [01_oop_temelleri](01_oop_temelleri) | `class`, üye fonksiyon, `this`, yapıcı, yıkıcı |
| 10 dk | Ara | | |
| 30 dk | Kapsülleme | [01_oop_temelleri](01_oop_temelleri) | Ring buffer'ı sınıfa taşımak, başlık ve kaynak dosyaya bölmek |
| 25 dk | RAII | [01_oop_temelleri](01_oop_temelleri) | `delete` derdinden kurtulmak, kopyalama tuzağı |
| 10 dk | Ara | | |
| 40 dk | Kalıtım ve çok biçimlilik | [01_oop_temelleri](01_oop_temelleri) | `virtual`, soyut sınıf, taban sınıf pointer'ı |
| 10 dk | Ara | | |
| 60 dk | Final projesi | [02_filtre_kutuphanesi](02_filtre_kutuphanesi) | Hareketli ortalama, medyan, FIR, IIR ve filtre zinciri |
| 10 dk | Kapanış | | Hâsılı ve bundan sonrası |

Sıkışırsak nereden kısaltacağımız sayfanın sonundaki [Eğitmen notu](#eğitmen-notu) kısmında yazıyor.

Bütün demo dosyaları tek başına derlenir:

```bash
cd 03_gun2_oop_filtre/01_oop_temelleri
g++ -std=c++17 -Wall -Wextra 01_struct_class.cpp -o struct_class
./struct_class
```

## 0. Açılış ve Kısa Tekrar (10 dk)

Bugün sık sık kullanacağımız üç şeyi hatırlayalım.

**Pointer ve `->`:** Ödev 03'te fonksiyonlarınız `RingBuffer* rb` alıyordu ve alanlara `rb->bas` diye erişiyordunuz. `rb->bas` yazmak `(*rb).bas` yazmakla aynı şeydir: "pointer'ın gösterdiği nesnenin `bas` alanı".

**Referans:** Bir değişkene takılan ikinci isimdir. Pointer gibi kopyalamadan iş görür, lakin `nullptr` olamaz ve `*` ile uğraştırmaz.

```cpp
void sifirla(int& x) { x = 0; }   // x, çağıranın değişkeninin ta kendisi
int a = 5;
sifirla(a);                       // a artık 0
```

**`new` ve `delete`:** Heap'ten aldığınızı geri vermek sizin sorumluluğunuzdadır. `new[]` ile alınan `delete[]` ile verilir. Bugünün ortasında bu sorumluluğu bir nesneye devredeceğiz.

Ödevlerin bugün neye dönüşeceğine bir daha bakalım:

| Ödev | Bugün neye dönüşüyor? | Nerede? |
|---|---|---|
| 02 ADC | `Adc` sınıfı ve `SinyalUreteci` | [02_yapici_yikici.cpp](01_oop_temelleri/02_yapici_yikici.cpp), [sinyal.hpp](02_filtre_kutuphanesi/include/sinyal.hpp) |
| 03 Ring Buffer | `RingBuffer` sınıfı, filtrelerin geçmiş deposu | [ring_buffer.hpp](02_filtre_kutuphanesi/include/ring_buffer.hpp) |
| 04 Filtre | `HareketliOrtalama` ve `MedyanFiltre` sınıfları | [filtre.hpp](02_filtre_kutuphanesi/include/filtre.hpp) |

> **Not:** Ödevlerin son teslim tarihi geçtiği için bugünkü kodlarda ödev çözümlerinin sınıf hâllerini açıkça göreceksiniz. Kendi çözümünüzle karşılaştırın, farklı yaptığınız yerleri not edin.

## 1. Sınıflar ve Nesneler (35 dk)

### 1.1 Neden sınıf? (8 dk)

Demo dosyası: [`01_struct_class.cpp`](01_oop_temelleri/01_struct_class.cpp)

Ödev 03'teki düzeni hatırlayın: verinin kendisi bir `struct` içinde, onu kullanan fonksiyonlar dışarıda, ve her fonksiyona hangi tampon üzerinde çalışacağını pointer ile söylüyoruz. Bu düzen çalışıyor, lakin iki sıkıntısı var:

1. **Veri ile onu kullanan fonksiyonlar arasında bir bağ yok.** `rb_ekle` fonksiyonunun `RingBuffer` ile ilgili olduğunu sadece isminden anlıyoruz.
2. **Herkes her alana dokunabiliyor.** Biri `rb.adet = 50` yazarsa derleyici ses çıkarmaz, tampon sessizce bozulur.

Demo dosyasında bunu bir sayaç üzerinde görüyoruz. Mikrodenetleyicideki bir zamanlayıcı gibi `ust_sinir` değerine ulaşınca başa dönüyor:

```cpp
SayacStruct s = {0, 3};
s.deger = 1000;        // kimse bizi durdurmuyor
sayac_artir(&s);       // 1001, 1002, ... artık hiç başa dönmeyecek
```

**Sınıf**, veriyi ve o veri üzerinde çalışan fonksiyonları tek bir çatı altında toplar. Sınıftan oluşturulan her somut örneğe **nesne** (object) diyoruz. Sınıf bir kalıp, nesne o kalıptan dökülen parça.

```cpp
class Sayac {
public:
    void ayarla(int ust_sinir) { deger_ = 0; ust_sinir_ = ust_sinir; }
    void artir()
    {
        if (deger_ == ust_sinir_) deger_ = 0;
        else deger_++;
    }
    int deger() const { return deger_; }

private:
    int deger_ = 0;
    int ust_sinir_ = 0;
};

Sayac a;          // a bir nesne
a.ayarla(3);
a.artir();        // sayac_artir(&a) yerine a.artir()
```

Üye değişkenlerin sonundaki `_` bir zorunluluk değil, bir gelenek. Kod okurken "bu bir üye değişken" diye hemen anlaşılsın diye.

### 1.2 `public`, `private` ve `struct` ile `class` farkı (5 dk)

* `public:` altındakileri herkes kullanabilir. Buna sınıfın **arayüzü** diyoruz.
* `private:` altındakilere yalnızca sınıfın kendi üye fonksiyonları dokunabilir.

```cpp
a.deger_ = 1000;   // DERLENMEZ: 'int Sayac::deger_' is private within this context
```

Peki `struct` ile `class` arasındaki fark ne? C++'ta **tek fark** varsayılan erişimdir: `struct` içinde her şey baştan `public`, `class` içinde baştan `private`. Geri kalan her özellik ikisinde de aynı. Gelenek şöyle: sadece veri taşıyan basit yapılar için `struct`, kuralı ve davranışı olan şeyler için `class`.

### 1.3 Üye fonksiyonlar ve `this` (5 dk)

`a.artir()` dediğimizde `artir` fonksiyonu hangi sayacın `deger_` alanını artıracağını nereden biliyor? Her üye fonksiyona, gizli bir parametre olarak, çağrıldığı nesnenin adresi gelir. Bu adresin adı `this`.

```cpp
void artir() { deger_++; }          // biz böyle yazıyoruz
void artir() { this->deger_++; }    // derleyici böyle anlıyor
```

Yani aslında Ödev 03'teki `rb_ekle(RingBuffer* rb, ...)` fonksiyonundan çok da farklı değil: `rb` parametresi gitti, yerine `this` geldi ve onu bizim yazmamıza gerek kalmadı. Demo dosyası `&a` ile `a` içindeki `this` değerini yan yana yazdırıyor, aynı çıkıyorlar.

Bir başka önemli nokta: aynı sınıftan iki nesne oluşturduğunuzda **veriler ayrı, fonksiyonlar ortaktır**. `a` ile `b`'nin ayrı `deger_` alanları var, ama `artir` fonksiyonunun makine kodu bellekte tek bir yerde duruyor.

### 1.4 `const` üye fonksiyonlar (3 dk)

```cpp
int deger() const { return deger_; }
```

Sondaki `const`, "bu fonksiyon nesneyi değiştirmez" sözüdür. Fonksiyonun içinde bir üyeyi değiştirmeye kalkarsanız derleyici yakalar. Daha önemlisi, elinizde bir `const` nesne ya da `const` referans varsa üzerinden **yalnızca** `const` fonksiyonları çağırabilirsiniz. Bunu 2. bölümde kullanacağız. Kural basit: nesneyi değiştirmeyen her üye fonksiyonun sonuna `const` yazın.

> **Sıra sizde (3 dk):** `Sayac` sınıfına `sifirla()` adında, sayacı 0'a çeken bir fonksiyon ekleyin. Sonra aynı fonksiyonun sonuna `const` ekleyip derleyin. Hata mesajı ne diyor?

### 1.5 Yapıcı (constructor) ve ilk değer listesi (8 dk)

Demo dosyası: [`02_yapici_yikici.cpp`](01_oop_temelleri/02_yapici_yikici.cpp)

`Sayac` sınıfında bir sorun var: nesneyi oluşturup `ayarla` çağırmayı unutursak sınır 0 kalıyor. "Kullanmadan önce şunu çağırmayı unutma" türünden kurallar mutlaka unutulur. **Yapıcı**, nesne doğarken kendiliğinden çalışan özel bir fonksiyondur. Sınıfla aynı isimde olur ve dönüş tipi yoktur.

Ödev 02'deki ADC fonksiyonlarını hatırlayın: her çağrıda `vref` ve `bit` değerlerini tekrar tekrar veriyordunuz. Bir sınıf bunları bir kere öğrenip hatırlayabilir:

```cpp
class Adc {
public:
    Adc(double vref, unsigned bit)
        : vref_(vref), bit_(bit), lsb_(0.0)      // ilk değer listesi
    {
        if (bit_ < 1 || bit_ > 16) bit_ = 12;    // geçersiz nesne hiç doğmasın
        if (vref_ <= 0.0) vref_ = 3.3;
        lsb_ = vref_ / (1u << bit_);
    }

    Adc() : Adc(3.3, 12) {}                      // işi diğer yapıcıya devrediyor

    uint16_t kodla(double volt) const;
    double coz(uint16_t kod) const;

private:
    double   vref_;
    unsigned bit_;
    double   lsb_;
};

Adc a(3.3, 10);      // yapıcı burada çalışır
Adc b;               // parametresiz yapıcı
```

Birkaç önemli nokta:

* **İlk değer listesi** (`:` ile başlayan kısım), üyeleri gövde çalışmadan **önce** kurar. Gövdenin içinde `vref_ = vref;` yazmak da çalışır, lakin o durumda üye önce kurulur, sonra üzerine yazılır. Bazı üyeler (referanslar, `const` üyeler, kendi yapıcısı parametre isteyen nesneler) **yalnızca** ilk değer listesinde kurulabilir. Alışkanlık edinin.
* Yapıcının asıl görevi nesnenin **geçerli bir durumda doğmasını** garanti etmektir. 99 bitlik bir ADC istenirse yapıcı onu düzeltiyor, artık sınıfın geri kalanı "bit sayısı geçerli mi" diye kontrol etmek zorunda değil.
* Hiç yapıcı yazmazsanız derleyici sizin için boş bir parametresiz yapıcı yazar. Bir tane bile yazarsanız artık yazmaz.

### 1.6 Yıkıcı (destructor) ve nesnenin ömrü (6 dk)

**Yıkıcı**, nesne ölürken kendiliğinden çalışan fonksiyondur. Başında `~` vardır, parametre almaz.

```cpp
~Adc() { std::cout << "ADC yok oldu\n"; }
```

Peki nesne ne zaman ölür?

| Nerede oluşturuldu? | Ne zaman ölür? |
|---|---|
| Stack (yerel değişken) | Tanımlandığı blok `}` ile bitince |
| Heap (`new` ile) | Siz `delete` dediğinizde (demezseniz hiç) |
| Global ya da `static` | Program biterken |

Demo dosyasını çalıştırıp çıktıyı satır satır takip edin. Bilhassa şuna dikkat edin: aynı blokta oluşturulan nesneler **doğuşun tersi sırayla** yıkılır. Önce `b`, sonra `c` doğduysa önce `c`, sonra `b` ölür. Tabağı üst üste dizip en üsttekini önce almak gibi. Bunun sebebi, sonra doğan nesnenin öncekine bel bağlamış olabilmesidir.

> **Sıra sizde (3 dk):** `main` içine bir `for` döngüsü ekleyin ve döngünün gövdesinde bir `Adc` nesnesi oluşturun. Döngü üç tur dönerse yapıcı ve yıkıcı kaçar kez çalışır? Önce tahmin edin, sonra çalıştırıp bakın.

## Ara (10 dk)

## 2. Kapsülleme (30 dk)

### 2.1 Ring buffer'ı sınıfa taşımak (10 dk)

Demo dosyası: [`03_kapsulleme.cpp`](01_oop_temelleri/03_kapsulleme.cpp)

**Kapsülleme** (encapsulation), bir şeyin iç yapısını gizleyip dışarıya sadece nasıl kullanılacağını göstermektir. Bir mikrodenetleyicinin datasheet'i gibi: hangi register'a ne yazacağınızı söyler, silikonun içinde transistörlerin nasıl dizildiğini değil.

Ödev 03'teki fonksiyonlar sınıfın içine şöyle taşınıyor:

| Ödev 03 (struct ve fonksiyon) | Sınıf hâli |
|---|---|
| `RingBuffer rb; rb_baslat(&rb);` | `RingBuffer rb;` (yapıcı hallediyor) |
| `rb_ekle(&rb, 3.0f)` | `rb.ekle(3.0f)` |
| `rb_ekle_ezerek(&rb, 3.0f)` | `rb.ekle_ezerek(3.0f)` |
| `rb_cikar(&rb, &x)` | `rb.cikar(x)` (pointer yerine referans) |
| `rb_bak(&rb, 0, &x)` | `rb.bak(0, x)` |
| `rb_adet(&rb)`, `rb_bos(&rb)`, `rb_dolu(&rb)` | `rb.adet()`, `rb.bos()`, `rb.dolu()` |
| `rb.bas = 5;` (derlenir, tampon bozulur) | `rb.bas_ = 5;` (**derlenmez**) |

Bir şeye dikkat edin: `nullptr` kontrollerinin hepsi gitti. Bir nesnenin üye fonksiyonunu çağırıyorsanız elinizde o nesne var demektir.

### 2.2 Değişmez (invariant) (5 dk)

Ring buffer'ın üç alanı birbirine bağlı: `bas_` her zaman kapasiteden küçük olmalı, `adet_` kapasiteyi geçmemeli. Bu tür "her zaman doğru olması gereken" kurallara **değişmez** (invariant) diyoruz.

Alanlar `public` olsaydı bu kuralı programdaki **her satır** bozabilirdi ve bir hata çıktığında suçluyu bütün projede arardık. `private` olunca kuralı bozabilecek yalnızca sınıfın kendi fonksiyonları kalıyor. Hata ararken bakacağımız yer bir anda bir avuç fonksiyona iniyor. Kapsüllemenin asıl kazancı budur. Verileri "saklamak" bir amaç değil, değişmezleri korumanın yolu.

Bu yüzden her alana otomatik olarak bir `get` ve bir `set` fonksiyonu yazmayın. `set_adet(size_t)` diye bir fonksiyon açarsanız alanı `public` yapmaktan farkı kalmaz. Sınıfın arayüzü **ne yaptığını** anlatmalı (`ekle`, `cikar`), **neyi sakladığını** değil.

### 2.3 `const` referans ile parametre almak (5 dk)

```cpp
void yazdir(const RingBuffer& rb)
{
    for (size_t i = 0; i < rb.adet(); i++) { ... }
    // rb.ekle(1.0f);   // DERLENMEZ: ekle const değil
}
```

Nesneyi fonksiyona değer olarak (`RingBuffer rb`) verseydik bütün tampon kopyalanırdı. Pointer ile verseydik `nullptr` kontrolü gerekirdi. `const RingBuffer&` hem kopyalamaz hem de "değiştirmeyeceğim" sözü verir. Bu sözün tutulduğunu derleyici denetler: içeride yalnızca `const` üye fonksiyonları çağırabilirsiniz. 1.4'teki "değiştirmeyen her fonksiyona `const` yazın" kuralının sebebi tam olarak bu.

Genel kural: **küçük tipleri** (`int`, `float`, `bool`) değer olarak, **nesneleri** `const&` ile, **değiştirilecek nesneleri** `&` ile alın.

### 2.4 Sınıfı başlık ve kaynak dosyaya bölmek (10 dk)

Demo dosyalarında her şeyi tek dosyaya yazdık. Gerçek projede, ödevlerde de gördüğünüz gibi, ikiye bölüyoruz:

* **Başlık dosyası** (`.hpp`): Sınıfın neye benzediği. Arayüz, yani datasheet.
* **Kaynak dosyası** (`.cpp`): Fonksiyonların gövdeleri.

```cpp
// ring_buffer.hpp
#pragma once
#include <cstddef>

class RingBuffer {
public:
    explicit RingBuffer(size_t kapasite);
    void ekle(float deger);
    size_t adet() const { return adet_; }     // tek satırlıklar başlıkta kalabilir
    ...
private:
    size_t kapasite_;
    float* veri_;
    size_t bas_;
    size_t adet_;
};
```

```cpp
// ring_buffer.cpp
#include "ring_buffer.hpp"

void RingBuffer::ekle(float deger)        // RingBuffer:: "bu, RingBuffer'ın ekle fonksiyonu"
{
    if (dolu()) {
        veri_[bas_] = deger;
        bas_ = (bas_ + 1) % kapasite_;
    } else {
        veri_[(bas_ + adet_) % kapasite_] = deger;
        adet_++;
    }
}
```

`RingBuffer::` öneki olmadan yazarsanız derleyici bunu sınıfla ilgisi olmayan, `ekle` adında serbest bir fonksiyon sanar ve `bas_` diye bir şey bilmediğini söyler. Yeni başlayanların en sık yaptığı hata budur.

`#pragma once` ise başlığın bir derleme içinde iki kez eklenmesini engeller. İki kez eklenirse derleyici "bu sınıf zaten tanımlı" diye itiraz eder.

> **Sıra sizde (5 dk):** `03_kapsulleme.cpp` dosyasındaki `RingBuffer` sınıfına, tampondaki örneklerin ortalamasını döndüren `float ortalama() const` fonksiyonunu ekleyin. Boşsa 0 döndürsün. Bu fonksiyon için `bas_` ve `adet_` alanlarına doğrudan mı erişmek daha iyi, yoksa `bak()` fonksiyonunu mu kullanmak? Neden?

## 3. RAII (25 dk)

Demo dosyası: [`04_raii.cpp`](01_oop_temelleri/04_raii.cpp)

### 3.1 Problem: `delete` yazmayı unutmak (5 dk)

Ödev 03'te kapasite derleme anında sabitti (`RB_KAPASITE = 8`). Filtre kütüphanesinde ise her filtre farklı uzunlukta bir geçmişe ihtiyaç duyacak: 5'lik bir medyan, 61 katsayılı bir FIR. Yani belleği çalışma anında `new` ile almamız lazım. Gün 1'den hatırlarsınız, `new` ile alınanı bir yerde `delete` ile geri vermek gerekiyor. Mesele şu ki fonksiyonların birden fazla çıkış kapısı olabiliyor:

```cpp
bool olcum_eski(size_t n, bool sensor_hatasi)
{
    float* tampon = new float[n];
    if (sensor_hatasi) {
        return false;            // SIZINTI: tampon hiç silinmedi!
    }
    ...
    delete[] tampon;
    return true;
}
```

Her `return` öncesinde `delete[]` yazmayı hatırlamak zorundasınız. Fonksiyona üç ay sonra yeni bir `return` ekleyen kişi (ekseriya siz) bunu hatırlamayacak.

### 3.2 Çözüm: Kaynağı bir nesneye emanet etmek (7 dk)

Yıkıcının **kesinlikle** çalıştığını biliyoruz: nesne hangi kapıdan çıkarsa çıksın, blok bitince ölür. O hâlde belleği yapıcıda alıp yıkıcıda geri veren bir sınıf yazarsak `delete` yazmayı unutmak imkânsız hâle gelir.

```cpp
class DinamikTampon {
public:
    explicit DinamikTampon(size_t n) : veri_(new float[n]), boyut_(n) {}
    ~DinamikTampon() { delete[] veri_; }
    ...
private:
    float* veri_;
    size_t boyut_;
};

bool olcum_yeni(size_t n, bool sensor_hatasi)
{
    DinamikTampon tampon(n);
    if (sensor_hatasi) {
        return false;            // tampon'un yıkıcısı yine çalışır
    }
    ...
    return true;
}
```

Bu tekniğin adı **RAII**: *Resource Acquisition Is Initialization*, yani "kaynağı edinmek, nesneyi kurmaktır". İsmi pek başarılı değil, fikri ise C++'ın belki de en önemli fikri. Kaynak sadece bellek olmak zorunda değil: açık bir dosya, bir kilit (mutex), bir donanım çevre birimi... Hepsi için aynı kalıp geçerli: **yapıcıda al, yıkıcıda bırak.**

Standart kütüphanedeki `std::vector`, `std::string` ve `std::unique_ptr` tam olarak bunu yapıyor. Gün 1'de `std::vector` kullanırken `delete` yazmamanızın sebebi buydu.

### 3.3 Kopyalama tuzağı (8 dk)

Şimdi tehlikeli bir soru: `DinamikTampon` nesnesini kopyalarsak ne olur?

```cpp
DinamikTampon a(10);
DinamikTampon b = a;     // ne olur?
```

Kendimiz yazmadıysak derleyici bizim için bir kopyalama yazar ve bu kopyalama üyeleri tek tek kopyalar. `veri_` bir pointer, yani kopyalanan şey **adres**, gösterdiği bellek değil:

```
  a.veri_ ──┐
            ├──▶ [ 10 float ]
  b.veri_ ──┘
```

İki nesne aynı belleği gösteriyor. `main` bitince önce `b` ölür ve belleği siler, sonra `a` ölür ve **aynı belleği tekrar** siler. Çift silme (double free) tanımsız davranıştır, ekseriya program çöker. Daha kötüsü, bazen çökmez.

İki çözüm var:

1. **Düzgün bir kopyalama yazmak:** Yeni bellek alıp içeriği kopyalayan bir kopyalama yapıcısı ve atama operatörü.
2. **Kopyalamayı yasaklamak:** Kopyalanması anlamsız olan şeyler için en temiz yol bu.

```cpp
DinamikTampon(const DinamikTampon&) = delete;              // kopyalama yapıcısı yok
DinamikTampon& operator=(const DinamikTampon&) = delete;   // kopyalayarak atama yok
```

Artık `DinamikTampon b = a;` satırı **derlenmez**. Hatayı çalışma anında çökme olarak değil, derleme anında görmek her zaman daha iyidir.

Akılda tutulacak kural, **üçler kuralı** (rule of three): Bir sınıfa yıkıcı yazmanız gerekiyorsa büyük ihtimalle kopyalama yapıcısı ve atama operatörü de yazmanız (ya da yasaklamanız) gerekir. Üçü birlikte gelir.

### 3.4 `explicit` (2 dk)

Tek parametre alan yapıcılar, derleyiciye o tipten bu sınıfa sessizce dönüşüm yapma izni verir:

```cpp
DinamikTampon t = 50;    // explicit olmasaydı derlenirdi: 50 elemanlı tampon!
```

Bu ekseriya istenmeyen bir şeydir. Tek parametreli yapıcıların başına `explicit` yazın.

### 3.5 Bileşim: RAII nesnesini üye yapmak (3 dk)

İşin en güzel kısmı burası. Final projesindeki hareketli ortalama filtresine bakın:

```cpp
class HareketliOrtalama : public Filtre {
    ...
private:
    RingBuffer gecmis_;      // belleği RingBuffer yönetiyor
    double toplam_;
};
```

`HareketliOrtalama` dolaylı olarak heap'ten bellek kullanıyor, lakin ne yıkıcı yazdık ne de `delete`. Zira nesne ölürken üyelerinin yıkıcıları da kendiliğinden çalışır. Bellek yönetimini **bir kere**, `RingBuffer` içinde doğru yaptık ve onu kullanan herkes bedavaya kazandı. Bir sınıfın başka bir sınıfı üye olarak içermesine **bileşim** (composition) diyoruz.

Hedefimiz şu olmalı: `new` ve `delete` yalnızca birkaç küçük RAII sınıfının içinde geçsin, geri kalan kodda hiç görünmesin.

> **Sıra sizde (3 dk):** `04_raii.cpp` dosyasında kopyalamayı yasaklayan iki satırı yorum satırına çevirin ve `main` içine `DinamikTampon a(10); DinamikTampon b = a;` ekleyin. Derleyip çalıştırın. Yıkıcının çıktısında ne görüyorsunuz? Program nasıl bitiyor?

## Ara (10 dk)

## 4. Kalıtım ve Çok Biçimlilik (40 dk)

### 4.1 Kalıtım (10 dk)

Demo dosyası: [`05_kalitim.cpp`](01_oop_temelleri/05_kalitim.cpp)

Bir projede sıcaklık, ışık, gerilim sensörleri olsun. Hepsinin bir adı ve kaç kez okunduğunu tutan bir sayacı var. Bunları her sınıfta tekrar tekrar yazmak yerine ortak kısmı bir **taban sınıfa** (base class) koyup diğerlerini ondan **türetebiliriz** (derived class).

```cpp
class Sensor {
public:
    explicit Sensor(const std::string& ad) : ad_(ad), okuma_sayisi_(0) {}
    const std::string& ad() const { return ad_; }
protected:
    std::string ad_;
    int okuma_sayisi_;
};

class SicaklikSensoru : public Sensor {       // "SicaklikSensoru bir Sensor'dür"
public:
    SicaklikSensoru(const std::string& ad, double ofset)
        : Sensor(ad), ofset_(ofset) {}         // önce taban sınıf kurulur
    double oku(int ham_kod)
    {
        okuma_sayisi_++;                       // protected: türetilmiş sınıf erişebilir
        return ham_kod * 3.3 / 4095.0 * 100.0 + ofset_;
    }
private:
    double ofset_;
};
```

* `SicaklikSensoru`, `Sensor`'ün bütün `public` fonksiyonlarını miras alır. `oda.ad()` çağırabiliriz, hiç yazmadığımız hâlde.
* `protected:` yeni bir erişim seviyesi. Dışarıya kapalı, lakin türetilmiş sınıflara açık.
* Türetilmiş nesne kurulurken **önce taban sınıfın** yapıcısı çalışır. Taban sınıfın yapıcısı parametre istiyorsa ilk değer listesinde onu çağırmak zorundayız.
* Yıkım ters sırayla olur: önce türetilmiş, sonra taban. Demo çıktısında görebilirsiniz.

Kalıtım bir **"bir ... dır" (is-a)** ilişkisidir. Sıcaklık sensörü **bir** sensördür. Bunu bileşimden (3.5) ayırmak önemli: hareketli ortalama filtresi bir ring buffer **değildir**, bir ring buffer'a **sahiptir** (has-a). O yüzden `RingBuffer`'dan türetmedik, onu üye yaptık. Emin değilseniz bileşimi seçin, ekseriya o doğrudur.

### 4.2 Problem: Taban sınıf pointer'ı (5 dk)

Türetilmiş bir nesneyi taban sınıf tipinde bir pointer ile gösterebiliriz. Ne de olsa sıcaklık sensörü bir sensördür:

```cpp
SicaklikSensoru oda("oda", -0.5);
Sensor* s = &oda;
std::cout << s->birim();    // "C" bekliyoruz, "?" yazıyor!
```

Her iki sınıfta da `birim()` fonksiyonu var. Derleyici `s->birim()` satırını derlerken sadece pointer'ın tipine, yani `Sensor*` kısmına bakıyor ve `Sensor::birim` fonksiyonunu çağırıyor. `s`'nin gerçekte bir sıcaklık sensörünü gösterdiğini bilmiyor. Buna **statik bağlama** diyoruz: karar derleme anında veriliyor.

### 4.3 `virtual` ve `override` (8 dk)

Demo dosyası: [`06_sanal_fonksiyonlar.cpp`](01_oop_temelleri/06_sanal_fonksiyonlar.cpp)

Taban sınıftaki fonksiyonun başına `virtual` yazarsak karar çalışma anına kalır ve pointer'ın değil **nesnenin gerçek tipine** göre verilir:

```cpp
class Sensor {
public:
    virtual const char* birim() const { return "?"; }
};

class SicaklikSensoru : public Sensor {
public:
    const char* birim() const override { return "C"; }
};

Sensor* s = &oda;
s->birim();      // artık "C"
```

`override` yazmak zorunlu değil, lakin mutlaka yazın. Taban sınıftaki imzayla birebir uyuşmayan bir fonksiyon yazarsanız (sondaki `const`'u unutmak gibi) `override` olmadan derleyici bunu yepyeni bir fonksiyon sanar ve sessizce taban sınıfınki çağrılmaya devam eder. `override` ile ise derleme hatası alırsınız.

Perde arkasında ne oluyor? Sanal fonksiyonu olan her nesnenin içinde gizli bir pointer bulunur ve bu pointer, o sınıfın sanal fonksiyonlarının adreslerini tutan bir tabloyu (vtable) gösterir. `s->birim()` çağrısı "tablodaki `birim` satırına git, orada hangi adres yazıyorsa onu çağır" demektir. Bedeli nesne başına bir pointer ve çağrı başına bir dolaylı adresleme. Bir masaüstü bilgisayarda yok denecek kadar küçük, çok küçük bir mikrodenetleyicide ise hesaba katılabilecek bir maliyet.

### 4.4 Saf sanal fonksiyon ve soyut sınıf (5 dk)

"Bilinmeyen bir sensörün birimi nedir?" sorusunun anlamlı bir cevabı yok. O zaman taban sınıfta hiç gövde yazmayalım:

```cpp
class Sensor {
public:
    virtual ~Sensor() = default;
    virtual double oku(int ham_kod) = 0;       // saf sanal (pure virtual)
    virtual const char* birim() const = 0;
};
```

Sonundaki `= 0`, "bu fonksiyonun gövdesi yok, türetilen her sınıf yazmak zorunda" demektir. En az bir saf sanal fonksiyonu olan sınıfa **soyut sınıf** (abstract class) denir ve ondan doğrudan nesne oluşturulamaz:

```cpp
Sensor s;     // DERLENMEZ: cannot declare variable 's' to be of abstract type 'Sensor'
```

Soyut sınıfı bir **sözleşme** gibi düşünün. "Sensör olmak isteyen herkes `oku` ve `birim` fonksiyonlarını sağlamak zorunda." Final projesinde `Filtre` sınıfı tam olarak böyle bir sözleşme.

### 4.5 Çok biçimlilik: Hepsine aynı soruyu sormak (5 dk)

Şimdi asıl gücü görelim:

```cpp
SicaklikSensoru t;
IsikSensoru i;
GerilimSensoru g;

Sensor* sensorler[] = {&t, &i, &g};
for (int k = 0; k < 3; k++) {
    sensorler[k]->rapor(ham_kodlar[k]);
}
```

Farklı tipteki nesneleri aynı dizide tutup hepsine aynı soruyu soruyoruz, her biri kendi bildiği gibi cevap veriyor. Buna **çok biçimlilik** (polymorphism) diyoruz.

Bu döngünün en değerli tarafı şu: projeye yarın bir nem sensörü eklemek istesek döngüye **hiç dokunmamız gerekmiyor**. Yeni sınıfı yazıp diziye eklemek kâfi. Değişmeyen kod, bozulmayan koddur.

Bir de `rapor` fonksiyonuna bakın: kendisi sanal değil, ama içinde sanal olan `oku`, `ad` ve `birim` fonksiyonlarını çağırıyor. Taban sınıfta "işin iskeletini" yazıp detayları türetilmiş sınıflara bırakmak çok sık kullanılan bir kalıp. Final projesindeki `isle_dizi` de aynı fikir.

### 4.6 Sanal yıkıcı (2 dk)

```cpp
Sensor* p = new SicaklikSensoru;
delete p;
```

`Sensor`'ün yıkıcısı sanal olmasaydı `delete p` satırı da statik bağlanır, sadece `~Sensor` çalışır, `~SicaklikSensoru` hiç çağrılmazdı. Bu tanımsız davranıştır ve türetilmiş sınıf bellek tutuyorsa o bellek sızar. Kural kısa: **sanal fonksiyonu olan bir sınıfın yıkıcısı da sanal olsun.** `virtual ~Sensor() = default;` yazmak kâfi.

> **Sıra sizde (5 dk):** `06_sanal_fonksiyonlar.cpp` dosyasına bir `NemSensoru` sınıfı ekleyin (ham kodu 0 ile 100 arasında yüzde nem değerine çevirsin, birimi `"%"` olsun). Diziye ekleyin. `main` içindeki döngüde kaç satırı değiştirmeniz gerekti?

## Ara (10 dk)

## 5. Final Projesi: Dijital Filtre Kütüphanesi (60 dk)

Klasör: [`02_filtre_kutuphanesi`](02_filtre_kutuphanesi)

Bütün gün öğrendiklerimizi bir araya getirme vakti. Senaryomuz şöyle: bir sensör 1 kHz ile örnekleniyor ve 5 Hz'lik yavaş bir sinyal ölçüyor. Lakin hayat bu kadar temiz değil, sinyalin üstüne iki tür bozulma biniyor:

1. **Beyaz gürültü:** Her örnekte küçük, rastgele sapmalar. Isıl gürültü, kuantizasyon gürültüsü.
2. **Dürtü gürültüsü:** Arada bir gelen kocaman sıçramalar. Gevşek bir konnektör, ESD, bir bit hatası.

Görevimiz bu sinyali temizleyen filtreler yazmak ve hangisinin ne kadar iyi iş çıkardığını ölçmek.

### 5.1 Mimari (5 dk)

```
                        ┌─────────────────────────────┐
                        │       Filtre  (soyut)       │
                        ├─────────────────────────────┤
                        │ isle(x)       = 0           │
                        │ sifirla()     = 0           │
                        │ ad()          = 0           │
                        │ gecikme()     (varsayılan)  │
                        │ isle_dizi()   (sanal değil) │
                        └──────────────┬──────────────┘
       ┌──────────────────┬────────────┼──────────────┬──────────────────┐
       │                  │            │              │                  │
HareketliOrtalama   MedyanFiltre   FIRFiltre   IIRAlcakGeciren    FiltreZinciri
  ◆ RingBuffer       ◆ RingBuffer   ◆ RingBuffer   (tek bir float)    ◇ Filtre* [8]
                                    ◆ katsayılar
```

`◆` "sahibi" (bileşim, RAII), `◇` "sadece gösteriyor, sahibi değil" demek.

Dosya düzeni:

```
02_filtre_kutuphanesi/
├── include/
│   ├── ring_buffer.hpp     RingBuffer sınıfı
│   ├── filtre.hpp          Filtre arayüzü ve bütün filtre sınıfları
│   └── sinyal.hpp          SinyalUreteci, rms_fark
├── src/                    her sınıfın gövdesi kendi .cpp dosyasında
│   ├── ring_buffer.cpp
│   ├── filtre.cpp          Filtre::isle_dizi ve FiltreZinciri
│   ├── hareketli_ortalama.cpp
│   ├── medyan_filtre.cpp
│   ├── fir.cpp
│   ├── iir.cpp
│   └── sinyal.cpp
├── test/                   mini_test.hpp ve test_filtre.cpp
├── ornek/main.cpp          bütün filtreleri karşılaştıran örnek program
└── Makefile
```

**Derleme ve çalıştırma:**

```bash
cd 03_gun2_oop_filtre/02_filtre_kutuphanesi
make test       # testleri çalıştır
make ornek      # örnek programı çalıştır, filtre_cikti.csv üretir
```

`make` yoksa (Windows'ta sık olur):

```bash
g++ -std=c++17 -Wall -Wextra -I include -I test src/*.cpp test/test_filtre.cpp -o test_prog
./test_prog
g++ -std=c++17 -Wall -Wextra -I include src/*.cpp ornek/main.cpp -o ornek_prog
./ornek_prog
```

### 5.2 Sözleşme: `Filtre` sınıfı (7 dk)

```cpp
class Filtre {
public:
    virtual ~Filtre() = default;
    virtual float isle(float x) = 0;
    virtual void sifirla() = 0;
    virtual const char* ad() const = 0;
    virtual size_t gecikme() const { return 0; }
    void isle_dizi(const float* giris, float* cikis, size_t n);
};
```

Ödev 04'teki filtreleriniz bütün diziyi bir kerede alıp işliyordu. Bizimkiler ise **akış** (streaming) mantığıyla çalışıyor: her `isle()` çağrısında **bir** örnek alıp **bir** örnek döndürüyor, geçmişi nesnenin kendisi hatırlıyor. Bir mikrodenetleyicide ADC kesmesi her geldiğinde `isle()` çağırdığınızı düşünün. Bütün sinyalin elinizde olmasını beklemiyorsunuz, örnek geldikçe işliyorsunuz.

Arayüzdeki dört farklı fonksiyon tipine dikkat edin, her biri bilinçli bir seçim:

| Fonksiyon | Türü | Neden? |
|---|---|---|
| `~Filtre()` | sanal | Filtreleri taban pointer'ı ile silebilelim (4.6) |
| `isle`, `sifirla`, `ad` | saf sanal | Her filtre bunları kendisi yazmak **zorunda** |
| `gecikme` | sanal, varsayılanlı | Çoğu filtrenin gecikmesi var, ama "0" makul bir varsayılan |
| `isle_dizi` | sanal değil | Her filtre için aynı: sadece `isle`'yi döngüde çağırıyor |

```cpp
void Filtre::isle_dizi(const float* giris, float* cikis, size_t n)
{
    if (giris == nullptr || cikis == nullptr) return;
    for (size_t i = 0; i < n; i++) {
        cikis[i] = isle(giris[i]);     // hangi isle()? Nesnenin gerçek tipine göre!
    }
}
```

### 5.3 `RingBuffer`: Ödev 03'ün sınıf hâli (5 dk)

Dosyalar: [`ring_buffer.hpp`](02_filtre_kutuphanesi/include/ring_buffer.hpp), [`ring_buffer.cpp`](02_filtre_kutuphanesi/src/ring_buffer.cpp)

Ödev 03'ten iki farkı var. Birincisi, kapasite yapıcıda veriliyor ve bellek RAII ile yönetiliyor. İkincisi, filtrelerin işini kolaylaştırmak için `bak()` henüz gelmemiş bir örnek sorulduğunda hata döndürmek yerine `0.0f` döndürüyor. FIR filtre başlangıçta geçmişi sıfır kabul ettiği için bu tam istediğimiz davranış.

Yapıcıda bir tuzak var:

```cpp
private:
    size_t kapasite_;     // önce bu
    float* veri_;         // sonra bu
    ...

RingBuffer::RingBuffer(size_t kapasite)
    : kapasite_(kapasite == 0 ? 1 : kapasite),
      veri_(new float[kapasite_]),      // kapasite_'yi kullanıyor
      ...
```

**Üyeler, ilk değer listesine hangi sırayla yazdığınıza göre değil, sınıfta hangi sırayla bildirildiklerine göre kurulur.** `veri_` üyesini `kapasite_`'den önce bildirseydik `new float[kapasite_]` satırı henüz kurulmamış, içi çöp dolu bir `kapasite_` okurdu. `-Wall` ile derlerseniz derleyici sırasızlık için uyarı verir (`-Wreorder`). Uyarıları ciddiye alın.

### 5.4 Hareketli ortalama (5 dk)

Dosya: [`hareketli_ortalama.cpp`](02_filtre_kutuphanesi/src/hareketli_ortalama.cpp)

Ödev 04'te her çıkış örneği için penceredeki bütün örnekleri baştan topluyordunuz. 64'lük bir pencerede bu, her örnek için 64 toplama demek. Daha akıllıcası, toplamı bir üye değişkende tutup her adımda güncellemek:

```cpp
float HareketliOrtalama::isle(float x)
{
    if (gecmis_.dolu()) {
        toplam_ -= gecmis_.bak(gecmis_.adet() - 1);   // pencereden düşeni çıkar
    }
    gecmis_.ekle(x);
    toplam_ += x;                                     // yeni geleni ekle
    return static_cast<float>(toplam_ / gecmis_.adet());
}
```

Pencere 1000 de olsa her örnek için iki işlem. `toplam_` değişkenini `double` tuttuk, zira binlerce ekleme ve çıkarmadan sonra `float` hassasiyetinde küçük yuvarlama hataları birikebilir. Testlerde 10000 örnek sonra bile ortalamanın kaymadığını kontrol ediyoruz.

### 5.5 Medyan filtre (5 dk)

Dosya: [`medyan_filtre.cpp`](02_filtre_kutuphanesi/src/medyan_filtre.cpp)

Ödev 04'teki medyan filtre **merkeziydi**: `i` numaralı çıkış için `i − k` ile `i + k` arasındaki örneklere bakıyordu. Yani **gelecekteki** örnekleri biliyordu. Kaydedilmiş bir dosyayı işlerken bu mümkün, lakin gerçek zamanda çalışırken gelecek örnek henüz gelmedi. Bu yüzden bizim filtremiz **nedensel** (causal): sadece son `N` örneğe bakıyor.

Bunun bir bedeli var: çıkış girişin `N / 2` örnek gerisinden geliyor. Merkezi filtrede bu gecikme yoktu, zira k örnek ileriye bakarak telafi ediliyordu. Hiçbir gerçek zamanlı filtre geleceği göremez, bunu aklınızın bir köşesinde tutun.

```cpp
float MedyanFiltre::isle(float x)
{
    gecmis_.ekle(x);
    size_t n = gecmis_.adet();
    for (size_t i = 0; i < n; i++) siralama_[i] = gecmis_.bak(i);   // kopyasını al
    insertion_sort(siralama_, n);                                    // Gün 1'den
    if (n % 2 == 1) return siralama_[n / 2];
    return 0.5f * (siralama_[n / 2 - 1] + siralama_[n / 2]);
}
```

Ring buffer'ın kendisini sıralasaydık zaman sırası bozulurdu ve bir sonraki adımda hangi örneğin en eski olduğunu bilemezdik. O yüzden kopyasını sıralıyoruz. Çalışma alanı için `MAKS_PENCERE = 31` boyutlu sabit bir dizi kullandık, heap'e gerek yok.

### 5.6 FIR filtre (8 dk)

Dosya: [`fir.cpp`](02_filtre_kutuphanesi/src/fir.cpp)

**FIR** (Finite Impulse Response, sonlu dürtü yanıtlı) filtre, son N girişin ağırlıklı toplamıdır:

```
y[n] = b[0]·x[n] + b[1]·x[n−1] + b[2]·x[n−2] + ... + b[N−1]·x[n−N+1]
```

`b[k]` katsayıları filtrenin bütün karakterini belirler. Filtreye tek bir `1` verip ardından sıfırlar verirseniz (dürtü) çıkışta katsayıların kendisini sırayla görürsünüz. Adı da buradan geliyor: dürtü yanıtı N örnek sonra biter, sonludur.

İlginç bir gözlem: hareketli ortalama da aslında bir FIR filtredir! Bütün katsayıları `1/N` olan bir FIR. Biz onu ayrı bir sınıf olarak yazdık, zira toplam güncelleme hilesiyle çok daha hızlı çalışıyor.

```cpp
float FIRFiltre::isle(float x)
{
    gecmis_.ekle(x);
    double y = 0.0;
    for (size_t k = 0; k < n_; k++) {
        y += b_[k] * gecmis_.bak(k);     // bak(k) = x[n−k]
    }
    return static_cast<float>(y);
}
```

`FIRFiltre` sınıfı iki kaynak tutuyor: katsayılar için `new` ile alınmış bir dizi ve geçmiş için bir `RingBuffer`. Katsayıları kendisi yönettiği için yıkıcısı ve yasaklanmış kopyalaması var (üçler kuralı), `RingBuffer` ise kendi başının çaresine bakıyor. Yapıcıya bakın: katsayılar **kopyalanıyor**. Kullanıcının verdiği diziyi saklasaydık, o dizi değiştiğinde ya da yok olduğunda filtremiz de bozulurdu. Testlerde bunu da kontrol ediyoruz.

**Katsayıları nereden bulacağız?** `fir_alcak_geciren_tasarla` fonksiyonu **pencereli sinc** yöntemiyle alçak geçiren katsayılar üretiyor. İdeal bir alçak geçiren filtrenin dürtü yanıtı sonsuz uzunlukta bir sinc fonksiyonudur. Biz onu ortasından N örnek uzunluğunda kesiyor, kesmenin oluşturduğu dalgalanmayı azaltmak için **Hamming penceresi** ile çarpıyor (Ödev 05'teki Hamming kodunu bulan Richard Hamming'in ta kendisi) ve katsayıların toplamını 1'e normalize ediyoruz ki sabit bir sinyal filtreden aynen geçsin. Ayrıntısı İşaretler ve Sistemler dersinin konusu, bugün kullanmamız kâfi.

Katsayılar simetrik olduğunda FIR filtrenin bütün frekansları **aynı** miktarda geciktirdiğini bilmek faydalı: tam `(N − 1) / 2` örnek. Bu özelliğe *doğrusal faz* deniyor ve sinyalin şeklini bozmadığı için çok kıymetli.

### 5.7 IIR filtre (5 dk)

Dosya: [`iir.cpp`](02_filtre_kutuphanesi/src/iir.cpp)

**IIR** (Infinite Impulse Response, sonsuz dürtü yanıtlı) filtre, girişlerin yanında **kendi önceki çıkışlarını** da kullanır. En basiti birinci dereceden alçak geçiren:

```
y[n] = y[n−1] + α · (x[n] − y[n−1])
```

Okuması kolay: çıkış, yeni gelen örneğe doğru her adımda `α` oranında yaklaşıyor. `α = 1` ise çıkış girişi anında yakalar (filtre yok), `α` küçüldükçe çıkış daha yavaş ve daha pürüzsüz hareket eder.

Bu, laboratuvardan tanıdığınız **RC alçak geçiren devrenin** ayrık zamandaki karşılığıdır. Kondansatör gerilimi de giriş gerilimine üstel olarak yaklaşır. Kesim frekansı `fc` ve örnekleme frekansı `fs` verildiğinde:

```
α = 1 − e^(−2π·fc/fs)
```

`IIRAlcakGeciren::alfa_hesapla` bu hesabı yapıyor. `static` bir üye fonksiyon olduğuna dikkat edin: bir nesneye ihtiyacı yok, `IIRAlcakGeciren::alfa_hesapla(10.0f, 1000.0f)` diye sınıf adıyla çağrılıyor.

IIR filtrenin büyük avantajı ucuzluğu: geçmişin tamamı tek bir `float` içinde (`y_`). 61 katsayılı bir FIR'ın yaptığı işe yakın bir işi her örnek için iki çarpma ve bir toplama ile yapıyor. Dezavantajı ise frekansları **farklı** miktarlarda geciktirmesi (doğrusal olmayan faz) ve kesim frekansının altındaki sinyalleri de biraz zayıflatması. Birazdan sonuçlarda göreceğiz.

### 5.8 Filtre zinciri (5 dk)

Dosya: [`filtre.cpp`](02_filtre_kutuphanesi/src/filtre.cpp)

Gerçek sistemlerde filtreler ekseriya arka arkaya bağlanır. `FiltreZinciri` bunu yapıyor ve **kendisi de bir `Filtre`**:

```cpp
float FiltreZinciri::isle(float x)
{
    for (size_t i = 0; i < adet_; i++) {
        x = filtreler_[i]->isle(x);      // her halkanın çıkışı bir sonrakinin girişi
    }
    return x;
}
```

Bunun iki güzel sonucu var. Birincisi, zincir hangi filtrelerden oluştuğunu bilmiyor ve umursamıyor; yarın yazacağınız yeni bir filtreyi de hiçbir değişiklik yapmadan zincire ekleyebilirsiniz. İkincisi, zincir bir filtre olduğu için **bir zinciri başka bir zincirin içine** koyabilirsiniz. Bu kalıbın adı *Composite* (bileşik).

**Sahiplik meselesi:** Zincir filtrelerin pointer'larını tutuyor, lakin onları silmiyor. Yani filtrelerin **sahibi değil**. Bu kararı başlık dosyasında açıkça yazdık, zira bir pointer'a bakarak onu kimin sileceğini anlamak mümkün değil. Kullanıcının dikkat etmesi gereken: zincire eklenen filtreler zincirden önce ölmemeli.

Bir de şu: bir filtre nesnesi **tek bir sinyal akışının** geçmişini tutar. Aynı nesneyi hem tek başına hem de zincirin içinde kullanırsanız iki akışın örnekleri aynı ring buffer'a karışır. Örnek programda zincir için ayrı nesneler oluşturmamızın sebebi bu.

### 5.9 Sonuçlar: Hangi filtre kazandı? (10 dk)

Dosya: [`ornek/main.cpp`](02_filtre_kutuphanesi/ornek/main.cpp)

Örnek program bütün filtreleri bir `Filtre*` dizisine koyup aynı gürültülü sinyalden geçiriyor. 4.5'teki sensör döngüsünün aynısı:

```cpp
Filtre* filtreler[] = {&ortalama, &medyan, &fir, &iir, &zincir};
for (size_t f = 0; f < F; f++) {
    filtreler[f]->isle_dizi(gurultulu, cikislar[f], N);
    ...
}
```

`make ornek` çıktısı:

```
Sinyal: 5 Hz sinus, fs = 1000 Hz, 1000 ornek
Gurultulu sinyalin RMS hatasi: 0.3004

Filtre                Gecikme     RMS hata   RMS (gecikmesiz)
------                -------     --------   ----------------
Hareketli ortalama         10       0.2380             0.0580
Medyan                      7       0.1642             0.0523
FIR                        30       0.6396             0.0543
IIR alcak geciren          15       0.3202             0.0901
Zincir                     12       0.2696             0.0376
```

İlk bakışta tuhaf bir tablo. **FIR filtrenin hatası, hiç filtrelenmemiş sinyalden bile büyük!** Filtre bozuk mu?

Değil. 61 katsayılı FIR çıkışı 30 örnek, yani 30 ms geciktiriyor. 5 Hz'lik bir sinüs için 30 ms, periyodun %15'i, yani 54 derecelik bir faz kayması demek. Genliği 1 olan bir sinüsle onun 54 derece kaymış hâli arasındaki RMS fark `√2 · sin(27°) ≈ 0.64`. Tablodaki hata neredeyse tamamen gecikmeden geliyor, gürültüden değil.

Son sütun, her filtrenin çıkışını kendi gecikmesi kadar geri kaydırıp öyle karşılaştırıyor. Asıl temizleme başarısı orada görünüyor:

* **Hareketli ortalama ve FIR** beyaz gürültüyü iyi bastırıyor, lakin dürtüler onları zorluyor. 3 birimlik bir sıçrama, ortalamaya girince 21 örnek boyunca küçük bir tümsek bırakıyor.
* **Medyan** dürtüleri adeta silip atıyor, zira sıralanmış pencerenin ortasına bir uç değer asla gelemiyor. Ödev 04'ün README'sinde söylediğimiz şey tam olarak buydu.
* **IIR** en ucuzu, fakat en kötü sonuç onun. Sebebi iki tane: 10 Hz kesim frekansı 5 Hz'lik sinyalimizi de yaklaşık %10 zayıflatıyor ve fazı doğrusal olmadığı için tek bir sayı ile gecikmesini tam telafi edemiyoruz.
* **Zincir** (5'lik medyan, ardından 21'lik ortalama) birinciliği alıyor. Küçük medyan önce sıçramaları yok ediyor, ardından ortalama geriye kalan beyaz gürültüyü yumuşatıyor. Her filtre en iyi yaptığı işi yapıyor.

Buradan çıkarılacak ders şu: **"en iyi filtre" diye bir şey yok.** Gürültünün türüne, gecikmeye ne kadar tahammülünüz olduğuna ve işlemcinizin gücüne göre seçim yapıyorsunuz. Bir motor kontrol döngüsünde 30 ms gecikme sistemi kararsız yapabilir, bir sıcaklık kaydedicide ise kimsenin umurunda olmaz.

Program ayrıca `filtre_cikti.csv` dosyası üretiyor. Excel, MATLAB ya da Python ile açıp temiz, gürültülü ve filtrelenmiş sinyalleri üst üste çizin. Gecikmeyi gözünüzle görmek tablodan çok daha öğretici.

### 5.10 Testler

Dosya: [`test/test_filtre.cpp`](02_filtre_kutuphanesi/test/test_filtre.cpp)

Ödevlerdeki `mini_test.hpp` aracıyla 116 kontrol yazdık. Birkaçına bakmakta fayda var, zira bir filtrenin "doğru çalışması" ne demek sorusuna cevap veriyorlar:

* FIR'a bir dürtü verince çıkışta katsayılar sırayla görünüyor mu?
* Tasarlanan alçak geçiren filtre 300 Hz'lik bir sinüsü gerçekten yok ediyor mu?
* Hareketli ortalama 10000 örnek sonra hâlâ doğru mu, yoksa yuvarlama hataları birikti mi?
* `isle_dizi` çağırmak, `isle`'yi döngüde tek tek çağırmakla bit bit aynı sonucu veriyor mu?
* Heap'teki bir filtreyi `Filtre*` üzerinden silmek güvenli mi?

> **Sıra sizde (kalan süre ve ev için):** Aşağıdakilerden birini seçin, yeni sınıfı `filtre.hpp` ve `src/` altına ekleyin, testlerini yazın ve örnek programın filtre dizisine ekleyin. Örnek programın döngüsünde bir satır bile değiştirmeniz gerekmeyecek.
>
> 1. **`Kazanc`**: Her örneği sabit bir sayıyla çarpan en basit filtre. Isınma turu.
> 2. **`DCEngelleyici`**: Sinyalin ortalamasını (DC bileşenini) atan yüksek geçiren filtre. İpucu: `y[n] = x[n] − x[n−1] + R · y[n−1]`, `R` yaklaşık 0.995. Test için sinüse bir ofset ekleyin.
> 3. **`Turev`**: `y[n] = (x[n] − x[n−1]) · fs`. Hız sensöründen ivme çıkarmak gibi. Gürültüye ne yaptığına bakın.
> 4. **`IkinciDereceIIR`** (biquad): `y[n] = b0·x[n] + b1·x[n−1] + b2·x[n−2] − a1·y[n−1] − a2·y[n−2]`. Ses işlemenin iş atı budur. Katsayıları "RBJ Audio EQ Cookbook" ile aratarak bulabilirsiniz.
> 5. **Grafik**: `filtre_cikti.csv` dosyasını okuyup bütün sinyalleri çizen kısa bir Python ya da MATLAB betiği yazın.

## 6. Kapanış (10 dk)

Bugün öğrendiklerimizin hâsılı:

* **Sınıf**, veriyi ve onu kullanan fonksiyonları bir araya getirir. `private` alanlar, sınıfın değişmezlerini korur.
* **Yapıcı** nesnenin geçerli doğmasını, **yıkıcı** temiz ölmesini sağlar.
* **RAII**: Kaynağı yapıcıda al, yıkıcıda bırak. `new` ve `delete` birkaç küçük sınıfın içinde kalsın. Yıkıcı yazdıysanız kopyalamayı da düşünün.
* **Kalıtım** bir "is-a" ilişkisidir. "Has-a" ise bileşimdir, ekseriya doğru seçim odur.
* **Sanal fonksiyonlar** sayesinde taban sınıf pointer'ı üzerinden doğru fonksiyon çağrılır. Soyut sınıf bir sözleşmedir. Sanal fonksiyonu olan sınıfın yıkıcısı da sanal olsun.
* Gerçek zamanlı bir filtre geleceği göremez, her filtrenin bir gecikmesi ve bir maliyeti vardır. "En iyi filtre" uygulamaya göre değişir.

**Bundan sonra ne öğrenmeli?** Sıradaki doğal adımlar şablonlar (`template`, böylece `RingBuffer` yalnızca `float` değil her tip için çalışır), standart kütüphanenin kapları (`std::vector`, `std::array`) ve akıllı pointer'lar (`std::unique_ptr`). Bugün elle yaptığımız bellek yönetiminin hazır ve sağlam hâlleri onlar. Gömülü tarafta ilgilenenler için ise bir STM32 ya da Arduino üzerinde bugünkü filtrelerden birini ADC kesmesine bağlamak harika bir hafta sonu projesi olur.

İki gün boyunca bizimle olduğunuz için teşekkürler. Takıldığınız yerde çekinmeden sorun, repo açık kalmaya devam edecek.

## Ek Okuma

* [learncpp.com](https://www.learncpp.com): Bölüm 14 ve 15 (sınıflar), Bölüm 24 ve 25 (kalıtım ve sanal fonksiyonlar).
* [cppreference: RAII](https://en.cppreference.com/w/cpp/language/raii): Kısa ve net.
* [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines): Bilhassa C (sınıflar) ve R (kaynak yönetimi) bölümleri. Bugün söylediğimiz kuralların çoğu oradan.
* Steven W. Smith, *The Scientist and Engineer's Guide to Digital Signal Processing*: Ücretsiz çevrim içi kitap ([dspguide.com](https://www.dspguide.com)). Bölüm 15 (hareketli ortalama), 16 (pencereli sinc) ve 19 (IIR) bugünkü projenin teorisi.

## Eğitmen notu

Bu kısım dersi anlatacak arkadaşlar için. Süre tablosu bir öneridir, sınıfın seviyesine göre esnetin.

**Toplam:** 210 dk ders, 30 dk ara (3 ara), 240 dk. Gün 1'de olduğu gibi **planda hiç tampon yok.**

**Açılışta ödev kontrolü:** Ana README'de "12 Kasım'da birkaç kişiye bu satır ne yapıyor diye soracağız" dedik. Açılışa sığmaz, onu final projesinin ilk 5 dakikasına ya da aralara yayın. Ödev 03 ve 04'ü teslim edenlerin kodunu 5.3 ve 5.4'te kütüphanedeki sınıflarla karşılaştırmak hem kontrol hem tekrar olur.

**Ön koşullar:** Referanslar (Gün 1, 2.10 ve 3.3) ve `std::string` (Gün 1, 2.4) birinci günde anlatıldı, lakin `const std::string&` gibi bir parametre ilk kez burada geçiyor. 2.3'e gelmeden `05_kalitim.cpp` açılırsa bir cümleyle açıklayın. `std::string` sınıfının kendi belleğini kendisi yönettiğini 3.2'de RAII örneği olarak hatırlatmak iyi bir köprü olur.

**Sınıflar bölümünden kısaltmak gerekirse (35 dk yerine 25 dk):**

* 1.3'teki `this` adres çıktısını gösterip geçin, perde arkası açıklamasını kısaltın.
* 1.6'daki "Sıra sizde" alıştırmasını sözlü sorun, tahminleri alıp çıktıyı birlikte izleyin.
* 1.5'teki delegating constructor bir cümleyle geçilebilir.

**Kapsülleme ve RAII'den kısaltmak gerekirse:**

* 2.4 (başlık ve kaynak dosyası) **kesilemez**, final projesi tamamen buna dayanıyor. Ödevlerde zaten bu düzeni gördüler, "ödevlerdeki gibi" diyerek hızlandırabilirsiniz.
* 3.4 `explicit` bir cümleye inebilir.
* 3.3'teki kopyalama tuzağını **kesmeyin**. Çift silme demosunu canlı yapmak çok etkili. Windows'ta MinGW ile program ekseriya sessizce ya da bir hata koduyla biter, Linux ve macOS'ta `free(): double free detected` mesajı görülür. Önceden kendi makinenizde deneyin.

**Kalıtım bölümünden kısaltmak gerekirse (40 dk yerine 30 dk):**

* 4.3'teki vtable açıklamasını tek cümleye indirin.
* 4.1'de `protected` konusunu kısa tutun, final projesinde hiç kullanmıyoruz.
* 4.4, 4.5 ve 4.6'ya **dokunmayın**. `Filtre` sınıfının bütün tasarımı bu üçüne dayanıyor.

**Final projesi hakkında:**

* Proje **tamamen yazılmış** hâlde geliyor. 60 dakikada sıfırdan yazdırmak gerçekçi değil. Akış şöyle: önce `make test` ve `make ornek` çalıştırılsın (5 dk), sonra 5.2'den 5.8'e kadar dosyalar ekranda birlikte okunsun, en sonda 5.9'daki tabloyu tartışın. Tablo tartışması dersin en akılda kalan kısmı olabilir, sıkışırsanız 5.6'daki pencereli sinc anlatımını kısaltıp oraya zaman ayırın.
* Süre kalırsa "Sıra sizde" listesinden 1 ya da 2 numarayı derste birlikte yazın. `Kazanc` 5 dakikada biter ve yeni bir sınıfın arayüze nasıl eklendiğini baştan sona gösterir.
* 5.9'daki tablo, `SinyalUreteci` sabit tohumla (2026) çalıştığı için her makinede aynı çıkmalı. Farklı çıkarsa derleyicinin `std::normal_distribution` gerçeklemesi farklıdır (MSVC ile GCC farklı sayılar üretebilir). Sıralama değişmez, sayılar biraz oynar.
* `make` Windows'ta ekseriya kurulu değil. Makefile'ın başında ve 5.1'de tek satırlık `g++` komutları var, onları kullanın.
* IIR'ın gecikmesi (`gecikme()` fonksiyonu) düşük frekans için yapılmış bir yaklaşım, bunu soran olursa "IIR'da gecikme frekansa bağlıdır, tek bir sayı yok" deyin. Tablodaki IIR sonucunun kötü çıkmasının bir kısmı bundan.
