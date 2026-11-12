# Gün 2 Terimleri | Teknik Sözlük

İkinci günün bütün terimlerinin kısa ve doğrudan tanımları. Metafor ya da hikâye yok; onları isteyen [SOZLUK.md](SOZLUK.md) sayfasına bakabilir. Her terimin yanındaki numara, [ders notunda](README.md) anlatıldığı bölümü gösteriyor.

Terimler konularına göre gruplandı. Her grubun içinde, öğrenme sırasına göre dizildiler.

---

## Sınıf Temelleri

**Sınıf** (*class*) · 1.1
Veriyi (üye değişkenler) ve o veri üzerinde çalışan fonksiyonları (üye fonksiyonlar) tek bir tipte toplayan kullanıcı tanımlı tip.

**Nesne** (*object*) · 1.1
Bir sınıf tipinden oluşturulmuş, bellekte yer kaplayan somut örnek. `Sayac a;` satırında `Sayac` sınıf, `a` nesnedir.

**Üye değişken** (*member variable, data member*) · 1.1
Sınıfın içinde tanımlanan değişken. Her nesnenin kendi kopyası vardır. Bu repoda sonuna `_` konuyor (`deger_`).

**Üye fonksiyon** (*member function, method*) · 1.1
Sınıfın içinde tanımlanan fonksiyon. Bir nesne üzerinden çağrılır (`a.artir()`) ve o nesnenin üyelerine doğrudan erişir. Makine kodu bütün nesneler için ortaktır.

**Erişim belirteci** (*access specifier*) · 1.2
Bir üyeye kimin erişebileceğini belirler:

| Belirteç | Sınıfın kendisi | Türetilmiş sınıflar | Dışarısı |
|---|:---:|:---:|:---:|
| `public` | ✓ | ✓ | ✓ |
| `protected` | ✓ | ✓ | ✗ |
| `private` | ✓ | ✗ | ✗ |

**`struct` ile `class` farkı** · 1.2
Tek fark varsayılan erişimdir: `struct` üyeleri varsayılan olarak `public`, `class` üyeleri `private` olur. Gelenek olarak `struct` yalnızca veri taşıyan basit yapılar için kullanılır.

**`this`** · 1.3
Her (statik olmayan) üye fonksiyona gizlice geçirilen, fonksiyonun çağrıldığı nesnenin adresini tutan pointer. `deger_` yazmak `this->deger_` yazmakla aynıdır.

**`const` üye fonksiyon** (*const member function*) · 1.4
İmzasının sonunda `const` olan, nesnenin üyelerini değiştirmeyeceğini garanti eden fonksiyon. `const` bir nesne ya da `const` referans üzerinden yalnızca bu fonksiyonlar çağrılabilir.

```cpp
size_t adet() const { return adet_; }
```

**`static` üye fonksiyon** (*static member function*) · 5.7
Bir nesneye bağlı olmayan, `this` pointer'ı almayan üye fonksiyon. Sınıf adıyla çağrılır: `IIRAlcakGeciren::alfa_hesapla(10.0f, 1000.0f)`.

**`static constexpr` üye** · 5.5
Bütün nesneler için ortak, derleme anında bilinen sabit. `MedyanFiltre::MAKS_PENCERE` gibi.

---

## Nesnenin Ömrü

**Yapıcı** (*constructor*) · 1.5
Nesne oluşturulurken otomatik çağrılan, sınıfla aynı adı taşıyan, dönüş tipi olmayan özel fonksiyon. Görevi nesneyi geçerli bir başlangıç durumuna getirmektir. Aşırı yüklenebilir (birden fazla yapıcı yazılabilir).

**Varsayılan yapıcı** (*default constructor*) · 1.5
Parametresiz çağrılabilen yapıcı. Sınıfa hiç yapıcı yazılmazsa derleyici boş bir tane üretir; tek bir yapıcı bile yazılırsa üretmez.

