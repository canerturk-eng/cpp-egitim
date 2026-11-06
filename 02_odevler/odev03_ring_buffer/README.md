# İTÜ Yonga | C++ Ödev 03: Ring Buffer (Dairesel Tampon)

## Görevin Amacı

Bir UART hattından gelen baytlar, bir ADC'nin ürettiği örnekler, bir ses kartının çaldığı veriler... Hepsi sürekli akar ve hepsinin **son birkaç örneğini** bir yerde tutmamız gerekir. Bu iş için gömülü dünyanın en sevilen veri yapısı **ring buffer** (dairesel tampon) kullanılır.

Bu ödevin amacı, dinamik bellek kullanmadan, **sabit boyutlu bir dizi** üzerinde çalışan bir ring buffer yazmaktır. Burada yazacağınız yapı, 12 Kasım'daki dijital filtrenin geçmiş örnekleri tuttuğu yer olacak.

## Teknik İsterler ve Mimari

Düz bir diziyi kuyruk olarak kullansaydık, her eleman çıkarıldığında bütün diziyi bir sola kaydırmamız gerekirdi. Bu O(n) maliyet demek. Ring buffer ise elemanları yerinde bırakır, sadece **indeksleri** döndürür. Dizinin sonuna gelindiğinde baştan devam edilir, sanki dizinin iki ucu birbirine bağlıymış gibi.

```
 RB_KAPASITE = 8,  bas = 6,  adet = 4

 indeks:   0     1     2     3     4     5     6     7
         ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
         │  C  │  D  │     │     │     │     │  A  │  B  │
         └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
                        ▲                       ▲
                  sıradaki yazma            bas (en eski)
```

Burada A en eski, D en yeni örnek. Tampon dizinin sonundan başına sarmış durumda.

### İndeks Hesapları

Bütün sihir mod (`%`) operatöründe:

$$
\text{yazılacak indeks} = (\text{bas} + \text{adet}) \bmod K
$$

$$
\text{gecmis. örnek} = \big(\text{bas} + \text{adet} - 1 - \text{gecmis}\big) \bmod K
$$

Burada $K$ tampon kapasitesidir. `gecmis = 0` en yeni örneği, `gecmis = adet - 1` en eski örneği verir.

> **Örnek:** Yukarıdaki durumda yeni bir E örneği eklenirse $(6 + 4) \bmod 8 = 2$ numaralı indekse yazılır. `rb_bak(rb, 0)` çağrısı E'yi, `rb_bak(rb, 3)` çağrısı ise B'yi döner.

### İki Ekleme Modu

| Mod | Tampon doluyken ne olur? | Nerede kullanılır? |
|---|---|---|
| `rb_ekle` | Ekleme reddedilir, `false` döner | UART alıcısı gibi veri kaybının istenmediği yerler |
| `rb_ekle_ezerek` | En eski örneğin üzerine yazılır | Filtreler, zira bize hep **son K örnek** lazım |

## Fonksiyon Haritası (Interface)

| Fonksiyon | Girdi | Çıktı | Açıklama |
|---|---|:---:|---|
| `rb_baslat` | `rb` | | Tamponu boşaltır |
| `rb_ekle` | `rb`, `deger` | `bool` | Doluysa eklemez |
| `rb_ekle_ezerek` | `rb`, `deger` | | Doluysa en eskinin üzerine yazar |
| `rb_cikar` | `rb`, `deger*` | `bool` | En eskiyi çıkarıp pointer ile döner |
| `rb_bak` | `rb`, `gecmis`, `deger*` | `bool` | Çıkarmadan bakar, 0 en yeni |
| `rb_adet` | `rb` | `size_t` | Tampondaki örnek sayısı |
| `rb_bos` / `rb_dolu` | `rb` | `bool` | Durum sorguları |

`RingBuffer` yapısının alanları ve tam imzalar [`include/ring_buffer.hpp`](include/ring_buffer.hpp) dosyasında. Fonksiyonların sonucu **pointer parametre** ile döndürdüğüne dikkat edin (`float* deger`), birinci günde swap örneğinde gördüğümüz yöntemin aynısı.

## Tasarım Kuralları

1. **Yapının anlamı sabit:** `bas` her zaman en eski örneğin indeksi, `adet` her zaman tampondaki örnek sayısı olmalı. Testler bu alanlara doğrudan da bakıyor. Yapıya yeni alan eklemeyin.
2. **O(1) işlemler:** `rb_ekle`, `rb_ekle_ezerek`, `rb_cikar` ve `rb_bak` içinde **döngü olmamalı.** Hiçbir elemanı kaydırmayın, sadece indeksleri hesaplayın.
3. **İndeks hep geçerli:** `bas` değeri hiçbir zaman `RB_KAPASITE` ve üstüne çıkmamalı. 100 bin örnek ekleyen bir test var, indeks taşarsa yakalanırsınız.
4. **Statik bellek:** `new`, `malloc` ve STL kapları (`std::vector`, `std::deque`, `std::queue`) yasak. Bütün veri, yapının içindeki sabit dizide durmalı.
5. **Savunmacı kod:** `rb` ya da `deger` pointer'ı `nullptr` gelirse program çökmemeli. Başlık dosyasında her fonksiyonun bu durumda ne döneceği yazıyor.

## Teslim Sınırları

| Sınır | Değer |
|---|---|
| Değiştirilecek dosya | Sadece `src/ring_buffer.cpp` |
| İzin verilen başlıklar | `<cstddef>` |
| Yasak olanlar | `new`, `malloc`, bütün STL kapları |
| Ekleme, çıkarma ve bakma fonksiyonlarında döngü | Yasak |
| Derleme bayrakları | `-std=c++17 -Wall -Wextra -Wpedantic -Werror` |

## Teslim Süreci ve İstenenler

Klasörü `teslimler/<kullanici_adiniz>/odev03_ring_buffer/` altına kopyalayıp orada çalışın ve Pull Request açın.

**Teslim etmeniz gerekenler:**

1. `src/ring_buffer.cpp` dosyasının doldurulmuş hâli.
2. PR açıklamasında `make test` çıktısının ekran görüntüsü.
3. PR açıklamasında şu soruya kısa bir cevap: **Kapasiteyi 8 yerine 2'nin bir kuvveti olan bir sayı seçmek, mod işlemini nasıl hızlandırabilir?** (İpucu: `x % 8` ile `x & 7` ifadelerini karşılaştırın.)

> **İpucu:** `rb_bak` fonksiyonunda `bas + adet - 1 - gecmis` ifadesi `size_t` tipinde. `size_t` işaretsizdir, yani negatif olamaz. Önce `gecmis >= adet` kontrolünü yaparsanız bu ifade hiçbir zaman sıfırın altına düşmez. Kontrolü unutursanız sayı başa sarar ve dev bir indeks elde edersiniz.
