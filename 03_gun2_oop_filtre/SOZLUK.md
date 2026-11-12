# Gün 2 Sözlüğü | Çatıdaki Hava İstasyonu

Bu sayfa, ikinci günün bütün kavramlarını tek bir hikâye üzerinden anlatan bir sözlüktür. Metaforsuz, kısa teknik tanımlar isterseniz [TERIMLER.md](TERIMLER.md) sayfasına bakın. Ders notunun ([README.md](README.md)) yerine geçmez, onun yanında durur. Bir kavramı ders notunda okuyup "tamam da bu ne demek şimdi" dediğiniz anda buraya gelin.

Her kavramın yanında üç şey bulacaksınız:

* **Metafor:** Kavramın gündelik hayattaki karşılığı.
* **Hikâyede:** Kavramın aşağıdaki hikâyede nereye denk geldiği.
* **Kodda:** Kavramın C++ hâli, mümkün olduğunca kısa.

Sayfanın en sonunda bütün terimleri alfabetik sırayla gösteren bir [hızlı başvuru tablosu](#hızlı-başvuru-tablosu) var. Bu sayfanın nasıl yazıldığını, yani hikâyeleştirme tekniğini merak ediyorsanız [Hikâyeleştirme Tekniği](#hikâyeleştirme-tekniği-bu-sayfa-nasıl-yazıldı) bölümüne bakın. Yeni bir konuyu kendiniz öğrenirken ya da birine anlatırken aynı yöntemi kullanabilirsiniz.

---

## Hikâyenin Başı

Kulübün odası fakültenin en üst katında. Bir gün başkan bir duyuru yapıyor: "Çatıya bir hava istasyonu kuruyoruz. Sıcaklık, ışık ve gerilim ölçecek, verileri her saniye bin kez okuyacak. Yazılımı da bizden biri yazacak."

Görev ikinci sınıftan **Deniz**'e düşüyor. Deniz birinci günü dinlemiş, ödevleri yapmış; pointer da biliyor, ring buffer da. Yanına mentor olarak üst dönemden **Ekin** veriliyor. Ekin'in tek bir kuralı var: "Kod yazmadan önce bana neyi neye benzettiğini anlat."

Hikâye boyunca Deniz'in istasyonu nasıl kurduğunu izleyeceğiz.

---

## 1. Bölüm: Plan ve Cihaz

Deniz ilk gün çatıya çıkıp üç termometre takmak istiyor. Ödevdeki gibi her biri için bir `struct` açıp fonksiyonları dışarıya yazmaya başlıyor. Ekin durduruyor:

"Fabrika üç termometre üretirken her biri için ayrı ayrı çizim yapar mı? Bir plan çizer, o plandan üç tane basar. Sen de öyle yap."

### Sınıf (class)

> **Metafor:** Fabrikadaki cihaz planı. Kurabiye kalıbı da olur.

**Hikâyede:** "Termometre" planı. İçinde neyi hatırlayacağı (ölçüm sayısı, ofset ayarı) ve hangi düğmelerinin olacağı (oku, sıfırla) yazıyor.

```cpp
class Termometre { ... };
```

### Nesne (object)

> **Metafor:** Plandan basılıp çatıya monte edilen somut cihaz. Kalıptan çıkan kurabiye.

**Hikâyede:** Kuzey köşedeki termometre, güney köşedeki termometre. Aynı plandan çıktılar, lakin her birinin kendi ölçümü var.

```cpp
Termometre kuzey;
Termometre guney;
```

### Üye değişken ve üye fonksiyon (member variable, member function)

> **Metafor:** Üye değişken cihazın **iç hafızası**, üye fonksiyon cihazın **düğmeleri**.

**Hikâyede:** Termometrenin hafızasında `ofset_` yazıyor. Üzerinde de `oku()` düğmesi var. Düğmeye basınca cihaz kendi hafızasına bakıp cevap veriyor.

Önemli bir ayrıntı: Fabrika her cihaza ayrı bir hafıza çipi takıyor, lakin düğmelerin arkasındaki devre şeması hepsinde aynı. **Veriler ayrı, fonksiyonlar ortak.**

### `public` ve `private`

> **Metafor:** `public` cihazın **ön paneli**, herkes basabilir. `private` ise **vidalı arka kapak**, üzerinde "garanti etiketi, açmayın" yazıyor.

**Hikâyede:** Çatıya çıkan herkes `oku()` düğmesine basabilir. Ama termometrenin kalibrasyon ayarına ancak termometrenin kendisi dokunabilir.

```cpp
class Termometre {
public:
    double oku();
private:
    double ofset_;
};

kuzey.ofset_ = 99;   // DERLENMEZ: garanti etiketini koparamazsın
```

`struct` ile `class` arasındaki tek fark: `struct` arka kapağı açık gelir, `class` vidalı.

### `this`

> **Metafor:** Cihazın kendi **seri numarası**. Düğmeye basıldığında cihaz "bana basıldı, kendi hafızama bakayım" der.

**Hikâyede:** Deniz "kuzey.oku()" dediğinde fonksiyon nereden biliyor da güneyin değil kuzeyin ofsetini kullanıyor? Çünkü her çağrıda gizlice cihazın adresi de geliyor. O adresin adı `this`.

```cpp
double oku() { return ham * 0.1 + ofset_; }        // biz böyle yazıyoruz
double oku() { return ham * 0.1 + this->ofset_; }  // derleyici böyle anlıyor
```

### `const` üye fonksiyon

> **Metafor:** Cihazın ekranına **bakmak**. Bakmak cihazı değiştirmez.

**Hikâyede:** "Kaç kez okundun?" sorusu cihazı değiştirmez, `const` olur. "Oku" ise sayacı artırdığı için `const` olamaz.

```cpp
int okuma_sayisi() const { return sayac_; }
```

Kural: Nesneyi değiştirmeyen her fonksiyonun sonuna `const` yazın.

---

## 2. Bölüm: Kurulum ve Söküm

Deniz termometreyi çatıya takıyor, lakin ofset ayarını yapmayı unutuyor. Bir hafta boyunca bütün ölçümler 3 derece yanlış geliyor. Ekin gülüyor: "Ayarı kullanıcıya bırakırsan kullanıcı unutur. Ayarı kurulum ekibine yaptır."

### Yapıcı (constructor)

> **Metafor:** **Kurulum teknisyeni.** Cihaz çatıya ayarsız teslim edilmez. Teknisyen gelir, ayarlar, kontrol eder, ondan sonra cihaz çalışmaya başlar.

**Hikâyede:** Termometre doğar doğmaz ofseti ayarlanmış oluyor. Biri saçma bir değer verirse teknisyen düzeltiyor.

```cpp
Adc(double vref, unsigned bit) : vref_(vref), bit_(bit)
{
    if (bit_ < 1 || bit_ > 16) bit_ = 12;   // 99 bitlik ADC olmaz, düzelt
}
```

Yapıcının asıl görevi: **Geçersiz bir nesne hiç doğmasın.**

### İlk değer listesi (initializer list)

> **Metafor:** Parçaları **fabrikada takılı** gelen cihaz ile kutudan çıkarıp sonradan takılan cihaz arasındaki fark.

**Hikâyede:** `: vref_(vref), bit_(bit)` kısmı, parçaların cihaz kutudan çıkmadan yerine takılması. Gövdenin içinde atama yapmak ise cihazı önce boş kurup sonra parçaları tek tek değiştirmek gibi.

⚠️ Bir tuzak: Fabrika parçaları **sizin listeye yazdığınız sırayla değil, planda çizildikleri sırayla** takar. Bir parça diğerine bağlıysa, planda (sınıf tanımında) önce o yazılmalı.

### Yıkıcı (destructor)

> **Metafor:** **Söküm ekibi.** Cihazı çatıdan indirirken kablolarını toplar, pilini çıkarır, arkasında çöp bırakmaz.

**Hikâyede:** Deniz bir termometreyi söktüğünde cihaz son bir kez "kapandım" diye log yazıyor.

```cpp
~Termometre() { std::cout << "kapandim\n"; }
```

### Nesnenin ömrü: stack ve heap

> **Metafor:**
> * **Stack**, toplantı odasındaki **geçici masa**. Toplantı (blok) bitince görevliler masayı kendiliğinden toplar.
> * **Heap**, şehir dışında **kiralık depo**. Siz "iade ediyorum" (`delete`) diyene kadar kira işlemeye devam eder. Unutursanız sonsuza kadar öder.

**Hikâyede:** Deniz bir fonksiyonun içinde termometre oluşturuyor, fonksiyon bitince termometre de gidiyor. `new` ile oluşturduğu ise Deniz `delete` diyene kadar yaşıyor.

Bir de sıra meselesi var: Masalar **son kurulan ilk toplanır** düzeniyle kaldırılır. Üst üste dizilmiş tabaklar gibi.

---

## 3. Bölüm: Arka Kapağın Vidaları

İstasyon büyüdü. Artık son ölçümleri tutan bir ring buffer var. Bir gün kulüpten biri hata ayıklarken ring buffer'ın `adet` alanına elle 50 yazıyor. Tamponun kapasitesi 8. İstasyon o geceden sonra saçmalamaya başlıyor ve sorunun nerede olduğunu bulmak iki gün sürüyor.

### Kapsülleme (encapsulation)

> **Metafor:** Bankadaki **kasa ve kasiyer.** Kasadaki paraya müşteri elini sokamaz. Para yatırmak ya da çekmek isteyen kasiyere gider. Kasiyer her işlemi deftere yazar.

**Hikâyede:** Ring buffer'ın `bas_` ve `adet_` alanları kasadaki para. Onlara yalnızca `ekle()` ve `cikar()` dokunabiliyor. Artık kimse `adet_ = 50` yazamıyor.

### Değişmez (invariant)

> **Metafor:** Bankanın altın kuralı: **"Kasadaki para, defterdeki toplamla her an aynı olmalı."**

**Hikâyede:** Ring buffer'ın kuralı: "`adet_` hiçbir zaman kapasiteyi geçmez." Alanlar `private` olunca bu kuralı bozabilecek kişi sadece kasiyerler (sınıfın kendi fonksiyonları) kalıyor. Bir sorun çıkarsa bütün projeyi değil, bir avuç fonksiyonu kontrol etmek yetiyor.

Kapsüllemenin amacı saklamak değil, **kuralı korumaktır.** O yüzden her alana bir `set_...()` fonksiyonu açmak, kasanın kapağına "herkes elini sokabilir" deliği açmakla aynıdır.

### Arayüz (interface)

> **Metafor:** Cihazın **kullanım kılavuzu.** Hangi düğme ne yapar, onu söyler. İçerideki devreyi anlatmaz.

**Hikâyede:** Ring buffer'ın arayüzü `ekle`, `bak`, `adet`, `dolu`, `bos`. İçeride bir dizi mi var, bağlı liste mi, kullanıcıyı ilgilendirmiyor.

### Başlık ve kaynak dosyası (`.hpp` ve `.cpp`)

> **Metafor:** `.hpp` dosyası **datasheet**, `.cpp` dosyası **devre kartının kendisi**.

**Hikâyede:** Kulüpten başka biri istasyona yeni bir özellik ekleyecekse sadece `ring_buffer.hpp` dosyasını okuması yetiyor. Gövdeleri merak ederse `ring_buffer.cpp` dosyasına bakıyor.

```cpp
// .cpp içinde gövde yazarken hangi cihazın düğmesi olduğunu belirtmek şart:
void RingBuffer::ekle(float deger) { ... }
```

### `const` referans (`const&`)

> **Metafor:** Cihazı **müze vitrininde** göstermek. İsteyen gelip bakar, lakin cama dokunamaz. Üstelik kopyasını da yapmak gerekmez, asıl cihaz orada duruyor.

**Hikâyede:** Deniz istasyonun durumunu ekrana yazdıran bir fonksiyon yazıyor. Fonksiyona ring buffer'ı `const RingBuffer&` olarak veriyor: kopyalanmıyor, değiştirilemiyor.

```cpp
void yazdir(const RingBuffer& rb);
```

Kısa kural: Küçük şeyleri (`int`, `float`) elden ele verin, büyük nesneleri vitrinde gösterin (`const&`), değiştirilecek nesneleri anahtarıyla teslim edin (`&`).

---

## 4. Bölüm: Depo Anahtarı

İstasyon artık farklı uzunlukta geçmiş tutan filtreler istiyor. Kimine 5 örnek yetiyor, kimine 61. Deniz belleği `new` ile kiralamaya başlıyor. Bir gece sensör hata verip fonksiyon erken dönüyor ve `delete` satırına hiç gelinmiyor. Bir hafta sonra istasyonun belleği dolup çöküyor.

### RAII

> **Metafor:** Otel odasının **anahtar kartı.** Kartı yuvaya takınca ışıklar, klima, priz açılır. Kartı çıkarıp çıktığınız an hepsi kendiliğinden kapanır. Kapıdan mı çıktınız, balkondan mı kaçtınız fark etmez.

**Hikâyede:** Deniz belleği bir nesneye emanet ediyor. Nesne doğarken (yapıcı) belleği kiralıyor, ölürken (yıkıcı) iade ediyor. Fonksiyon hangi `return` satırından çıkarsa çıksın nesne ölüyor ve bellek geri veriliyor.

```cpp
class DinamikTampon {
public:
    explicit DinamikTampon(size_t n) : veri_(new float[n]) {}   // kartı tak
    ~DinamikTampon() { delete[] veri_; }                        // kartı çek
private:
    float* veri_;
};
```

İsmi korkutucu: *Resource Acquisition Is Initialization*. Fikri basit: **Yapıcıda al, yıkıcıda bırak.**

### Kopyalama tuzağı ve çift silme (double free)

> **Metafor:** Bir deponun anahtarından **ikinci bir kopya** çıkarıp iki kişiye veriyorsunuz. İkisi de "depoyu iade ediyorum" diyor. İlki iade ediyor, depo başkasına kiralanıyor. İkincisi gelip **başkasının deposunu** boşaltıyor.

**Hikâyede:** Deniz bir tamponu kopyalıyor. Derleyici depoyu değil, sadece anahtarı (pointer'ı) kopyalıyor. İki nesne de ölürken aynı belleği siliyor ve istasyon çöküyor.

### `= delete` (kopyalamayı yasaklamak)

> **Metafor:** Anahtarın üzerine kazınmış **"Kopyalanamaz"** damgası. Çilingir daha anahtarı eline aldığı anda reddeder.

```cpp
DinamikTampon(const DinamikTampon&) = delete;
DinamikTampon& operator=(const DinamikTampon&) = delete;
```

Artık hata gece yarısı çökme olarak değil, sabah derleme hatası olarak çıkıyor.

### Üçler kuralı (rule of three)

> **Metafor:** Depo kiraladıysanız üç şeyi birlikte düşünmeniz gerekir: **iade** nasıl olacak (yıkıcı), anahtar **kopyalanırsa** ne olacak (kopyalama yapıcısı), biri **başka deponun anahtarını** sizinkinin yerine koyarsa ne olacak (atama operatörü).

Yıkıcı yazdıysanız diğer ikisini ya düzgünce yazın ya da yasaklayın.

### `explicit`

> **Metafor:** Kapıdaki **dikkatli görevli.** "50" yazan bir kâğıt gösterene "bu bir kimlik değil" der, içeri almaz.

**Hikâyede:** `DinamikTampon t = 50;` satırı, `explicit` olmasaydı sessizce 50 elemanlı bir tampon açardı. Görevli buna izin vermiyor.

### Bileşim (composition, "has-a")

> **Metafor:** Bir araba **motora sahiptir.** Araba motor değildir, ama motorsuz da çalışmaz. Araba hurdaya çıkınca motor da onunla birlikte gider, kimsenin ayrıca söylemesine gerek yoktur.

**Hikâyede:** `HareketliOrtalama` filtresinin içinde bir `RingBuffer` var. Filtre öldüğünde ring buffer da ölüyor ve belleğini iade ediyor. Filtreye tek satır yıkıcı yazmak gerekmiyor. Anahtar kartı mantığını bir kere doğru kurdunuz mu, onu kullanan herkes bedavaya faydalanıyor.

---

## 5. Bölüm: Sensör Ailesi

İstasyona ışık sensörü ve gerilim sensörü de ekleniyor. Deniz her biri için ad, okuma sayısı, rapor fonksiyonu gibi aynı kodları üçüncü kez yazarken Ekin yine durduruyor: "Bunların hepsinin ortak bir soyadı yok mu?"

### Kalıtım (inheritance, "is-a")

> **Metafor:** **Aile ve soyadı.** Bütün sensörler "Sensör" ailesinden gelir. Aileden gelen her şeyi (adı olması, okunma sayısı tutması) miras alırlar, üstüne kendi yeteneklerini eklerler.

**Hikâyede:** Sıcaklık sensörü **bir** sensördür. Işık sensörü de **bir** sensördür.

```cpp
class SicaklikSensoru : public Sensor { ... };
```

Bir ev gibi düşünün: **önce temel atılır** (taban sınıfın yapıcısı), sonra kat çıkılır (türetilmiş sınıfın yapıcısı). Yıkımda sıra terstir: önce kat, sonra temel.

Bileşimle karıştırmayın: Filtre bir ring buffer **değildir**, bir ring buffer'a **sahiptir**. Emin değilseniz bileşimi seçin.

### `protected`

> **Metafor:** **Aile tarifi.** Yabancılara verilmez, lakin çocuklar ve torunlar bilir.

**Hikâyede:** `okuma_sayisi_` alanı dışarıdan değiştirilemiyor, ama sıcaklık sensörü kendi `oku()` fonksiyonunda onu artırabiliyor.

### Statik bağlama (static binding)

> **Metafor:** Zarfın **üstündeki yazıya** bakıp karar veren postacı. Zarfta "Sensör" yazıyorsa içinde ne olduğuna bakmaz, genel sensör kutusuna atar.

**Hikâyede:** Deniz bütün sensörleri `Sensor*` ile gösteriyor. `s->birim()` diyor, "C" bekliyor, "?" geliyor. Derleyici sadece pointer'ın tipine (zarfın üstüne) baktı.

### `virtual` (sanal fonksiyon)

> **Metafor:** **Akıllı santral.** "Sensör hattını" aradığınızda santral, o hatta şu an gerçekte hangi cihazın bağlı olduğuna bakar ve çağrıyı ona aktarır.

**Hikâyede:** `birim()` fonksiyonu `virtual` olunca `s->birim()` çağrısı nesnenin gerçek tipine gidiyor. Sıcaklık sensörü "C", ışık sensörü "lux" diyor.

### vtable

> **Metafor:** Santralin **rehberi.** Her cihaz tipinin "birim sorulursa şu numaraya bağla" yazan kendi sayfası var.

Bedeli küçük: her nesnede rehbere giden bir işaret ve her çağrıda bir rehber bakışı.

### `override`

> **Metafor:** Aile düğmesinin **kendi versiyonunu** yaptığınızı resmen beyan etmek. Beyanname yanlış doldurulmuşsa (imza tutmuyorsa) memur reddeder.

```cpp
const char* birim() const override { return "C"; }
// const unutulursa: DERLENMEZ, iyi ki derlenmez
```

### Saf sanal fonksiyon (pure virtual, `= 0`)

> **Metafor:** **İş ilanındaki zorunlu madde.** "Adayın İngilizce bilmesi şarttır." İlanı veren şirket İngilizceyi kendisi konuşmaz, adayın konuşmasını şart koşar.

```cpp
virtual double oku(int ham_kod) = 0;
```

### Soyut sınıf (abstract class)

> **Metafor:** **İş ilanının kendisi.** İlanı işe alamazsınız, ancak ilandaki şartları karşılayan bir adayı alabilirsiniz.

**Hikâyede:** `Sensor s;` derlenmiyor. "Sensör" diye genel bir cihaz yok. Ama şartları yerine getiren sıcaklık sensörü var.

Final projesindeki `Filtre` sınıfı tam bir iş ilanı: "Filtre olmak isteyen herkes `isle`, `sifirla` ve `ad` yapabilmeli."

### Çok biçimlilik (polymorphism)

> **Metafor:** **Orkestra şefi.** Şef çubuğunu kaldırıp "çal!" der. Keman kemanca, davul davulca çalar. Şefin her çalgıya ayrı ayrı nasıl çalınacağını anlatması gerekmez.

**Hikâyede:**

```cpp
Sensor* sensorler[] = {&sicaklik, &isik, &gerilim};
for (Sensor* s : sensorler) s->rapor(...);
```

Yarın istasyona nem sensörü eklense döngüye dokunmak gerekmiyor. Orkestraya yeni bir çalgıcı katılınca şef hareketini değiştirmez.

### Sanal yıkıcı (virtual destructor)

> **Metafor:** Söküm ekibine sadece "bu bir sensör" deyip gönderirseniz genel sensör sökümünü yaparlar ve **içindeki özel pili unuturlar.** Ekibin kutunun içine bakması lazım.

```cpp
virtual ~Sensor() = default;
```

Kural: Sanal fonksiyonu olan sınıfın yıkıcısı da sanal olsun.

### `static` üye fonksiyon

> **Metafor:** Fabrikanın duvarındaki **hesap tablosu.** Herhangi bir cihaza ihtiyaç duymadan bakılır.

**Hikâyede:** IIR filtrenin `alfa_hesapla(fc, fs)` fonksiyonu bir nesneye ihtiyaç duymuyor, `IIRAlcakGeciren::alfa_hesapla(10, 1000)` diye sınıf adıyla çağrılıyor.

---

## 6. Bölüm: Fırtınalı Gece

Her şey yerinde. Bir gece fırtına çıkıyor. Sabah Deniz verilere bakınca dehşete düşüyor: sıcaklık eğrisi tırtıl gibi titriyor ve arada bir 50 dereceye fırlıyor. Ekin kahvesini yudumlayıp diyor ki: "Hoş geldin, gerçek dünya. Şimdi filtre zamanı."

### Örnekleme (sampling)

> **Metafor:** Bir yarışı **saniyede bin kare** çeken bir fotoğraf makinesi. Arada ne olduğunu bilemezsiniz, sadece karelere bakarsınız.

**Hikâyede:** İstasyon sensörü saniyede 1000 kez okuyor. Örnekleme frekansı `fs = 1000 Hz`.

### Beyaz gürültü ve dürtü gürültüsü (white noise, impulse noise)

> **Metafor:**
> * **Beyaz gürültü** kalabalık bir kafedeki **sürekli uğultu.** Her an orada, ama hiçbiri yüksek değil.
> * **Dürtü gürültüsü** biri **tepsiyi düşürdüğünde** çıkan gürültü. Nadiren olur ama kulak tırmalar.

**Hikâyede:** Titreme beyaz gürültü (ısıl gürültü, kuantizasyon). 50 dereceye fırlamalar dürtü gürültüsü (rüzgârda gevşeyen konnektör).

### Akış işleme (streaming)

> **Metafor:** **Konveyör bant.** Paket geldikçe işlenir. Bütün paketlerin gelmesi beklenmez.

**Hikâyede:** Filtreler her `isle()` çağrısında bir örnek alıp bir örnek veriyor. Gerçek istasyonda bu, ADC kesmesi her geldiğinde olur.

### Ring buffer

> **Metafor:** **Döner suşi bandı.** Bantta sabit sayıda tabak yeri var. Yeni tabak gelince en eski tabak banttan iner.

**Hikâyede:** Filtrelerin "son N örneği" hatırladığı yer.

### Hareketli ortalama (moving average)

> **Metafor:** Tek bir kişinin sözüne değil, **son 21 kişinin ortalama görüşüne** göre karar vermek. Biri saçmalasa bile etkisi 21'de bire iner.

**Hikâyede:** Uğultuyu (beyaz gürültü) çok güzel bastırıyor. Ama tepsi düştüğünde (dürtü) o gürültü ortalamaya bulaşıyor ve 21 örnek boyunca küçük bir tümsek bırakıyor.

Hile: Her seferinde 21 kişiyi baştan saymak yerine, **giden kişiyi çıkarıp geleni ekliyoruz.**

### Medyan filtre (median filter)

> **Metafor:** **Jüri puanlaması.** Jürinin puanları sıraya dizilir, ortadaki puan alınır. Bir jüri üyesi 0, biri 100 verse de sonucu değiştiremez.

**Hikâyede:** 50 dereceye fırlamaları tamamen siliyor. Sıralanmış dizinin ortasına bir uç değer asla gelemiyor.

### FIR filtre

> **Metafor:** Son N malzemeyi **belirli oranlarla karıştıran bir tarif.** Katsayılar tarifin oranları: "Son örnekten %20, bir öncekinden %15..."

**Hikâyede:** Deniz 61 maddelik bir tarif hazırlıyor (61 katsayı). Tarifi elle yazmak yerine `fir_alcak_geciren_tasarla` fonksiyonu hesaplıyor.

```
y[n] = b[0]·x[n] + b[1]·x[n−1] + ... + b[60]·x[n−60]
```

Hareketli ortalama da aslında bir FIR'dır: bütün malzemeleri **eşit oranda** karıştıran bir tarif.

### Dürtü yanıtı (impulse response)

> **Metafor:** Boş bir salonda **bir kez el çırpınca** duyduğunuz yankı. Salonun akustiği hakkında her şeyi anlatır.

**Hikâyede:** FIR'a tek bir 1 verip ardından sıfırlar verince çıkışta katsayılar sırayla görünüyor. FIR'da yankı N adımda **kesin olarak** biter. Adındaki "sonlu" (finite) buradan geliyor.

### IIR filtre

> **Metafor:** **Kendi önceki fikrine güvenen, yeni haberleri azar azar kabul eden** biri. Her yeni haberde fikrini biraz o yöne kaydırır ama tamamen değiştirmez. Ya da: sıcak bir çay bardağının **yavaş yavaş oda sıcaklığına gelmesi.**

**Hikâyede:**

```
y = y + α · (x − y)
```

`α` ne kadar açık fikirli olduğu: `α = 1` her habere anında inanır (filtre yok), `α` küçüldükçe daha temkinli olur. Laboratuvardaki **RC devresinin** dijital karşılığı. Bütün geçmişi tek bir sayıda (`y`) tutuyor, yani çok ucuz.

Dürtü yanıtı hiç tam bitmez, sönümlenerek sonsuza kadar sürer. Adındaki "sonsuz" (infinite) buradan.

### Kesim frekansı (cutoff frequency)

> **Metafor:** **Eleğin göz büyüklüğü.** Gözden küçük (yavaş) olanlar geçer, büyük (hızlı) olanlar takılır.

**Hikâyede:** Sıcaklık yavaş değişiyor (5 Hz), titreme hızlı. Kesim frekansını 10 ile 15 Hz arasına koyunca sıcaklık geçiyor, titreme takılıyor.

---

## 7. Bölüm: Arıtma Hattı

Deniz bütün filtreleri deniyor, hiçbiri tek başına mükemmel değil. Ortalama uğultuyu alıyor ama tepsiyi kaçırıyor, medyan tepsiyi yakalıyor ama uğultuda o kadar iyi değil. Ekin bir su arıtma tesisinin fotoğrafını gösteriyor.

### Filtre zinciri (Composite kalıbı)

> **Metafor:** **Su arıtma tesisi.** Önce kaba ızgara taşları ve dalları alır, ardından ince kum filtresi bulanıklığı temizler. Her aşama en iyi yaptığı işi yapar.

**Hikâyede:** Önce 5'lik medyan sıçramaları (taşları) alıyor, ardından 21'lik ortalama uğultuyu (kumu) temizliyor. Bütün filtreler arasında birinciliği bu zincir alıyor.

Zincirin kendisi de bir filtre. Yani bir arıtma tesisini, daha büyük bir tesisin aşamalarından biri yapabilirsiniz.

### Sahiplik (ownership)

> **Metafor:** **Konferans programı.** Programda konuşmacıların adı yazar, lakin programı hazırlayan kişi konuşmacıları işe almadı, işten de çıkaramaz. Konuşmacı gelmezse program çöker.

**Hikâyede:** Zincir filtrelerin pointer'larını tutuyor ama onları silmiyor. Filtreler zincirden önce ölmemeli. Bir pointer'a bakarak onu kimin sileceği anlaşılmaz, bu yüzden sahipliği başlık dosyasında yazıyla belirtiyoruz.

### Gecikme (delay)

> **Metafor:** **Gök gürültüsü.** Şimşeği görürsünüz, sesi birkaç saniye sonra gelir. Ya da canlı yayının birkaç saniye geriden gelmesi.

**Hikâyede:** 21 kişinin ortalamasını almak için 21 kişiyi dinlemek gerekiyor ve haberin "ortası" 10 kişi önceye ait. Yani ortalama 10 örnek geriden geliyor. 61 katsayılı FIR 30 örnek geriden geliyor. Deniz ilk ölçümde FIR'ın hatasını filtrelenmemiş sinyalinkinden bile büyük buluyor. Hatanın sebebi gürültü değil, gecikme.

### Nedensellik (causality)

> **Metafor:** **Falcı olamazsınız.** Yarın ne olacağını bilmeden bugün karar vermek zorundasınız.

**Hikâyede:** Ödev 04'teki medyan filtre, kaydedilmiş bir dosyada çalıştığı için "gelecekteki" örneklere de bakabiliyordu. Çatıdaki istasyon gerçek zamanlı çalışıyor, geleceği bilemiyor. Bu yüzden filtreleri nedensel yazdık ve bedeli gecikme oldu.

### Doğrusal faz (linear phase)

> **Metafor:** Bir koronun bütün seslerinin **aynı gecikmeyle** yankılanması. Şarkı biraz geç gelir ama bozulmaz. Bası bir saniye, tizi üç saniye geç gelseydi şarkı tanınmaz hâle gelirdi.

**Hikâyede:** Simetrik katsayılı FIR doğrusal fazlıdır, sinyalin şeklini korur. IIR değildir, bu yüzden gecikmesi tek bir sayıyla tam telafi edilemiyor.

### RMS hata

> **Metafor:** Hataların **dürüst ortalaması.** Küçük hatalara hafif, büyük hatalara ağır ceza verir (karesini aldığı için).

**Hikâyede:** Deniz her filtrenin çıkışını temiz sinyalle karşılaştırıp RMS hatayı hesaplıyor. Bir de gecikmeyi telafi ederek ölçüyor. Asıl temizleme başarısı ancak o zaman görünüyor.

---

## Hikâyenin Sonu

Bahar geldiğinde istasyon bir aydır kesintisiz çalışıyor. Deniz son raporunu Ekin'e gösteriyor ve Ekin her zamanki sorusunu soruyor: "Peki en iyi filtre hangisi?"

Deniz gülümsüyor: "En iyi filtre yok. Gürültünün türüne, gecikmeye ne kadar tahammülüm olduğuna ve işlemcinin gücüne bakıp seçiyorum. Bizim istasyonda sıcaklık için 12 örneklik gecikme kimsenin umurunda değil, o yüzden medyan ve ortalama zinciri. Motor kontrol etseydik başka bir şey seçerdim."

Ekin başını sallıyor: "Artık hazırsın."

Hikâyedeki her şeyin gerçek kodu bu klasörde:

| Hikâyede | Dosya |
|---|---|
| Plan ve cihaz, `this` | [01_struct_class.cpp](01_oop_temelleri/01_struct_class.cpp) |
| Kurulum ve söküm | [02_yapici_yikici.cpp](01_oop_temelleri/02_yapici_yikici.cpp) |
| Kasa ve kasiyer | [03_kapsulleme.cpp](01_oop_temelleri/03_kapsulleme.cpp) |
| Otel kartı ve depo anahtarı | [04_raii.cpp](01_oop_temelleri/04_raii.cpp) |
| Sensör ailesi | [05_kalitim.cpp](01_oop_temelleri/05_kalitim.cpp), [06_sanal_fonksiyonlar.cpp](01_oop_temelleri/06_sanal_fonksiyonlar.cpp) |
| Fırtınalı gece ve arıtma hattı | [02_filtre_kutuphanesi](02_filtre_kutuphanesi) |

---

## Hikâyeleştirme Tekniği: Bu Sayfa Nasıl Yazıldı?

Bu sayfayı okuyup "metafor bulmak yetenek işi" diye düşünmeyin. Arkasında herkesin uygulayabileceği bir yöntem var. Yeni bir konuyu öğrenirken kendinize anlatmak, sınava çalışırken notlarınızı düzenlemek ya da kulüpte bir sonraki dönemin öğrencilerine ders vermek için kullanabilirsiniz.

### Neden işe yarıyor?

Beynimiz tek başına duran bir tanımı ekseriya unutur. "RAII: Resource Acquisition Is Initialization" cümlesi hafızada tutunacak bir şey bulamaz. Otel kartı ise zaten orada duruyor: kartı takınca ışıkların yandığını, çıkarınca söndüğünü biliyorsunuz. Metafor, **yeni bilgiyi zaten bildiğiniz bir şeye bağlar.** Yeni bilgi sıfırdan bir raf kurmak yerine var olan bir rafa yerleşir.

Hikâye ise kavramları birbirine bağlar. Tek tek metaforlar bir kelime listesi gibidir. Hikâyede ise her kavram bir **sorunun cevabı** olarak doğar: Deniz ofseti ayarlamayı unuttu, o yüzden yapıcıya ihtiyaç duyduk. Biri `adet`'e 50 yazdı, o yüzden kapsülleme geldi. Bir kavramın **neden var olduğunu** bilmek, onu ne zaman kullanacağınızı da söyler. Sınavda ya da projede işe yarayan bilgi tam olarak budur.

### Yedi adımda hikâyeleştirme

**1. Tek bir dünya seçin.** Her kavram için ayrı bir metafor dünyası açarsanız okuyucu her paragrafta yeniden yön bulmak zorunda kalır. Bu sayfada her şey bir hava istasyonunun etrafında dönüyor. Dünyayı seçerken okuyucunun **zaten tanıdığı** bir yer seçin. Elektronik öğrencileri için çatıdaki bir sensör, bir laboratuvar, bir devre kartı iyi seçimlerdir.

**2. İki karakter yeter: öğrenen ve rehber.** Deniz hata yapar, Ekin soru sorar. Öğrenen karakter okuyucunun yerine geçer, okuyucunun yapacağı hataları onun yerine yapar. Rehber ise cevabı doğrudan vermez, doğru soruyu sorar ("Fabrika üç termometre için üç ayrı çizim yapar mı?"). Böylece okuyucu cevabı bir an için kendisi bulmuş gibi hisseder.

**3. Her kavramı bir acıdan doğurun.** Kavramı tanıtmadan önce, o kavram **olmasaydı** ne kötü olacağını gösterin. Önce sızıntı, sonra RAII. Önce yanlış birim, sonra `virtual`. Çözümü sorundan önce gösterirseniz okuyucu "bu neden lazım ki?" diye sorar ve ilgisini kaybeder.

**4. Metaforu kavramın yapısına göre seçin, yüzeyine göre değil.** İyi bir metafor kavramla aynı **parçalara ve ilişkilere** sahiptir. Bir metafor seçince küçük bir eşleme tablosu yapın:

| RAII'deki parça | Otel kartındaki karşılığı |
|---|---|
| Kaynak (bellek, dosya) | Işıklar, klima, prizler |
| Yapıcı: kaynağı alır | Kartı yuvaya takmak |
| Yıkıcı: kaynağı bırakır | Kartı çekip çıkmak |
| Hangi `return` ile çıkılırsa çıkılsın yıkıcı çalışır | Kapıdan da çıksanız balkondan da kaçsanız ışıklar söner |
| `delete` yazmayı unutmak imkânsız | Işıkları kapatmayı unutmak imkânsız |

Tablonun her satırı dolduysa metafor sağlamdır. Doldurmakta zorlandığınız satırlar varsa ya metaforu değiştirin ya da o satırı açıkça "burada benzemiyor" diye not edin.

**5. Metaforun kırıldığı yeri söyleyin.** Hiçbir metafor kusursuz değildir. Kırıldığı yeri gizlerseniz okuyucu yanlış bir şey öğrenir. Bu sayfada ve derste dikkat ettiğimiz birkaç kırılma noktası:

* **Kurabiye kalıbı:** Kalıptan çıkan her kurabiyenin kendi hamuru vardır, buraya kadar doğru. Lakin nesnelerin **fonksiyonları ortaktır**, kurabiyelerin "düğmeleri" yok. Bu yüzden 1. bölümde kalıbın yanına bir de "düğmelerin arkasındaki devre şeması hepsinde aynı" cümlesini ekledik.
* **Kiralık depo:** Gerçek bir depoyu iade etmeyi unutursanız size fatura gelir, haberiniz olur. Bellek sızıntısında ise kimse size haber vermez, program sessizce şişer. Metaforun bu farkı söylemesi gerekir.
* **Soğuyan çay:** IIR filtrenin üstel yaklaşımını iyi anlatır, lakin çayın "sıcaklık girişi" sabittir. Gerçek filtrede giriş her an değişir. Bu yüzden IIR için ikinci bir metafor daha verdik: yeni haberleri azar azar kabul eden kişi.

**6. Her metaforu hemen koda bağlayın.** Metafor bir köprüdür, varış noktası değildir. Okuyucu "otel kartı" kısmında kalırsa C++ öğrenmemiş olur. Bu yüzden her kavramın altında **Kodda** kısmı var, hikâyenin sonunda da her bölümün gerçek dosyasına giden bir tablo. Metaforu okuyup koda geçtiğinizde "aa, bu kartı takmak demekmiş" demeniz gerekiyor.

**7. Hikâyeyi bir sonuçla kapatın.** Son sahne bütün kavramları tek bir cümlede toplamalı. Bu sayfada o cümle Deniz'in "en iyi filtre yok" cevabı. Okuyucu hikâyeyi unutsa bile o cümleyi hatırlar ve o cümleden geriye doğru diğer kavramları çıkarabilir.

### Sık yapılan hatalar

* **Metaforu fazla uzatmak.** Otel kartından otelin resepsiyonuna, oradan oda servisine geçerseniz okuyucu hangi parçanın C++'ta neye karşılık geldiğini kaybeder. Metafor kavramı anlatmaya yettiği an durun.
* **Okuyucunun bilmediği bir metafor seçmek.** Hiç otelde kalmamış biri için otel kartı yeni bir kavramdır. Metaforu okuyucuya göre seçin, kendinize göre değil.
* **Her şeyi metaforla anlatmaya çalışmak.** Bazı kavramlar doğrudan anlatıldığında daha açıktır. `#pragma once` için hikâye uydurmaya gerek yok, "başlık iki kez eklenmesin diye" demek kâfi.
* **Hikâyeyi teknik doğruluğun önüne koymak.** Hikâye güzel olsun diye kavramı çarpıtmayın. Şüphedeyseniz önce kodu çalıştırıp doğrulayın, sonra hikâyeyi ona göre yazın. Bu sayfadaki bütün sayılar örnek programın gerçek çıktısından alındı.

### Sıra sizde

Gün 2'de değinip geçtiğimiz kavramlardan birini seçin ve yukarıdaki yedi adımla hava istasyonu hikâyesine yeni bir bölüm yazın. Birkaç fikir:

* **Şablon (`template`):** Deniz'in ring buffer'ı sadece `float` tutuyor, nem sensörü ise `int` veriyor. Aynı kodu her tip için tekrar mı yazacak?
* **`std::unique_ptr`:** Heap'teki bir filtrenin sahibi tek bir nesne olsun ve o nesne ölünce filtre de silinsin. Hangi metafor? (İpucu: devredilebilen ama kopyalanamayan bir şey.)
* **Kuantizasyon:** Ödev 02'deki ADC. Sürekli bir değeri basamaklara yuvarlamak neye benzer?

Önce eşleme tablosunu doldurun, sonra metaforun kırıldığı yeri bulun, en son hikâyeyi yazın. Beğendiğiniz bir bölüm çıkarsa Pull Request açın, bu sayfaya ekleyelim.

---

## Hızlı Başvuru Tablosu

| Terim | İngilizcesi | Tek cümlelik metafor | Bölüm |
|---|---|---|:---:|
| Akış işleme | streaming | Paket geldikçe işleyen konveyör bant | 6 |
| Arayüz | interface | Cihazın kullanım kılavuzu | 3 |
| Başlık dosyası | header (`.hpp`) | Datasheet | 3 |
| Beyaz gürültü | white noise | Kafedeki sürekli uğultu | 6 |
| Bileşim | composition | Araba motora sahiptir | 4 |
| Çift silme | double free | Aynı depoyu iki kez iade etmek | 4 |
| Çok biçimlilik | polymorphism | "Çal!" diyen orkestra şefi | 5 |
| Değişmez | invariant | "Kasa ile defter her an tutmalı" kuralı | 3 |
| Doğrusal faz | linear phase | Bütün seslerin aynı gecikmeyle yankılandığı koro | 7 |
| Dürtü gürültüsü | impulse noise | Yere düşen tepsi | 6 |
| Dürtü yanıtı | impulse response | Boş salonda bir kez el çırpınca gelen yankı | 6 |
| `explicit` | explicit | "50 bir kimlik değil" diyen görevli | 4 |
| FIR filtre | finite impulse response | Son N malzemeyi oranlarla karıştıran tarif | 6 |
| Filtre zinciri | composite | Su arıtma tesisi | 7 |
| Gecikme | delay | Şimşekten sonra gelen gök gürültüsü | 7 |
| Hareketli ortalama | moving average | Son 21 kişinin ortalama görüşü | 6 |
| IIR filtre | infinite impulse response | Yavaş yavaş soğuyan çay | 6 |
| İlk değer listesi | initializer list | Parçaları fabrikada takılı gelmek | 2 |
| Kalıtım | inheritance | Aile ve soyadı | 5 |
| Kapsülleme | encapsulation | Kasa ve kasiyer | 3 |
| Kaynak dosyası | source (`.cpp`) | Devre kartının kendisi | 3 |
| Kesim frekansı | cutoff frequency | Eleğin göz büyüklüğü | 6 |
| `const` referans | const reference | Müze vitrini | 3 |
| `const` üye fonksiyon | const member function | Ekrana bakmak, düğmeye basmamak | 1 |
| Kopyalama tuzağı | shallow copy | Depo anahtarının kopyası | 4 |
| Medyan filtre | median filter | Jüri puanlamasında ortadaki puan | 6 |
| Nedensellik | causality | Falcı olamazsınız | 7 |
| Nesne | object | Plandan basılıp monte edilen cihaz | 1 |
| Ömür (heap) | heap lifetime | Siz iade edene kadar kirası işleyen depo | 2 |
| Ömür (stack) | stack lifetime | Toplantı bitince toplanan masa | 2 |
| Örnekleme | sampling | Saniyede bin kare çeken fotoğraf makinesi | 6 |
| `override` | override | "Kendi versiyonumu yaptım" beyannamesi | 5 |
| `protected` | protected | Aile tarifi | 5 |
| `public` / `private` | access specifiers | Ön panel ve vidalı arka kapak | 1 |
| RAII | RAII | Otel odasının anahtar kartı | 4 |
| Ring buffer | ring buffer | Döner suşi bandı | 6 |
| RMS hata | RMS error | Büyük hataya ağır ceza veren dürüst ortalama | 7 |
| Sahiplik | ownership | Konuşmacıları işe alamayan konferans programı | 7 |
| Sanal fonksiyon | virtual function | Çağrıyı gerçek cihaza aktaran santral | 5 |
| Sanal yıkıcı | virtual destructor | İçindeki pili unutmayan söküm ekibi | 5 |
| Saf sanal fonksiyon | pure virtual | İş ilanındaki zorunlu madde | 5 |
| Sınıf | class | Fabrikadaki cihaz planı | 1 |
| Soyut sınıf | abstract class | İşe alınamayan iş ilanı | 5 |
| Statik bağlama | static binding | Zarfın üstüne bakan postacı | 5 |
| `static` üye fonksiyon | static member function | Fabrikanın duvarındaki hesap tablosu | 5 |
| `this` | this | Cihazın kendi seri numarası | 1 |
| Üçler kuralı | rule of three | Depoda iade, kopya ve değiş tokuş birlikte düşünülür | 4 |
| Üye değişken | member variable | Cihazın iç hafızası | 1 |
| Üye fonksiyon | member function | Cihazın düğmeleri | 1 |
| vtable | vtable | Santralin rehberi | 5 |
| Yapıcı | constructor | Kurulum teknisyeni | 2 |
| Yıkıcı | destructor | Söküm ekibi | 2 |
| `= delete` | deleted function | "Kopyalanamaz" damgalı anahtar | 4 |