**Yetki devreden yapıcı** (*delegating constructor*) · 1.5
İşini aynı sınıfın başka bir yapıcısına devreden yapıcı: `Adc() : Adc(3.3, 12) {}`.

**İlk değer listesi** (*member initializer list*) · 1.5
Yapıcı imzasından sonra `:` ile başlayan, üyeleri gövde çalışmadan önce kuran liste. Referans üyeler, `const` üyeler ve varsayılan yapıcısı olmayan nesne üyeler yalnızca burada kurulabilir.

```cpp
Adc(double vref, unsigned bit) : vref_(vref), bit_(bit) { ... }
```

**Üye kurulum sırası** (*member initialization order*) · 5.3
Üyeler, ilk değer listesindeki yazım sırasına göre değil, **sınıf tanımındaki bildirim sırasına** göre kurulur. Bir üye diğerine bağlıysa bildirimde ondan sonra gelmelidir. GCC'de `-Wreorder` uyarısı (`-Wall` içinde) sıra uyuşmazlığını bildirir.

**Yıkıcı** (*destructor*) · 1.6
Nesnenin ömrü bittiğinde otomatik çağrılan, `~SinifAdi()` biçiminde yazılan, parametre almayan fonksiyon. Her sınıfın tam bir yıkıcısı vardır.

**Otomatik ömür** (*automatic storage duration*) · 1.6
Stack'teki yerel nesnelerin ömrü. Nesne tanımlandığı blok `}` ile bitince yıkılır. Aynı bloktaki nesneler oluşturulma sırasının tersine yıkılır.

**Dinamik ömür** (*dynamic storage duration*) · 1.6
`new` ile heap'te oluşturulan nesnelerin ömrü. Nesne yalnızca `delete` çağrıldığında yıkılır; çağrılmazsa bellek sızar.

**Statik ömür** (*static storage duration*) · 1.6
Global ve `static` nesnelerin ömrü. Program başlarken kurulur, program biterken yıkılır.

---

## Kapsülleme ve Dosya Düzeni

**Kapsülleme** (*encapsulation*) · 2.1
Bir sınıfın iç verisini `private` yapıp dışarıya yalnızca işlem yapan bir arayüz sunması. Amacı değişmezleri korumaktır.

**Değişmez** (*class invariant*) · 2.2
Bir nesne için her public fonksiyon çağrısından önce ve sonra doğru olması gereken koşul. Ring buffer için: `bas_ < kapasite_` ve `adet_ <= kapasite_`. Yapıcı değişmezi kurar, üye fonksiyonlar korur.

**Arayüz** (*interface*) · 2.2
Bir sınıfın dışarıdan kullanılabilen public üyelerinin bütünü. Sınıfın ne yaptığını tanımlar, nasıl yaptığını gizler.

**Getter ve setter** (*accessor, mutator*) · 2.2
Bir üyeyi okuyan ve yazan public fonksiyonlar. Her üyeye koşulsuz bir setter açmak kapsüllemeyi fiilen ortadan kaldırır.

**Referans** (*reference*) · 0, 2.3
Var olan bir nesneye takılan ikinci isim. Kurulduktan sonra başka bir nesneye bağlanamaz ve `nullptr` olamaz.

**`const` referans** (*const reference*) · 2.3
Nesneyi kopyalamadan, değiştirmeme garantisiyle geçirmek için kullanılan parametre biçimi: `void yazdir(const RingBuffer& rb)`.

**Parametre geçirme kuralı** · 2.3
Küçük tipler (`int`, `float`, `bool`) değerle, okunacak nesneler `const T&` ile, değiştirilecek nesneler `T&` ile geçirilir.

**Başlık dosyası** (*header, `.hpp`*) · 2.4
Sınıf tanımlarını ve fonksiyon bildirimlerini içeren, `#include` ile eklenen dosya.

**Kaynak dosyası** (*source, `.cpp`*) · 2.4
Fonksiyon gövdelerini içeren, ayrı ayrı derlenen dosya. Üye fonksiyon gövdeleri sınıf adıyla nitelenir: `void RingBuffer::ekle(float deger) { ... }`.

