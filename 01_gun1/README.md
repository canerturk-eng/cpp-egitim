# Gün 1 | C++ Sözdizimi, Bellek ve Pointerlar

**Tarih:** 5 Kasım 2026
**Süre:** En fazla 4 saat (aralar dahil)

Bu sayfa birinci günün ders notudur. Derste anlattığımız her şey burada, hem de sırasıyla. Dersi kaçırdıysanız ya da bir yeri tekrar okumak istiyorsanız buradan devam edebilirsiniz. Daha önce hiç kod yazmadıysanız da endişe etmeyin, bu notu tam olarak sizi düşünerek yazdık.

## Günün Akışı

Aşağıdaki tablo bir öneridir. Sözdizimi bölümü sıfırdan başlayanlara göre genişlediği için süreleri yeniden dengeledik. Eski planla karşılaştırması ve sıkışırsak nereden kısaltacağımız sayfanın sonundaki [Eğitmen notu](#eğitmen-notu) kısmında yazıyor.

| Süre | Bölüm | Klasör | Hedef |
|:---:|---|---|---|
| 5 dk | Açılış | | İki günün yol haritası ve final projesinin haberini vermek |
| 20 dk | Kurulum ve terminal | [00_kurulum](../00_kurulum/README.md) | Herkes ilk programını derleyip çalıştırmış olsun |
| 40 dk | C++ sözdizimi, 1. kısım | [01_syntax](01_syntax) | Program, derleme, değişken, tip, operatör, girdi ve çıktı |
| 10 dk | Ara | | |
| 55 dk | C++ sözdizimi, 2. kısım | [01_syntax](01_syntax) | Koşul, döngü, dizi, fonksiyon, bit işlemleri, hata okuma |
| 10 dk | Ara | | |
| 45 dk | Bellek ve pointerlar | [02_pointers](02_pointers) | Günün kalbi burası |
| 10 dk | Ara | | |
| 35 dk | Uygulama: sıralama | [03_sorting](03_sorting) | Pointer bilgisini algoritma yazarak pekiştirmek |
| 10 dk | Kapanış | [02_odevler](../02_odevler/README.md) | Ödevlerin tanıtımı ve teslim usulü |

## 0. Açılış (5 dk)

Neden C++ öğreniyoruz, onunla başlayalım. Elektronik ve haberleşme mühendisliğinde yazdığımız kodun ekseriya bir donanımla konuşması gerekir. Bir mikrodenetleyicinin register'ına bit yazmak, bir ADC'den örnek okumak, bir sinyali gerçek zamanlı filtrelemek. Daha üst seviye diller bunlar için çoğu zaman yavaş kalır ya da donanıma o kadar yaklaşamaz. C++ ise hem donanıma çok yakın durur hem de büyük projeleri düzenli tutmamızı sağlayan sınıf yapısını sunar.

Bu iki günün hikâyesi şöyle:

1. **Bugün** dili ve belleği tanıyacağız. Bir değişkenin bellekte nerede durduğunu, pointer denen şeyin aslında ne olduğunu göreceğiz.
2. **Hafta boyunca** ödevlerle bu bilgiyi elektronik problemlerinde kullanacağız: register işlemleri, ADC simülasyonu, ring buffer, basit filtreler ve Hamming kodu.
3. **12 Kasım'da** sınıflarla tanışıp bu parçaları bir araya getirerek bir **dijital filtre kütüphanesi** tasarlayacağız.

Yani bu hafta yazacağınız ödevler, final projesinin yapı taşları olacak. Bunu baştan bilmek motivasyon açısından ehemmiyetli.

## 1. Kurulum ve Terminal (20 dk)

Ayrıntılar [00_kurulum](../00_kurulum/README.md) sayfasında. Terminal nedir, klasörler arasında nasıl gezilir, hepsi orada adım adım anlatılıyor. Derste hedefimiz şu: herkes `01_gun1/01_syntax` klasörüne gidip aşağıdaki komutları yazsın ve ekranda merhaba mesajını görsün.

```bash
g++ -std=c++17 -Wall -Wextra 01_merhaba.cpp -o merhaba
./merhaba
```

Kurulumu bitmeyenleri bekletmiyoruz. [godbolt.org](https://godbolt.org) ya da [OnlineGDB](https://www.onlinegdb.com/online_c++_compiler) üzerinden devam etsinler, arada yanlarına gidip yardım ederiz.

## 2. C++ Sözdizimi (95 dk)

Bu bölüm hiç kod yazmamış birinin takip edebileceği şekilde yazıldı. Her alt bölümün sonunda derste birkaç dakikada yapacağımız bir **Sıra sizde** alıştırması var. Çözümleri burada yok, zira asıl öğrenme o birkaç dakikalık uğraşta oluyor. Takılırsanız yanınızdakine ya da bize sorun.

### 2.1 Program nedir, derleme nedir? (5 dk)

**Program**, bilgisayara sırayla yaptırılacak işlerin listesidir. Bir yemek tarifi gibi düşünün. Tarifteki adımlar çok açık yazılmalıdır, zira bilgisayar sizin ne demek istediğinizi tahmin etmez, ne yazdıysanız onu yapar.

Lakin işlemci bizim yazdığımız metni okuyamaz, yalnızca sayılardan oluşan **makine kodunu** anlar. Araya bir çevirmen gerekir. Bu çevirmene **derleyici** (compiler), çeviri işine de **derleme** diyoruz.

```
kaynak kod (01_merhaba.cpp)  ──derleyici (g++)──▶  çalıştırılabilir dosya (merhaba.exe)  ──▶  çalıştır
```

İki dosya arasındaki farkı iyi oturtalım:

| | Kaynak kod | Çalıştırılabilir dosya |
|---|---|---|
| Örnek | `01_merhaba.cpp` | `merhaba.exe` ya da `merhaba` |
| İçinde ne var? | Bizim yazdığımız, okunabilir metin | İşlemcinin anladığı makine kodu |
| Nasıl açılır? | Herhangi bir metin editöründe | Terminalden çalıştırılır |
| Kim üretir? | Biz | Derleyici |

Şu kuralı unutmayın: **kaynak kodu değiştirdiğinizde yeniden derlemezseniz, çalıştırdığınız program hâlâ eski hâlidir.** Yeni başlayanların en sık yaşadığı "kodu düzelttim ama hiçbir şey değişmedi" durumunun sebebi budur.

Derleyici kodunuzu baştan sona okur, hata varsa daha program çalışmadan size söyler. Bu sinir bozucu gelebilir, lakin hataları erken yakalamak donanımda çalışan bir kod için büyük nimettir.

> **Sıra sizde (2 dk):** `01_merhaba.cpp` dosyasındaki mesajı değiştirip kaydedin. Evvela derlemeden programı çalıştırın, sonra derleyip tekrar çalıştırın. İki çalıştırma arasında ne fark var? Klasörde hangi dosyaların olduğuna da bir bakın.

### 2.2 Bir programın iskeleti (6 dk)

Demo dosyası: [`01_merhaba.cpp`](01_syntax/01_merhaba.cpp)

```cpp
#include <iostream>

int main()
{
    std::cout << "Merhaba YONGA!" << std::endl;
    return 0;
}
```

Satır satır bakalım:

* `#include <iostream>`: Ekrana yazı yazmamızı sağlayan kütüphaneyi programımıza dahil ediyoruz. Kütüphane, başkalarının yazıp hazır bıraktığı kod demektir.
* `int main()`: Her C++ programı buradan başlar. Başka bir yerden değil, mutlaka `main` fonksiyonundan.
* `std::cout << ...`: Ekrana yazdırır. `std::` öneki, bu şeyin standart kütüphaneye ait olduğunu söyler.
* `return 0`: İşletim sistemine "her şey yolunda bitti" demenin yolu.

**İfade (statement) ve noktalı virgül:** İfade, programın tek bir komutudur. C++'ta her ifade `;` ile biter. Türkçedeki cümle sonu noktası gibi düşünün. Derleyici için satır sonunun bir anlamı yoktur, ifadenin bittiğini ancak `;` görünce anlar. Unutursanız derlenmez, merak etmeyin başınıza çok gelecek.

**Süslü parantez blokları:** `{` ile `}` arasındaki ifadeler bir **blok** oluşturur ve birlikte hareket eder. Açtığınız her `{` işaretini bir `}` ile kapatmanız gerekir. `#include` satırlarının ve bloğu açan `int main()` satırının sonuna `;` konmaz.

**Girinti alışkanlığı:** Bloğun içindeki satırları dört boşluk içeriden başlatıyoruz. Derleyici girintiye hiç bakmaz, program girintisiz de çalışır. Yine de girinti yapın, zira kodu okuyacak olan bir insandır (ve o insan ekseriya bir hafta sonraki sizsinizdir). Aşağıdaki iki kod derleyici için aynıdır, hangisini okumak isteyeceğiniz malum:

```cpp
int main(){int a=5;if(a>3){a=0;}return a;}
```

```cpp
int main()
{
    int a = 5;
    if (a > 3) {
        a = 0;
    }
    return a;
}
```

**Yorum satırları:** Derleyicinin görmezden geldiği, insanlar için yazılan notlardır.

```cpp
// İki eğik çizgiden sonrası, satırın sonuna kadar yorumdur.

/* Bu şekilde
   birden fazla satırı
   yorum yapabilirsiniz. */

int adet = 3;   // Kodun yanına da yazılabilir.
```

Yorumda kodun ne yaptığını değil, **neden** öyle yaptığını anlatmaya çalışın. Bir satırı silmeden geçici olarak devre dışı bırakmak için de yorum yapabilirsiniz, bugün sıkça göreceksiniz.

> **Sıra sizde (2 dk):** `01_merhaba.cpp` dosyasına adınızı ve bölümünüzü ayrı satırlarda yazdıran iki ifade ekleyin. Sonra içlerinden birini yorum satırına çevirip tekrar derleyin ve çalıştırın.

### 2.3 Değişkenler (7 dk)

Demo dosyası: [`02_degiskenler.cpp`](01_syntax/02_degiskenler.cpp)

**Değişken**, üzerinde isim yazan bir kutudur. İçine bir değer koyarız, sonra ismiyle çağırıp kullanırız, istersek içindekini değiştiririz.

```cpp
int sayac;        // tanımlama: int tipinde, adı sayac olan bir kutu aç
sayac = 3;        // atama: kutunun içine 3 koy
int adet = 10;    // tanımlama ve ilk değer verme aynı satırda
```

Üçüncü satırdaki yöntemi alışkanlık edinin. İlk değer verilmemiş bir değişkenin içinde ne olduğu belirsizdir, önceden orada kalmış herhangi bir şey çıkabilir.

**İsimlendirme kuralları:**

* Harf, rakam ve alt çizgi kullanılabilir. Boşluk kullanılamaz.
* Rakamla başlayamaz: `2kanal` geçersiz, `kanal2` geçerli.
* `int`, `return`, `for` gibi dilin ayrılmış kelimeleri isim olamaz.
* Büyük küçük harf ayrımı vardır: `adet` ile `Adet` farklı değişkenlerdir.
* Türkçe karakter kullanmayın, `sayaç` yerine `sayac` yazın.
* Anlamlı isim seçin. `x` yerine `ornek_sayisi` yazmak bir hafta sonra size çok şey kazandırır.

**Atama sağdan sola okunur.** `adet = adet + 1` matematikte saçma görünür, lakin burada bir denklem değil, bir emirdir: "adet'in şu anki değerine 1 ekle, sonucu tekrar adet'e yaz."

**`=` ile `==` farkı:** Bu ikisini karıştırmak klasik bir hatadır.

| Yazım | Anlamı | Sonucu |
|---|---|---|
| `a = b` | Atama. `b`'nin değerini `a`'nın içine yaz. | `a` değişir |
| `a == b` | Karşılaştırma. İkisi eşit mi diye sor. | `true` ya da `false`, hiçbir şey değişmez |

```cpp
if (a = 5)  { ... }   // YANLIŞ: a'ya 5 yazar ve koşul hep doğru olur
if (a == 5) { ... }   // DOĞRU: a, 5'e eşit mi diye sorar
```

`-Wall` bayrağını bu yüzden seviyoruz, ilk satır için derleyici sizi `suggest parentheses around assignment used as truth value` diye uyarır.

> **Sıra sizde (3 dk):** `a = 3` ve `b = 8` olan iki değişken tanımlayın. Üçüncü bir değişken kullanarak değerlerini birbiriyle değiştirin ve ekrana yazdırın. Bu fikre pointer bölümünde geri döneceğiz.

### 2.4 Tipler (9 dk)

Demo dosyası: [`03_tipler.cpp`](01_syntax/03_tipler.cpp)

C++'ta her değişkenin bir **tipi** vardır. Tip, kutunun içine ne konabileceğini ve bellekte kaç bayt yer kaplayacağını belirler. Bir kere tanımladıktan sonra değişkenin tipi değişmez.

| Tip | Tipik boyut | Ne için? | Örnek |
|---|:---:|---|---|
| `bool` | 1 bayt | Doğru ya da yanlış | `true` |
| `char` | 1 bayt | Tek karakter, tek tırnakla | `'Y'` |
| `int` | 4 bayt | Tam sayı | `42` |
| `float` | 4 bayt | Ondalıklı sayı (az hassas) | `3.3f` |
| `double` | 8 bayt | Ondalıklı sayı (hassas) | `3.3` |

Ondalık ayracı olarak virgül değil, nokta kullanıyoruz: `3,3` değil `3.3`.

**Metin için `std::string`:** Birden fazla karakteri `#include <string>` ile gelen `std::string` tutar ve çift tırnakla yazılır. `'Y'` bir `char`, `"Y"` ise metindir.

```cpp
std::string kulup = "YONGA";
std::string selam = "Merhaba " + kulup;   // + metinleri birleştirir
selam.size();                             // uzunluğu: 13
selam[0];                                 // ilk karakteri: 'M'
```

**Sabit genişlikli tipler:** Buradaki "tipik" kelimesi önemli. `int` tipinin boyutu platforma göre değişebilir. Bir bilgisayarda 4 bayt olan `int`, eski bir 8 bitlik mikrodenetleyicide 2 bayt olabilir. Gömülü dünyada bu belirsizliği kimse istemez, bu yüzden boyutu kesin olan tipler kullanırız:

| Tip | Boyut | Aralık | Elektronikte nerede görürüz? |
|---|:---:|---|---|
| `uint8_t` | 8 bit | 0 ile 255 arası | 8 bitlik ADC, UART ile gelen bir bayt |
| `int16_t` | 16 bit | `-32768` ile `32767` arası | Ses örnekleri (CD kalitesi 16 bittir) |
| `uint16_t` | 16 bit | 0 ile 65535 arası | 12 bitlik ADC çıktısı buraya sığar |
| `uint32_t` | 32 bit | 0 ile yaklaşık 4,29 milyar arası | ARM mikrodenetleyicilerde register |

Bunlar için `#include <cstdint>` gerekir. Baştaki `u` harfi işaretsiz (unsigned) demektir, yani negatif değer tutmaz. Ödevlerde `size_t` diye bir tip de göreceksiniz. O da işaretsiz bir tam sayıdır ve boyut ile indeks tutmak için kullanılır.

**Taşma (overflow):** `uint8_t` tipindeki 250'ye 10 eklersek 260 olmaz, **4** olur. Zira 8 bit en fazla 255'i tutabilir ve sayaç başa sarar. Bunu demo kodunda canlı gösteriyoruz. Bir sayacın taşması, gerçek sistemlerde ciddi hatalara sebep olabilir.

**Tip dönüşümleri:** Bir tipi diğerine çevirmenin en açık yolu `static_cast` kullanmaktır.

```cpp
double olcum = 3.9;
int kesilmis = static_cast<int>(olcum);   // 3 olur, yuvarlanmaz, virgülden sonrası atılır
static_cast<int>('A');                    // 65, yani A harfinin sayı karşılığı
std::stoi("42");                          // metinden int: 42
std::to_string(1024);                     // int'ten metin: "1024"
```

**Tam sayı bölmesi tuzağı:** `7 / 2` işleminin sonucu `3` çıkar, `3.5` değil. İki tam sayıyı bölerseniz sonuç da tam sayı olur. Ondalıklı sonuç istiyorsanız birini `double` tipine çevirin: `static_cast<double>(7) / 2`.

**`const` ve `auto`:**

* `const double PI = 3.14159` yazarsak PI bir daha değiştirilemez. Değişmemesi gereken her şeyi `const` yapın, derleyici sizi kendi hatalarınızdan korur.
* `auto x = 3.0` yazarsak derleyici tipi kendisi çıkarır. Rahat, lakin tipin ne olduğunu bilmeden kullanmayın.

> **Sıra sizde (3 dk):** Çalıştırmadan önce kâğıda tahmininizi yazın, sonra deneyin: `uint8_t` tipinde 200 değerindeki bir değişkene 100 eklenirse ne çıkar? `5 / 2` ve `5.0 / 2` kaç eder? `static_cast<int>('a')` kaç eder?

### 2.5 Operatörler (7 dk)

Demo dosyası: [`04_operatorler.cpp`](01_syntax/04_operatorler.cpp)

**Aritmetik:**

| Operatör | Anlamı | `a = 17`, `b = 5` için | Sonuç |
|:---:|---|---|:---:|
| `+` | Toplama | `a + b` | 22 |
| `-` | Çıkarma | `a - b` | 12 |
| `*` | Çarpma | `a * b` | 85 |
| `/` | Bölme | `a / b` | 3 |
| `%` | Bölümden kalan (mod) | `a % b` | 2 |

Mod operatörünü küçümsemeyin. Bir sayının tek mi çift mi olduğunu `x % 2` ile anlarız, ring buffer ödevinde de bütün sihir bu operatörde.

**Karşılaştırma:** Sonuç her zaman `bool` olur. `==` eşit mi, `!=` farklı mı, `<` küçük mü, `>` büyük mü, `<=` küçük ya da eşit mi, `>=` büyük ya da eşit mi diye sorar. Ekrana basıldığında `true` 1, `false` 0 olarak görünür.

**Mantıksal:** Birden fazla koşulu birleştirir.

| Operatör | Adı | Ne zaman doğru? |
|:---:|---|---|
| `&&` | VE | İki taraf da doğruysa |
| `\|\|` | VEYA | En az bir taraf doğruysa |
| `!` | DEĞİL | Koşul yanlışsa |

```cpp
bool aralikta = (volt >= 3.0) && (volt <= 3.6);
```

Matematikteki gibi `3.0 <= volt <= 3.6` yazamazsınız, derlenir ama beklediğiniz şeyi yapmaz. Yukarıdaki gibi iki parçaya ayırın.

**Artırma ve azaltma:** `i++` ve `++i` ikisi de `i`'yi 1 artırır, `i--` ve `--i` ise 1 azaltır. Tek başına bir satırda yazıldıklarında aralarında fark yoktur. Fark, değeri aynı anda kullanırken ortaya çıkar:

```cpp
int i = 5;
int x = i++;   // önce kullan, sonra artır: x = 5, i = 6

int j = 5;
int y = ++j;   // önce artır, sonra kullan: y = 6, j = 6
```

**Bileşik atama:** `toplam += 5` yazmak `toplam = toplam + 5` yazmanın kısa yoludur. `-=`, `*=`, `/=` ve `%=` de aynı mantıkla çalışır.

**Öncelik sırası:** Yukarıdan aşağıya doğru önce yapılanlar:

| Sıra | Operatörler | Açıklama |
|:---:|---|---|
| 1 | `( )` | Önce parantez |
| 2 | `!`, `++`, `--` | Tek değerle çalışanlar |
| 3 | `*`, `/`, `%` | Çarpma grubu |
| 4 | `+`, `-` | Toplama grubu |
| 5 | `<`, `<=`, `>`, `>=` | Büyüklük karşılaştırması |
| 6 | `==`, `!=` | Eşitlik karşılaştırması |
| 7 | `&&` | Mantıksal VE |
| 8 | `\|\|` | Mantıksal VEYA |
| 9 | `=`, `+=`, `-=` ve benzerleri | Atama, en son |

Bunu ezberlemek şart değil. Şüpheye düştüğünüzde parantez koyun, hem güvenlidir hem de okuması kolaydır.

> **Sıra sizde (3 dk):** Çalıştırmadan önce tahmin edin, sonra deneyin: `2 + 3 * 4`, `17 % 5`, `10 - 4 - 3`. Ardından `int i = 3` için `int a = i++` ve `int b = ++i` satırlarından sonra `a`, `b` ve `i` değerlerinin ne olacağını kâğıda yazıp kontrol edin.

### 2.6 Girdi ve çıktı (6 dk)

Demo dosyası: [`05_girdi_cikti.cpp`](01_syntax/05_girdi_cikti.cpp)

**Ekrana yazmak:** `std::cout` ekranı temsil eder. `<<` ile parçaları art arda ona gönderirsiniz.

```cpp
std::cout << "Kanal " << kanal << " : " << volt << " V\n";
```

**Klavyeden okumak:** `std::cin` klavyeyi temsil eder, `>>` ile okunan değer değişkene aktarılır. Değişkenin tipi neyse o tipte okunur.

```cpp
int yas = 0;
double boy = 0.0;
std::cin >> yas >> boy;    // 20 1.75 yazıp Enter'a basın
```

`>>` boşlukta durur. Boşluk içeren bir satırı okumak için `std::getline(std::cin, tam_ad)` kullanın. Demo dosyasında bir de `std::cin.ignore` satırı göreceksiniz. O satır, önceki `>>` işleminden kalan Enter karakterini atlamak için orada.

**`std::endl` ile `"\n"` farkı:** İkisi de alt satıra geçer. `std::endl` buna ek olarak tamponu boşaltır (flush), yani bekleyen yazıyı hemen ekrana gönderir. Tampon, yazıların toplu gönderilmek üzere biriktiği ara depodur. Bu ek iş döngü içinde programı yavaşlatabildiği için ekseriya `"\n"` yeterlidir, kâfi gelmediği durumlarda `std::endl` kullanırız.

Ekrana bastığımız metinlerde bilhassa Türkçe karakter kullanmıyoruz, zira Windows konsolunda bozuk çıkabiliyor. Bu yüzden kodda `"Hos geldin"` gibi yazılar göreceksiniz.

> **Sınıfa soru:** `std::cin >> isim` yazıp "Ali Veli" girersek ne olur? (Cevap: sadece "Ali" okunur, zira `>>` boşlukta durur.)

> **Sıra sizde (3 dk):** Kullanıcıdan gerilim ve akım değerlerini okuyup harcanan gücü ($P = V \cdot I$) birimiyle birlikte ekrana yazan bir program yazın.

### Ara (10 dk)

### 2.7 Kontrol akışı: koşullar (9 dk)

Demo dosyası: [`06_kosullar.cpp`](01_syntax/06_kosullar.cpp)

Şimdiye kadar yazdığımız kodlar yukarıdan aşağıya düz bir çizgide çalıştı. Koşullar, programın duruma göre farklı yollara sapmasını sağlar.

**`if`, `else if`, `else`:**

```cpp
if (pil > 4.0) {
    std::cout << "Pil dolu\n";
} else if (pil > 3.3) {
    std::cout << "Pil normal\n";
} else {
    std::cout << "Pil zayif, sarja tak\n";
}
```

Akış şeması şöyle:

```
              pil > 4.0 ?
             /           \
          evet           hayır
           |               |
     "Pil dolu"       pil > 3.3 ?
                     /           \
                  evet           hayır
                   |               |
             "Pil normal"     "Pil zayif"
```

`pil = 3.45` için adım adım:

1. `pil > 4.0` soruluyor. Yanlış, ilk blok atlanıyor.
2. `pil > 3.3` soruluyor. Doğru, "Pil normal" basılıyor.
3. Bir dal çalıştığı için `else` hiç değerlendirilmeden yapının dışına çıkılıyor.

Yani yukarıdan aşağıya bakılır, ilk doğru olan dal çalışır, gerisi atlanır. Bu yüzden koşulların sırası önemlidir.

**`switch`:** Bir tam sayıyı ya da karakteri sabit değerlerle karşılaştırmak için kullanılır.

```cpp
switch (durum) {
    case 0:  std::cout << "BEKLE\n";   break;
    case 1:  std::cout << "OLC\n";     break;
    case 2:  std::cout << "GONDER\n";  break;
    default: std::cout << "HATA\n";    break;
}
```

Adım adım:

1. Parantez içindeki değer hesaplanır.
2. Bu değere eşit olan `case` satırına atlanır. Hiçbiri eşit değilse `default` satırına gidilir.
3. `break` görülene kadar aşağı doğru çalışılır.
4. `break` ile `switch` bloğunun dışına çıkılır.

Donanım tarafında **durum makinesi (FSM)** yazarken en çok bunu kullanacaksınız. Her `case` sonunda `break` yazmayı unutursanız program bir alttaki duruma "düşer". Bazen bu bilerek yapılır, ekseriya ise hatadır. `-Wextra` bayrağı bu durumda sizi uyarır. `switch` metinle çalışmaz, `std::string` için `if` kullanın.

**Ternary operatörü:** İki seçenekli küçük bir `if`, üstelik bir değer üretir.

```cpp
double kirpilmis = (giris > 3.3) ? 3.3 : giris;
```

Adım adım:

1. `?` işaretinin solundaki koşula bakılır.
2. Doğruysa `?` ile `:` arasındaki değer seçilir.
3. Yanlışsa `:` işaretinin sağındaki değer seçilir.

`giris = 5.2` için koşul doğru olduğundan `kirpilmis` 3.3 olur. Kısa işlerde şıktır, iç içe yazmaya kalkarsanız okunmaz hâle gelir.

> **Sıra sizde (3 dk):** Klavyeden bir sıcaklık okuyun. 0 altındaysa "donuyor", 0 ile 30 arasındaysa "normal", 30 üstündeyse "sicak" yazdırın. Ardından aynı programa, sayının tek mi çift mi olduğunu ternary ile yazan bir satır ekleyin.

### 2.8 Kontrol akışı: döngüler (13 dk)

Demo dosyası: [`07_donguler.cpp`](01_syntax/07_donguler.cpp)

Döngü, bir bloğu tekrar tekrar çalıştırır. Üç çeşidi var:

| Döngü | Ne zaman kullanılır? | Gövde en az kaç kere çalışır? |
|---|---|:---:|
| `for` | Kaç tur döneceği belliyse | 0 |
| `while` | Bir koşul sağlandığı sürece, tur sayısı belli değilse | 0 |
| `do while` | Önce bir kere yapıp sonra kontrol etmek gerekiyorsa | 1 |

**`for`:**

```cpp
int toplam = 0;
for (int i = 1; i <= 4; i++) {
    toplam += i;
}
```

Parantezin içinde `;` ile ayrılmış üç parça vardır: başlangıç, koşul ve artırım. Çalışma sırası şöyledir:

```
başlangıç ──▶ koşul doğru mu? ──hayır──▶ döngüden çık
                 ▲     │
                 │    evet
                 │     ▼
             artırım ◀── gövde
```

Bir döngünün ne yaptığını anlamanın en iyi yolu **iz tablosu** (trace table) çıkarmaktır. Kâğıda her değişken için bir sütun çizip programı elinizle çalıştırırsınız. Yukarıdaki kodun izi:

| Adım | `i` | `i <= 4` | `toplam += i` sonrası `toplam` | Ne oluyor? |
|:---:|:---:|:---:|:---:|---|
| Başlangıç | 1 | | 0 | `i` ve `toplam` hazır |
| 1. tur | 1 | doğru | 1 | Sonra `i` 2 olur |
| 2. tur | 2 | doğru | 3 | Sonra `i` 3 olur |
| 3. tur | 3 | doğru | 6 | Sonra `i` 4 olur |
| 4. tur | 4 | doğru | 10 | Sonra `i` 5 olur |
| Çıkış | 5 | yanlış | 10 | Gövde çalışmaz, döngü biter |

**`while`:** Koşul doğru olduğu sürece döner. Koşulu bir gün yanlış yapacak bir şey yazmayı unutursanız program sonsuza kadar döner. Terminalde `Ctrl+C` ile durdurabilirsiniz.

```cpp
int deger = 1, adim = 0;
while (deger < 20) {
    deger *= 2;
    adim++;
}
```

| Tur | `deger < 20` | Tur sonunda `deger` | Tur sonunda `adim` |
|:---:|:---:|:---:|:---:|
| 1 | 1 < 20 doğru | 2 | 1 |
| 2 | 2 < 20 doğru | 4 | 2 |
| 3 | 4 < 20 doğru | 8 | 3 |
| 4 | 8 < 20 doğru | 16 | 4 |
| 5 | 16 < 20 doğru | 32 | 5 |
| Çıkış | 32 < 20 yanlış | 32 | 5 |

**`do while`:** Koşula sonda baktığı için gövde en az bir kere çalışır. Kullanıcıdan geçerli bir değer girene kadar tekrar tekrar sormak gibi işlerde kullanılır. Sondaki `;` işaretine dikkat edin.

```cpp
int n = 100;
do {
    n++;
} while (n < 10);   // koşul yanlış, yine de bir tur döndü: n = 101
```

**`break` ve `continue`:**

* `break`: İçinde bulunduğu döngüden hemen çıkar.
* `continue`: Bu turun kalanını atlar, sıradaki tura geçer. `for` döngüsünde artırım yine çalışır.

**İç içe döngü:** Dıştaki döngünün her turu için içteki döngü baştan sona döner.

```cpp
for (int satir = 1; satir <= 2; satir++) {
    for (int sutun = 1; sutun <= 3; sutun++) {
        std::cout << satir * sutun << " ";
    }
    std::cout << "\n";
}
```

| `satir` | `sutun` | Ekrana basılan | Not |
|:---:|:---:|:---:|---|
| 1 | 1 | 1 | |
| 1 | 2 | 2 | |
| 1 | 3 | 3 | Sonra `sutun` 4 olur, iç döngü biter, alt satıra geçilir |
| 2 | 1 | 2 | `sutun` yeniden 1'den başladı |
| 2 | 2 | 4 | |
| 2 | 3 | 6 | İç döngü biter, ardından `satir` 3 olur ve dış döngü de biter |

Toplam tur sayısı $2 \times 3 = 6$ oldu. Sıralama bölümünde bu yapıyı bolca kullanacağız.

> **Sıra sizde (4 dk):** Evvela kâğıt üzerinde `for (int i = 10; i > 0; i -= 3)` döngüsünün iz tablosunu çıkarın. Kaç tur döner, çıkışta `i` kaç olur? Sonra bilgisayarda deneyip kontrol edin. Vakit kalırsa 1'den 5'e kadar çarpım tablosunu iç içe döngüyle yazdırın.

### 2.9 Diziler (6 dk)

Demo dosyası: [`08_diziler.cpp`](01_syntax/08_diziler.cpp)

Dizi, aynı tipten değerlerin bellekte yan yana durduğu yapıdır. 100 ölçüm için 100 ayrı değişken tanımlamak yerine tek bir dizi tanımlarız.

```cpp
int olcumler[5] = {12, 15, 11, 19, 14};
```

```
 indeks:      0    1    2    3    4
           ┌────┬────┬────┬────┬────┐
 değer:    │ 12 │ 15 │ 11 │ 19 │ 14 │
           └────┴────┴────┴────┴────┘
```

* **İndeksleme:** `olcumler[1]` ikinci elemanı verir, `olcumler[2] = 21` üçüncü elemanın üzerine yazar.
* **Sıfırdan başlama:** İlk elemanın indeksi 0'dır, 5 elemanlı dizinin son elemanı ise `olcumler[4]`. İndeksi "baştan kaç eleman uzakta" diye düşünün, ilk eleman baştan 0 uzaktadır.
* **Eksik elemanlar:** `int sifirli[4] = {7}` yazarsanız geri kalan elemanlar 0 olur.
* **Sabit boyut:** Dizinin boyutu tanımlandıktan sonra değişmez ve derleme anında bilinmelidir. `int dizi[n]` gibi değişken bir boyut, ödevlerdeki `-Wpedantic` bayrağıyla derlenmez.

Diziyi gezmenin klasik yolu `for` döngüsüdür. Koşulun `i < 5` olduğuna, `i <= 5` olmadığına dikkat edin:

```cpp
for (int i = 0; i < 5; i++) {
    std::cout << olcumler[i] << "\n";
}

for (int x : olcumler) {          // her elemanı sırayla x içine kopyalar
    std::cout << x << "\n";
}
```

**Sınır dışına çıkma tehlikesi:** `olcumler[5]` yazarsanız derleyici ekseriya ses çıkarmaz, program da her zaman çökmez. Dizinin bittiği yerden sonraki bellekte ne varsa onu okursunuz, yazarsanız da başka bir verinin üzerine yazarsınız. Program bugün çalışıp yarın saçmalayabilir, bu yüzden en sinsi hatalardandır. C++ sizin yerinize indeks kontrolü yapmaz, o sorumluluk bizde.

> **Sıra sizde (2 dk):** 5 elemanlı bir `double` dizisine istediğiniz ölçümleri yazın ve döngüyle ortalamasını hesaplayıp ekrana basın.

### 2.10 Fonksiyonlar (13 dk)

Demo dosyası: [`09_fonksiyonlar.cpp`](01_syntax/09_fonksiyonlar.cpp)

Fonksiyon, bir işe isim verip tekrar tekrar kullanmamızı sağlar. Aynı kodu üç yere kopyalamak yerine bir kere yazıp üç kere çağırırız.

**Tanım ve çağırma:**

```cpp
double guc_db(double guc_orani)         // tanım
{
    return 10.0 * std::log10(guc_orani);
}

double sonuc = guc_db(100.0);           // çağırma: sonuc 20 olur
```

Tanımın parçaları:

| Parça | Örnekte | Anlamı |
|---|---|---|
| Dönüş tipi | `double` | Fonksiyonun geri vereceği değerin tipi |
| İsim | `guc_db` | Çağırırken kullanacağımız ad |
| Parametre | `double guc_orani` | Fonksiyonun dışarıdan aldığı değer |
| Gövde | `{ ... }` | Çağrılınca çalışan kod |
| Dönüş değeri | `return ...` | Çağıran yere geri giden sonuç, fonksiyon burada biter |

`std::log10`, `std::sqrt`, `std::sin` ve `std::floor` gibi hazır matematik fonksiyonları `#include <cmath>` ile gelir.

**`void`:** Fonksiyon geri bir şey döndürmeyecekse dönüş tipi `void` olur.

```cpp
void baslik_yaz(int no)
{
    std::cout << "=== Bolum " << no << " ===\n";
}
```

**Kapsam (scope) ve yerel değişken ömrü:** Bir değişken yalnızca tanımlandığı süslü parantez bloğunun içinde yaşar. Bir fonksiyonun içinde tanımlanan **yerel değişken**, fonksiyon her çağrıldığında baştan oluşur ve fonksiyon bitince yok olur. Başka bir fonksiyon onu göremez.

```cpp
void yerel_sayac()
{
    int sayac = 0;
    sayac++;
    std::cout << sayac << "\n";
}

yerel_sayac();   // 1 basar
yerel_sayac();   // yine 1 basar, zira sayac sıfırdan doğdu
```

Farklı fonksiyonlarda aynı isimde değişken tanımlamak bu yüzden sorun çıkarmaz, onlar birbirinden habersiz ayrı kutulardır.

**Fonksiyon bildirimi (prototype):** Derleyici dosyayı yukarıdan aşağıya okur ve bir fonksiyonu çağırmadan önce onu tanımak ister. Tanımı aşağıya yazmak istiyorsanız yukarıya bir bildirim koyarsınız:

```cpp
int kare(int x);        // bildirim: böyle bir fonksiyon var, sonu ; ile biter

int main()
{
    std::cout << kare(6);
    return 0;
}

int kare(int x)         // tanım: asıl kod burada
{
    return x * x;
}
```

Ödevlerdeki `include/*.hpp` dosyalarının içinde de tam olarak bu bildirimler var. Siz de tanımları `src/*.cpp` dosyasına yazacaksınız.

Şimdi pointer bölümüne köprü kuracak kritik bir konu: **değerle aktarım ve referansla aktarım.**

```cpp
void iki_kat_deger(int x)     { x = x * 2; }   // x bir KOPYA
void iki_kat_referans(int& x) { x = x * 2; }   // x orijinalin ta kendisi
```

`a = 5` iken birincisini çağırırsak `a` hâlâ 5 kalır. Fonksiyon, `a` değişkeninin bir fotokopisini almış ve fotokopiyi karalamıştır. İkincisinde ise `int&` yazdığımız için fonksiyon orijinal değişkene erişir ve `a` 10 olur.

Referansın bir faydası daha var: bir fonksiyondan birden fazla sonuç almak. `min_max_bul` örneğinde en küçüğü ve en büyüğü referans parametreler üzerinden geri veriyoruz.

> **Sınıfa soru:** Peki fonksiyon, orijinal değişkenin nerede olduğunu nasıl biliyor? İşte bu sorunun cevabı bir sonraki bölümlerde: **adresler.**

> **Sıra sizde (3 dk):** Gerilim bölücü çıkışını ($V_{out} = V_{in} \cdot \frac{R_2}{R_1 + R_2}$) hesaplayan `double volt_bolucu(double vin, double r1, double r2)` fonksiyonunu yazın. Bildirimi `main` fonksiyonunun üstünde, tanımı altında olsun. İki farklı direnç çiftiyle çağırın.

### 2.11 Bit işlemleri (9 dk)

Demo dosyası: [`10_bitwise.cpp`](01_syntax/10_bitwise.cpp)

İşte elektronikçiyi bilgisayarcıdan ayıran yer burası. Bir mikrodenetleyicide LED yakmak, bir sensörün ayarını değiştirmek, bir haberleşme paketini çözmek hep bit işlemleriyle yapılır.

| Operatör | Adı | Örnek (`a = 1100`, `b = 1010`) | Sonuç |
|:---:|---|---|:---:|
| `&` | VE | `a & b` | `1000` |
| `\|` | VEYA | `a \| b` | `1110` |
| `^` | XOR | `a ^ b` | `0110` |
| `~` | DEĞİL | `~a` | `0011` |
| `<<` | Sola kaydır | `a << 1` | `11000` |
| `>>` | Sağa kaydır | `a >> 2` | `0011` |

Bunları ezberlemek yerine dört temel kalıbı öğrenin. Bütün gömülü kodlar bu dört satırın etrafında döner:

```cpp
reg |=  (1u << n);     // n. biti 1 yap      (SET)
reg &= ~(1u << n);     // n. biti 0 yap      (CLEAR)
reg ^=  (1u << n);     // n. biti tersle     (TOGGLE)
(reg >> n) & 1u;       // n. biti oku        (READ)
```

Nasıl çalıştıklarını tahtada çizerek anlatalım. `1u << 3` ifadesi `00001000` demektir, yani sadece 3. biti açık bir **maske**. Bu maskeyi VEYA ile uygularsak diğer bitlere dokunmadan sadece 3. biti 1 yapmış oluruz. VE ile uygularken maskenin tersini (`~`) alırız ki sadece 3. bit 0'a çekilsin.

> **Sık yapılan hata:** `&` ile `&&` farklı şeylerdir. `&` bit bit VE yapar, `&&` ise mantıksal VE yapar. `5 & 2` sonucu 0'dır, `5 && 2` sonucu ise `true`.

> **Not:** Sayının sonundaki `u`, onun işaretsiz (unsigned) olduğunu söyler. İşaretli sayılarla bit kaydırmak bazı durumlarda tanımsız davranışa yol açar, bu yüzden register işlemlerinde hep `1u` yazmayı alışkanlık edinin.

> **Sıra sizde (3 dk):** `uint8_t reg = 0` ile başlayın. Evvela kâğıt üzerinde şu sırayla sonucu bulun: 1. biti SET, 4. biti SET, 1. biti TOGGLE, 0. biti SET. Sonra kodunu yazıp `std::bitset<8>(reg)` ile kontrol edin.

### 2.12 Derleyici hata mesajlarını okumak (5 dk)

Demo dosyası: [`11_derleyici_hatalari.cpp`](01_syntax/11_derleyici_hatalari.cpp)

Hata mesajı bir azar değil, bir ipucudur. Korkmadan okumayı öğrenirseniz ciddi vakit kazanırsınız. Mesajın yapısı hep aynıdır:

```
h1.cpp:5:5: error: expected ',' or ';' before 'std'
    5 |     std::cout << x << "\n";
      |     ^~~
```

Soldan sağa okuyun: dosya adı, satır, sütun, türü ve açıklama. Altında da hatalı satır ve tam yeri gösteren bir ok çıkar.

* **`error`** derlemeyi durdurur, program oluşmaz.
* **`warning`** program oluşur, lakin derleyici bir terslik sezmiştir. Ciddiye alın, ödevlerde `-Werror` bayrağı uyarıları da hataya çevirecek.
* **Her zaman en üstteki hatadan başlayın.** Tek bir eksik işaret onlarca hata mesajı doğurabilir. İlkini düzeltip yeniden derleyin, ekseriya gerisi de kaybolur.

En sık karşılaşacağınız beş hata (mesajların dili bilgisayarınızın ayarlarına göre değişebilir):

| No | Sebep | Tipik mesaj | Ne yapmalı? |
|:---:|---|---|---|
| 1 | Noktalı virgül eksik | `expected ',' or ';' before 'std'` | Mesajın gösterdiği satırın **bir üstüne** bakın, eksik olan orada |
| 2 | Tanımsız değişken | `'sayc' was not declared in this scope; did you mean 'sayac'?` | Yazım hatası mı, tanımlamayı unuttunuz mu, kapsamın dışında mı kaldınız, kontrol edin |
| 3 | Tip uyuşmazlığı | `invalid conversion from 'const char*' to 'int'` | Değerin tipi ile değişkenin tipi uyuşmuyor, birini düzeltin |
| 4 | Eksik `#include` | `'cout' is not a member of 'std'` | Gereken başlığı ekleyin. Derleyici ekseriya hangisi olduğunu bir alttaki `note` satırında söyler |
| 5 | Kapanmamış süslü parantez | `expected '}' at end of input` | Hata dosyanın sonunda görünür, ama sebep yukarıdadır. Girintiyi takip ederek kapanmayan bloğu bulun |

Bir de sık göreceğiniz iki yakın akraba: fonksiyonu eksik parametreyle çağırırsanız `too few arguments to function 'int kare(int)'`, tanımlayıp kullanmadığınız bir değişken için de `unused variable 'x'` uyarısı alırsınız.

> **Sıra sizde (2 dk):** `11_derleyici_hatalari.cpp` dosyasındaki yorumları takip ederek hataları sırayla üretin. Her birinde önce mesajı okuyun, yukarıdaki tabloyla eşleştirin, sonra değişikliği geri alın.

## Ara (10 dk)

## 3. Bellek ve Pointerlar (45 dk)

Pointer, C++ öğrenen herkesin bir süre takıldığı konudur. Biz de takıldık, sizin de takılmanız gayet normal. Fakat bir kere oturduğunda, bilgisayarın aslında nasıl çalıştığına dair bakışınız kökten değişir.

### 3.1 Bellek bir apartman gibidir

RAM'i uzun bir apartman koridoru gibi düşünün. Her dairenin bir **kapı numarası** (adres) ve içinde oturan bir **değer** var.

```
 adres:   0x1000   0x1004   0x1008   0x100C
        ┌────────┬────────┬────────┬────────┐
 değer: │   42   │  3.3   │  'Y'   │  ...   │
        └────────┴────────┴────────┴────────┘
           sayi     volt     harf
```

`int sayi = 42` yazdığımızda derleyici bizim için boş bir daire bulur, içine 42 koyar ve o daireye "sayi" adını verir. Biz isimle konuşuruz, işlemci ise hep adresle.

### 3.2 `&` ve `*` operatörleri

Demo dosyası: [`01_adresler.cpp`](02_pointers/01_adresler.cpp)

| İfade | Okunuşu | Anlamı |
|---|---|---|
| `&sayi` | "sayi'nin adresi" | Değişkenin bellekteki kapı numarası |
| `int* p` | "p bir int pointer'ı" | İçinde bir adres tutan değişken |
| `p = &sayi` | "p, sayi'yi göstersin" | p'nin içine sayi'nin adresini koy |
| `*p` | "p'nin gösterdiği yer" | O adresteki değere git (dereference) |

```cpp
int  sayi = 42;
int* p    = &sayi;

std::cout << p;    // 0x7ffd...   (bir adres)
std::cout << *p;   // 42          (o adresteki değer)

*p = 100;          // sayi artık 100!
```

Demo kodunu çalıştırınca gerçek adresleri göreceksiniz. Her çalıştırmada farklı çıkabilir, bu normaldir (işletim sistemi güvenlik için adresleri karıştırır).

Kafa karıştıran nokta şu: `*` işareti iki farklı yerde iki farklı anlama geliyor.

* **Tanımlarken** (`int* p`): "p bir pointer'dır" demek.
* **Kullanırken** (`*p = 100`): "p'nin gösterdiği yere git" demek.

**`nullptr`:** Hiçbir yeri göstermeyen pointer. Bir pointer'ı tanımlayıp henüz bir yere bağlamadıysanız ona `nullptr` verin. `nullptr` olan bir pointer'ı dereference ederseniz program çöker (meşhur "segmentation fault"). Çökmesi aslında iyidir, en azından hatayı fark edersiniz.

> **Tahta önerisi:** İki kutu çizin. Birinin üstüne `sayi`, içine `42` yazın. Diğerinin üstüne `p`, içine `sayi`'nin adresini yazın ve `p` kutusundan `sayi` kutusuna bir ok çekin. Pointer bu oktan ibarettir.

### 3.3 Aynı işi üç yoldan yapmak: swap

Demo dosyası: [`02_swap_uc_yol.cpp`](02_pointers/02_swap_uc_yol.cpp)

İki sayının yerini değiştiren fonksiyonu üç şekilde yazıyoruz:

```cpp
void swap_deger(int a, int b);       // ÇALIŞMAZ: kopyalar değişir, orijinaller yerinde kalır
void swap_pointer(int* a, int* b);   // ÇALIŞIR: C usulü, adres gönder, adresteki değeri değiştir
void swap_referans(int& a, int& b);  // ÇALIŞIR: C++ usulü, aynı iş, daha temiz yazım
```

Çağırırken farka dikkat:

```cpp
swap_pointer(&x, &y);    // adres gönderiyoruz, & şart
swap_referans(x, y);     // referansta & yazmıyoruz, derleyici halleder
```

Buradaki asıl ders şu: **referans, kılık değiştirmiş bir pointer'dır.** Arka planda ikisi de aynı makine kodunu üretir. Referans sadece hiçbir zaman `nullptr` olamayan ve başka bir yeri göstermeye çevrilemeyen, daha güvenli bir pointer'dır.

Hangisini ne zaman kullanalım?

* Normal C++ kodunda **referans** tercih edin.
* "Burada bir şey olmayabilir" diyeceğiniz durumlarda (değer `nullptr` olabilirse) **pointer** kullanın.
* Donanım register'ları, C kütüphaneleri ve dizilerle çalışırken **pointer** kaçınılmazdır.

### 3.4 Diziler ve pointer aritmetiği

Demo dosyası: [`03_pointer_aritmetigi.cpp`](02_pointers/03_pointer_aritmetigi.cpp)

Bir dizi bellekte **yan yana** duran elemanlardan oluşur. Demo kodunu çalıştırdığınızda `int16_t` dizisinin elemanlarının adreslerinin ikişer ikişer arttığını göreceksiniz, zira her eleman 2 bayttır.

```cpp
int16_t ornekler[5] = {100, 200, 300, 400, 500};
int16_t* p = ornekler;     // dizinin adı, ilk elemanın adresine dönüşür
```

Şimdi sihirli kısım:

* `p + 1` ifadesi adresi **1 bayt değil, 1 eleman** ilerletir. `int16_t` için 2 bayt, `int` için 4 bayt, `double` için 8 bayt.
* `ornekler[3]` yazmakla `*(ornekler + 3)` yazmak **birebir aynı şeydir.** Köşeli parantez, aslında pointer aritmetiğinin kısaltmasıdır.

Bunu bilmenin pratik bir sonucu var: Bir diziyi fonksiyona verdiğinizde aslında sadece **ilk elemanın adresi** gider. Dizinin boyutu kaybolur. Bu yüzden C usulü fonksiyonlarda hep boyutu ayrıca göndeririz:

```cpp
double ortalama(const double* dizi, int n);
```

Buradaki `const`, "bu fonksiyon diziyi okuyacak ama değiştirmeyecek" sözüdür. Ödevlerde bu imzayı çok göreceksiniz.

> **Sık yapılan hata:** Dizinin sınırının dışına çıkmak. `ornekler[5]` yazarsanız derleyici sizi uyarmaz, program da her zaman çökmez. Sadece başka bir değişkenin dairesine girip oradaki değeri okursunuz ya da bozarsınız. Bu tür hatalar en sinsi olanlarıdır.

### 3.5 Stack, heap ve dinamik bellek

Demo dosyası: [`04_dinamik_bellek.cpp`](02_pointers/04_dinamik_bellek.cpp)

Belleğin iki ana bölgesi var:

| | Stack (yığıt) | Heap (öbek) |
|---|---|---|
| Kim yönetir? | Derleyici, otomatik | Siz, elle |
| Ömrü | Fonksiyon bitince silinir | Siz silene kadar yaşar |
| Boyutu | Küçük (birkaç MB) | Büyük |
| Hızı | Çok hızlı | Görece yavaş |
| Nasıl alınır? | `int x;` `double dizi[100];` | `new` ile |

Ne zaman heap'e ihtiyaç duyarız? Dizinin boyutunu derleme anında bilmiyorsak (kullanıcıdan alıyorsak) ya da veri, onu oluşturan fonksiyon bittikten sonra da yaşamalıysa.

```cpp
double* tampon = new double[n];   // heap'ten n tane double iste
// ... kullan ...
delete[] tampon;                  // işin bitince geri ver
tampon = nullptr;                 // eski adresi tutmaya devam etmesin
```

Kurallar basit, lakin uyması zor:

* `new` ile aldığınızı `delete` ile, `new[]` ile aldığınızı `delete[]` ile geri verin.
* Geri vermeyi unutursanız **bellek sızıntısı (memory leak)** olur. Bir masaüstü programında belki fark edilmez, ama aylarca açık kalan bir gömülü cihazda bu, cihazın günün birinde kilitlenmesi demektir.
* Silinmiş belleği kullanmaya devam ederseniz (dangling pointer) program tuhaf davranır.

> **Gömülü dünyadan bir not:** Pek çok güvenlik kritik gömülü sistemde (otomotiv, havacılık) `new` kullanmak **yasaktır.** Bütün bellek, program başlarken sabit boyutlu dizilerle ayrılır. Ring buffer ödevinde bu yaklaşımı deneyeceksiniz.

### 3.6 `std::vector`: Elle uğraşmak istemeyenlere

Demo dosyası: [`05_vector.cpp`](02_pointers/05_vector.cpp)

Masaüstü C++ yazarken yukarıdaki dertlerin çoğundan `std::vector` ile kurtuluruz:

```cpp
std::vector<double> olcumler;
olcumler.push_back(1.2);       // otomatik büyür
olcumler.size();               // boyutunu bilir
double* ham = olcumler.data(); // gerekirse ham pointer'ı verir
```

İşi bittiğinde belleği kendisi temizler, `delete` yazmanıza gerek kalmaz. Peki madem bu kadar kolay, niye pointer öğrendik? Çünkü `vector` da içeride tam olarak `new` ve `delete` yapıyor. Arka planı bilmeyen, önündeki aracı da tam anlayamaz. Bir de 12 Kasım'da kendi sınıflarımızı yazarken bu bilgi lazım olacak.

### 3.7 Elektronikçi için pointer: Register'a yazmak

Demo dosyası: [`06_register_analoji.cpp`](02_pointers/06_register_analoji.cpp)

Şimdiye kadar "pointer adres tutar" dedik. Mikrodenetleyicide bazı adresler RAM'e değil, **donanıma** bağlıdır. Mesela bir STM32 kartında `0x40020014` adresine yazdığınız her bit, doğrudan bir pinin voltajını belirler.

```cpp
volatile uint32_t* GPIOA_ODR = reinterpret_cast<volatile uint32_t*>(0x40020014);
*GPIOA_ODR |= (1u << 5);   // PA5 pinine bağlı LED yandı!
```

Bu satırda bugün öğrendiğimiz her şey bir arada: sabit genişlikli tip, pointer, dereference ve bit maskesi. `volatile` kelimesi ise derleyiciye "bu adresteki değer senin haberin olmadan değişebilir, optimize etmeye kalkma" der.

Bilgisayarımızda o adreste bir donanım olmadığı için demo kodunda sahte bir register üzerinden aynı işlemi simüle ediyoruz. Birinci ödev de tam olarak bunun üzerine kurulu.

## Ara (10 dk)

## 4. Uygulama: Sıralama Algoritmaları (35 dk)

Demo dosyası: [`siralama.cpp`](03_sorting/siralama.cpp)

Şimdi bütün bunları bir algoritma yazarak pekiştiriyoruz. Dosyayı açın, içinde bazı fonksiyonlar dolu, bazıları sizi bekliyor.

```bash
g++ -std=c++17 -Wall -Wextra -O2 siralama.cpp -o siralama
./siralama
```

### 4.1 Bubble Sort (birlikte, 8 dk)

Fikir: Yan yana iki elemana bak, soldaki büyükse yerlerini değiştir. Diziyi baştan sona böyle gezersen en büyük eleman en sona "kabarcık gibi" çıkar. Bunu tekrarla.

```
[5 2 9 1]  →  [2 5 9 1]  →  [2 5 9 1]  →  [2 5 1 9]   (1. tur bitti, 9 yerine oturdu)
```

Kodda dikkat edilecek iki şey:

* Yer değiştirmek için derste yazdığımız `degistir(&dizi[j], &dizi[j + 1])` fonksiyonunu kullanıyoruz. İşte pointer'lı swap'ın gerçek hayattaki karşılığı.
* `degisti` bayrağı: Bir turda hiç değişim olmadıysa dizi zaten sıralıdır, boşuna devam etmeyiz.

### 4.2 Selection Sort (siz, 9 dk)

Fikir: Kalan kısmın en küçüğünü bul, başa koy.

```
[5 2 9 1]  →  en küçük 1, başa al  →  [1 2 9 5]
[1|2 9 5]  →  kalanların en küçüğü 2, zaten yerinde
[1 2|9 5]  →  kalanların en küçüğü 5  →  [1 2 5 9]
```

İpucu: i. turda `[i, n)` aralığındaki en küçük elemanın **değerini değil, indeksini** tutun. Sonra `dizi[i]` ile o indeksi değiştirin.

### 4.3 Insertion Sort (siz, 9 dk)

Fikir: Elinizdeki iskambil kâğıtlarını dizer gibi. Sol taraf hep sıralı. Sıradaki kartı alıp, sol taraftaki kendisinden büyükleri birer sağa kaydırarak doğru yere yerleştirin.

```
[2 5 9 | 1]  →  anahtar = 1
             →  9'u sağa kaydır, 5'i sağa kaydır, 2'yi sağa kaydır
             →  [1 2 5 9]
```

İpucu: `int anahtar = dizi[i]` diye sıradaki elemanı saklayın, yoksa kaydırırken üzerine yazarsınız.

### 4.4 Hız karşılaştırması (4 dk)

Programın sonunda 20000 elemanlı rastgele bir dizi dört yöntemle sıralanıyor ve süreler ölçülüyor. Bizim makinemizde çıkan sonuçlar kabaca şöyle:

| Algoritma | Karmaşıklık | Süre (20000 eleman) |
|---|:---:|---:|
| Bubble Sort | O(n²) | yaklaşık 1000 ms |
| Selection Sort | O(n²) | yaklaşık 500 ms |
| Insertion Sort | O(n²) | yaklaşık 60 ms |
| `std::sort` | O(n log n) | yaklaşık 1 ms |

Bu tablodan çıkacak iki ders var:

1. Aynı karmaşıklığa sahip algoritmalar bile pratikte çok farklı hızlarda çalışabilir. Insertion sort, az eleman taşıdığı ve belleği sıralı gezdiği için ötekilerden çok daha hızlı.
2. Standart kütüphanenin işini yapan bir fonksiyonu varsa onu kullanın. `std::sort` bizim yazdıklarımızdan yaklaşık bin kat hızlı.

> **Bonus konu:** Süre ölçen `sure_olc` fonksiyonu, parametre olarak **bir fonksiyon** alıyor: `void (*)(int*, int)`. Bu bir **fonksiyon pointer'ıdır.** Evet, fonksiyonların da bellekte bir adresi var. İkinci günde bunun daha şık bir hâlini (sanal fonksiyonlar) göreceğiz.

### 4.5 Final projesine köprü: Medyan (5 dk)

Bir sıcaklık sensörü düşünün. Normalde 25 derece civarında ölçüyor, ama arada bir elektromanyetik parazit yüzünden saçma bir değer (999) geliyor:

```
25  26  24  25  999  25  26  24  25
```

* **Ortalama** alırsak sonuç yaklaşık **133** derece çıkıyor. Tek bir hatalı örnek bütün ölçümü mahvetti.
* **Medyan** alırsak (sıralayıp ortadakini seçersek) sonuç **25** çıkıyor. Parazit hiç etkilemedi.

İşte bu, sinyal işlemede **medyan filtrenin** temel fikri. Yazdığınız sıralama algoritması bir anda gerçek bir mühendislik problemini çözdü. 4 numaralı ödevde bunu kayan pencereyle bir sinyalin tamamına uygulayacaksınız, 12 Kasım'da da bir sınıfın içine yerleştireceğiz.

## 5. Kapanış ve Ödevler (10 dk)

Bugün öğrendiklerimizin hâsılı:

* C++'ta her değişkenin bir tipi ve bellekte bir adresi var.
* Pointer, bir adresi tutan değişkendir. Referans ise onun daha güvenli bir kardeşidir.
* Diziler bellekte yan yana durur ve fonksiyonlara adres olarak geçer.
* Heap'ten aldığınızı geri vermek sizin sorumluluğunuzdadır.
* Bit işlemleri ve pointerlar bir araya geldiğinde donanımla konuşabilirsiniz.

Hafta boyunca yapacağınız ödevler [02_odevler](../02_odevler/README.md) sayfasında. Her ödevin kendi README dosyası, hazır bir iskelet kodu ve otomatik testleri var. Kısaca:

| No | Ödev | Bugünkü hangi konuya dayanıyor? |
|:---:|---|---|
| 01 | Register Simülatörü | Bit işlemleri, pointer |
| 02 | ADC ve Kuantizasyon | Tipler, diziler, pointer ile dizi aktarımı |
| 03 | Ring Buffer | Diziler, modüler aritmetik, statik bellek |
| 04 | Hareketli Ortalama ve Medyan Filtre | Sıralama, diziler |
| 05 | Hamming(7,4) Kodu | Bit işlemleri |

**Son teslim: 11 Kasım 2026, saat 23.59**

Takıldığınız yerde çekinmeden sorun. Bir hafta sonra görüşmek üzere.

## Ek Okuma

* [learncpp.com](https://www.learncpp.com): Bölüm 12 (referans ve pointer) bilhassa faydalı.
* [cppreference.com](https://en.cppreference.com): Her şeyin resmî ve kesin açıklaması. Başta biraz ağır gelir, zamanla vazgeçilmez olur.
* [VisuAlgo: Sorting](https://visualgo.net/en/sorting): Sıralama algoritmalarını adım adım animasyonla izleyebilirsiniz.

## Eğitmen notu

Bu kısım dersi anlatacak arkadaşlar için. Süre tablosu bir öneridir, sınıfın seviyesine göre esnetin.

**Eski ve yeni plan:**

| Bölüm | Eski süre | Önerilen süre |
|---|:---:|:---:|
| Açılış | 10 dk | 5 dk |
| Kurulum | 25 dk | 20 dk |
| C++ sözdizimi | 35 dk | 95 dk (40 + 55) |
| Bellek ve pointerlar | 50 dk | 45 dk |
| Uygulama: sıralama | 50 dk | 35 dk |
| Kapanış | 15 dk | 10 dk |
| Aralar | 20 dk (2 ara) | 30 dk (3 ara) |
| **Toplam** | **205 dk** | **240 dk** |

**Planda hiç tampon yok.** 240 dakika üst sınırın kendisi. En gerçekçi tampon kurulumdur: 00_kurulum sayfasını dersten önce duyurup herkesin evde bitirmesini sağlarsak 10 ile 15 dakika kazanırız.

**Pointer bölümünden kısaltmak gerekirse (45 dk yerine 35 dk):**

* 3.6 `std::vector` kısmını okumaya bırakın. Ödev 02, 03 ve 04'te zaten yasak.
* 3.5 heap kısmını 3 dakikaya indirin, `new` ve `delete` eşleşmesini söylemek kâfi. Beş ödevin hiçbirinde dinamik bellek kullanılmıyor. Lakin RAII konusu buna yaslandığı için 12 Kasım'ın başında kısa bir tekrar gerekir.
* 3.7 kısmını tek örneğe indirin, Ödev 01 aynı konuyu işliyor.
* 3.1 ile 3.4 arasına dokunmayın. Ödevlerin imzaları pointer parametre ve boyut üzerine kurulu, burası oturmazsa hafta zor geçer.

**Sıralama bölümünden kısaltmak gerekirse (35 dk yerine 25 dk):**

* Selection sort'u eve bırakın, 9 dakika kazanırsınız.
* **Insertion sort'u kesmeyin.** `siralama.cpp` içindeki `medyan` fonksiyonu onu çağırıyor. Boş kalırsa dizi sıralanmaz, 4.5'teki örnekte ortadaki eleman olan 999 döner ve anlatmak istediğimizin tam tersi görünür. Ödev 04 de insertion sort istiyor.
* Hız karşılaştırmasını 1 dakikada, tablo üzerinden geçin. Doldurulmamış fonksiyonlar çıktıda `[YANLIS]` olarak görünür, bunu önceden söyleyin.

**Sözdizimi bölümünde sıkışırsanız:**

* `auto`, `std::getline` ve `do while` birer cümleyle geçilebilir.
* Sınıf önceden kod yazmışsa 2.1, 2.2 ve 2.3 hızlıca özetlenebilir, kazanılan süre pointerlara aktarılır.
* 2.11 bit işlemlerini kısmayın, Ödev 01 ve Ödev 05 doğrudan buna dayanıyor.

**Plana girmeyen bir eksik:** Ödev 03'teki `RingBuffer` bir `struct` ve fonksiyonları `RingBuffer*` alıyor, yani öğrencilerin `rb->bas` yazımını bilmesi gerekiyor. Bu iki konu birinci günde anlatılmıyor. Kapanışta ödevleri tanıtırken 2 dakika ayırıp gösterin.