**Kapsam çözümleme operatörü** (*scope resolution operator, `::`*) · 2.4
Bir adın hangi sınıfa ya da isim alanına ait olduğunu belirtir: `RingBuffer::ekle`, `std::cout`, `MedyanFiltre::MAKS_PENCERE`.

**`#pragma once`** · 2.4
Bir başlık dosyasının aynı derleme birimine birden fazla eklenmesini engelleyen yönerge.

**İsimsiz isim alanı** (*anonymous namespace*) · 5.5
`namespace { ... }` içindeki adlar yalnızca o `.cpp` dosyasından görünür. `medyan_filtre.cpp` içindeki `insertion_sort` böyle tanımlandı.

---

## Kaynak Yönetimi

**Bellek sızıntısı** (*memory leak*) · 3.1
`new` ile alınan belleğin hiç `delete` edilmemesi. Program çalıştıkça kullanılan bellek artar.

**RAII** (*Resource Acquisition Is Initialization*) · 3.2
Bir kaynağın (bellek, dosya, kilit, donanım) ömrünü bir nesnenin ömrüne bağlama tekniği: kaynak yapıcıda alınır, yıkıcıda bırakılır. Yıkıcı her çıkış yolunda çalıştığı için kaynak sızmaz. `std::vector`, `std::string` ve `std::unique_ptr` bu tekniği kullanır.

**Kopyalama yapıcısı** (*copy constructor*) · 3.3
Bir nesneyi aynı tipteki başka bir nesneden kuran yapıcı: `T(const T&)`. `T b = a;` satırında çağrılır.

**Kopyalayarak atama operatörü** (*copy assignment operator*) · 3.3
Var olan bir nesneye başka bir nesneyi atayan operatör: `T& operator=(const T&)`. `b = a;` satırında çağrılır.

**Yüzeysel kopya** (*shallow copy*) · 3.3
Derleyicinin ürettiği varsayılan kopyalama: üyeleri tek tek kopyalar. Pointer üyelerde gösterilen bellek değil adres kopyalanır, iki nesne aynı belleği paylaşır.

**Derin kopya** (*deep copy*) · 3.3
Pointer'ın gösterdiği belleği de yeni bir alana kopyalayan, elle yazılmış kopyalama.

**Çift silme** (*double free*) · 3.3
Aynı belleğin iki kez `delete` edilmesi. Tanımsız davranıştır; ekseriya program çöker. Yüzeysel kopyanın tipik sonucudur.

**Tanımsız davranış** (*undefined behavior*) · 3.3, 4.6
Dil standardının sonucunu tanımlamadığı durum. Program çökebilir, yanlış çalışabilir ya da doğru çalışıyormuş gibi görünebilir.

**Üçler kuralı** (*rule of three*) · 3.3
Bir sınıf yıkıcı, kopyalama yapıcısı ya da kopyalayarak atama operatöründen birini elle tanımlıyorsa büyük ihtimalle üçünü de tanımlamalı (ya da yasaklamalıdır).

**Silinmiş fonksiyon** (*deleted function, `= delete`*) · 3.3
Kullanılması derleme hatası veren fonksiyon. Kopyalamayı yasaklamak için kullanılır:

```cpp
RingBuffer(const RingBuffer&) = delete;
RingBuffer& operator=(const RingBuffer&) = delete;
```

**`= default`** · 4.6
Derleyiciye bir özel fonksiyonun varsayılan hâlini üretmesini açıkça söyler: `virtual ~Filtre() = default;`.

**`explicit`** · 3.4
Tek parametreli bir yapıcının örtük tip dönüşümünde kullanılmasını engeller. `explicit DinamikTampon(size_t)` varken `DinamikTampon t = 50;` derlenmez.

**Bileşim** (*composition, has-a*) · 3.5
Bir sınıfın başka bir sınıfın nesnesini üye olarak içermesi. Üye nesneler, içeren nesne yıkılırken otomatik yıkılır. `HareketliOrtalama` bir `RingBuffer` içerir ve bu yüzden yıkıcıya ihtiyaç duymaz.

**Sahiplik** (*ownership*) · 5.8
Bir kaynağı silmekten hangi nesnenin sorumlu olduğu. `FIRFiltre` katsayı dizisinin sahibidir; `FiltreZinciri` ise tuttuğu `Filtre*` pointer'larının sahibi değildir, onları silmez.

---

## Kalıtım ve Çok Biçimlilik

**Kalıtım** (*inheritance, is-a*) · 4.1
Bir sınıfın (türetilmiş) başka bir sınıfın (taban) üyelerini devralması: `class SicaklikSensoru : public Sensor`. Public kalıtımda türetilmiş nesne, taban tipin beklendiği her yerde kullanılabilir.

**Taban sınıf** (*base class*) ve **türetilmiş sınıf** (*derived class*) · 4.1
Kalıtımda üyelerini devreden sınıf taban, devralan sınıf türetilmiş sınıftır.

**Kurulum ve yıkım sırası** (kalıtımda) · 4.1
Kurulumda önce taban sınıf, sonra türetilmiş sınıfın üyeleri, en son türetilmiş sınıfın yapıcı gövdesi çalışır. Yıkım tam tersi sıradadır. Taban sınıfın yapıcısı parametre istiyorsa ilk değer listesinde çağrılır: `: Sensor(ad)`.

**Statik bağlama** (*static binding, early binding*) · 4.2
Hangi fonksiyonun çağrılacağına derleme anında, ifadenin **statik tipine** (pointer ya da referansın tipine) bakarak karar verilmesi. Sanal olmayan fonksiyonlar böyle çağrılır.

**Dinamik bağlama** (*dynamic binding, late binding*) · 4.3
Hangi fonksiyonun çağrılacağına çalışma anında, nesnenin **dinamik tipine** (gerçek tipine) bakarak karar verilmesi. Sanal fonksiyonlar böyle çağrılır.

**Sanal fonksiyon** (*virtual function*) · 4.3
Taban sınıfta `virtual` ile bildirilen, türetilmiş sınıflarda yeniden tanımlanabilen ve taban pointer ya da referans üzerinden çağrıldığında dinamik bağlanan fonksiyon.

**`override`** · 4.3
Bir fonksiyonun taban sınıftaki sanal bir fonksiyonu ezdiğini belirten belirteç. İmza uyuşmazsa derleme hatası verir.

**vtable** (*virtual table*) · 4.3
Sanal fonksiyonu olan her sınıf için derleyicinin oluşturduğu, sanal fonksiyonların adreslerini tutan tablo. Her nesne bu tabloyu gösteren gizli bir pointer (*vptr*) taşır. Maliyeti: nesne başına bir pointer, çağrı başına bir dolaylı adresleme.

**Saf sanal fonksiyon** (*pure virtual function*) · 4.4
Gövdesi olmayan, `= 0` ile biten sanal fonksiyon. Somut olmak isteyen her türetilmiş sınıf onu tanımlamak zorundadır.

```cpp
virtual float isle(float x) = 0;
```

**Soyut sınıf** (*abstract class*) · 4.4
En az bir saf sanal fonksiyonu olan sınıf. Doğrudan nesnesi oluşturulamaz; yalnızca pointer ya da referans tipi olarak ve taban sınıf olarak kullanılır. `Filtre` ve `Sensor` soyut sınıflardır.

**Somut sınıf** (*concrete class*) · 4.4
Bütün saf sanal fonksiyonları tanımlanmış, nesnesi oluşturulabilen sınıf. `FIRFiltre`, `SicaklikSensoru` gibi.

**Çok biçimlilik** (*polymorphism*) · 4.5
Farklı tipteki nesnelerin ortak bir taban tip üzerinden aynı arayüzle kullanılması ve her birinin kendi davranışını göstermesi. C++'ta taban pointer ya da referans ile sanal fonksiyonlar üzerinden sağlanır.

**Şablon yöntem kalıbı** (*template method pattern*) · 4.5, 5.2
Taban sınıftaki sanal olmayan bir fonksiyonun işin iskeletini kurup ayrıntılar için sanal fonksiyonları çağırması. `Filtre::isle_dizi` her örnek için sanal `isle()` fonksiyonunu çağırır.

**Sanal yıkıcı** (*virtual destructor*) · 4.6
Taban sınıfta `virtual` bildirilen yıkıcı. Türetilmiş bir nesne taban pointer üzerinden `delete` edildiğinde türetilmiş sınıfın yıkıcısının da çalışmasını sağlar. Sanal yıkıcı olmadan bu işlem tanımsız davranıştır. Sanal fonksiyonu olan her sınıfın yıkıcısı sanal olmalıdır.

**Bileşik kalıbı** (*composite pattern*) · 5.8
Bir nesne grubunun, gruptaki tek bir nesneyle aynı arayüze sahip olması. `FiltreZinciri` bir `Filtre`'dir ve `Filtre` nesneleri içerir; bu yüzden zincirler iç içe konabilir.

---

## Dijital Sinyal İşleme

**Örnekleme frekansı** (*sampling frequency, fs*) · 5
Sürekli bir sinyalden saniyede alınan örnek sayısı (Hz). Projede `fs = 1000 Hz`.

**Nyquist frekansı** · 5.6
Örnekleme frekansının yarısı (`fs / 2`). Bu frekansın üstündeki bileşenler doğru temsil edilemez. `fir_alcak_geciren_tasarla`, `fc >= fs / 2` için `false` döner.

**Beyaz gürültü** (*white noise*) · 5
Her frekansta eşit güce sahip rastgele gürültü. Projede ortalaması 0 olan Gauss dağılımıyla üretiliyor.

**Dürtü gürültüsü** (*impulse noise*) · 5
Nadiren ortaya çıkan, büyük genlikli ani sıçramalar.

**Akış işleme** (*stream processing*) · 5.2
Sinyalin tamamını beklemeden, her yeni örneği geldiği anda işleyip bir çıkış örneği üretme. Filtre durumunu (geçmişi) nesne tutar.

**Nedensel filtre** (*causal filter*) · 5.5
Çıkışı yalnızca şimdiki ve geçmiş girişlere bağlı olan filtre. Gerçek zamanlı çalışan her filtre nedensel olmak zorundadır.

**Merkezi filtre** (*non-causal, centered filter*) · 5.5
Çıkışı gelecekteki girişlere de bağlı olan filtre. Yalnızca kaydedilmiş veride uygulanabilir. Ödev 04'teki medyan filtre böyleydi.

**Ring buffer** (*circular buffer*) · 5.3
Sabit kapasiteli, sonu başına bağlıymış gibi kullanılan dizi. Doluyken yeni örnek en eskinin yerine yazılır. Filtrelerde son N örneği tutmak için kullanılır.

**Hareketli ortalama** (*moving average*) · 5.4
Son N örneğin aritmetik ortalaması. Toplam her adımda güncellenerek (çıkan örnek çıkarılıp giren eklenerek) N'den bağımsız sürede hesaplanır. Bütün katsayıları `1/N` olan bir FIR filtreye eşdeğerdir.

**Medyan filtre** (*median filter*) · 5.5
Son N örneğin medyanını (sıralandığında ortadaki değer) veren doğrusal olmayan filtre. Dürtü gürültüsünü etkili biçimde bastırır.

**FIR filtre** (*finite impulse response*) · 5.6
Çıkışı son N girişin ağırlıklı toplamı olan filtre: `y[n] = Σ b[k]·x[n−k]`. Dürtü yanıtı N örnek sonra sıfırlanır. Her zaman kararlıdır.

**Katsayılar** (*coefficients, taps*) · 5.6
FIR filtredeki `b[k]` ağırlıkları. Filtrenin frekans davranışını tamamen belirler. Katsayı sayısına filtre uzunluğu (*tap* sayısı) denir.

**Dürtü yanıtı** (*impulse response*) · 5.6
Filtreye tek bir birim dürtü (bir `1`, ardından sıfırlar) verildiğinde alınan çıkış. FIR filtrede dürtü yanıtı katsayıların kendisidir.

**Pencereli sinc yöntemi** (*windowed-sinc method*) · 5.6
İdeal alçak geçiren filtrenin sonsuz uzunluktaki sinc dürtü yanıtını N örnekte kesip bir pencere fonksiyonuyla çarparak FIR katsayısı tasarlama yöntemi.

**Hamming penceresi** (*Hamming window*) · 5.6
`w[i] = 0.54 − 0.46·cos(2πi / (N−1))`. Sinc'i kesmenin oluşturduğu dalgalanmayı (yan lobları) azaltmak için kullanılır.

**DC kazancı** (*DC gain*) · 5.6
Filtrenin sabit (0 Hz) bir girişi kaç katına çıkardığı. FIR'da katsayıların toplamına eşittir; projede 1'e normalize ediliyor.

**IIR filtre** (*infinite impulse response*) · 5.7
Çıkışı girişlerin yanında önceki çıkışlara da bağlı olan (geri beslemeli) filtre. Dürtü yanıtı teorik olarak sonsuza kadar sürer. Az işlemle güçlü filtreleme sağlar; katsayılar yanlış seçilirse kararsız olabilir.

**Birinci dereceden alçak geçiren IIR** · 5.7
`y[n] = y[n−1] + α·(x[n] − y[n−1])`. `α ∈ (0, 1]`; küçük `α` daha güçlü filtreleme demektir. Analog RC alçak geçiren devresinin ayrık zaman karşılığıdır. Kesim frekansından `α = 1 − e^(−2π·fc/fs)` ile hesaplanır.

**Kesim frekansı** (*cutoff frequency, fc*) · 5.6, 5.7
Alçak geçiren bir filtrenin geçirdiği ve bastırdığı frekansları ayıran sınır frekansı.

**Gecikme** (*delay, group delay*) · 5.9
Filtre çıkışının girişe göre kaç örnek geriden geldiği. Simetrik N katsayılı FIR için tam `(N−1)/2`, N pencereli medyan için `N/2`, birinci dereceden IIR için düşük frekanslarda yaklaşık `(1−α)/α` örnektir.

**Doğrusal faz** (*linear phase*) · 5.6
Bütün frekansların aynı süre geciktirilmesi. Sinyalin dalga şekli korunur. Simetrik katsayılı FIR filtreler doğrusal fazlıdır, IIR filtreler değildir.

**Filtre zinciri** (*cascade*) · 5.8
Filtrelerin arka arkaya bağlanması; her filtrenin çıkışı bir sonrakinin girişidir. Toplam gecikme, halkaların gecikmelerinin toplamıdır.

**RMS hata** (*root mean square error*) · 5.9
İki dizi arasındaki farkın karelerinin ortalamasının karekökü: `√( (1/n)·Σ (a[i] − b[i])² )`. Büyük hataları küçük hatalardan daha ağır cezalandırır.

**Geçici rejim** (*transient*) · 5.9
Filtrenin başlangıçta geçmişi henüz dolmamışken ürettiği, kalıcı davranışını yansıtmayan çıkış. Ölçümlerde ilk örnekler (`ATLA = 100`) bu yüzden hesaba katılmıyor.

**Rastgele sayı tohumu** (*seed*) · 5.9
Sözde rastgele sayı üretecinin başlangıç değeri. Aynı tohum her çalıştırmada aynı diziyi üretir; deneyler bu sayede tekrarlanabilir. `SinyalUreteci` varsayılan olarak 2026 tohumunu kullanıyor.
