# İTÜ Yonga | C++ Ödev 01: Register Simülatörü

## Görevin Amacı

Bir mikrodenetleyicide LED yakmak, bir pini giriş ya da çıkış yapmak, bir zamanlayıcıyı başlatmak hep aynı işe çıkar: **belirli bir adresteki belirli bitleri değiştirmek.** Bu ödevin amacı, birinci günde gördüğümüz bit işlemlerini ve pointerları kullanarak bir **register işlem kütüphanesi** yazmaktır.

Fonksiyonlarınız register'a doğrudan değil, onun adresini tutan bir `volatile uint32_t*` pointer üzerinden erişecek. Yani yazdığınız kod, gerçek bir STM32 kartına neredeyse hiç değiştirilmeden taşınabilir.

## Teknik İsterler ve Mimari

Kütüphaneniz iki seviyede çalışacak:

1. **Tek bit işlemleri:** Bir bitin değerini 1 yapmak, 0 yapmak, terslemek ve okumak.
2. **Bit alanı (bit field) işlemleri:** Register içinde birden fazla bitten oluşan bir alana değer yazmak ve okumak.

### Bit Alanı Mantığı

Gerçek register'larda ayarlar ekseriya birkaç bitlik alanlar hâlinde durur. Mesela STM32'deki `GPIOx_MODER` register'ında her pinin modu **2 bitle** belirlenir ve pin $k$ için bu alan $2k$ ile $2k+1$ numaralı bitlerdedir.

```
 bit:   31 30 | 29 28 | ... | 11 10 | ... |  1  0
 pin:    15   |  14   | ... |   5   | ... |   0
```

Konumu $p$, genişliği $w$ olan bir alanın maskesi şöyle hesaplanır:

$$
\text{maske} = (2^{w} - 1) \ll p
$$

Bir alana yeni değer yazmak iki adımdır: önce alanı temizle, sonra yeni değeri yerleştir.

```cpp
reg = (reg & ~maske) | (deger << konum);
```

> **Örnek:** `MODER = 0xA8000000` iken PA5 pinini çıkış yapmak için 10 ve 11 numaralı bitlere `01` yazılır. Sonuç `0xA8000400` olur, diğer bütün bitler olduğu gibi kalır.

## Fonksiyon Haritası (Interface)

| Fonksiyon | Girdi | Çıktı | Açıklama |
|---|---|:---:|---|
| `bit_set` | `reg`, `n` | | n. biti 1 yapar |
| `bit_clear` | `reg`, `n` | | n. biti 0 yapar |
| `bit_toggle` | `reg`, `n` | | n. biti tersler |
| `bit_read` | `reg`, `n` | `bool` | n. bitin değerini döner |
| `alan_yaz` | `reg`, `konum`, `genislik`, `deger` | `bool` | Bit alanına değer yazar, başarı durumunu döner |
| `alan_oku` | `reg`, `konum`, `genislik` | `uint32_t` | Bit alanını sağa yaslanmış olarak döner |
| `bit_say` | `deger` | `unsigned` | 1 olan bitlerin sayısını döner |

Tam imzalar ve geçersiz durumlarda ne olacağı [`include/register.hpp`](include/register.hpp) dosyasında yazıyor.

## Tasarım Kuralları

1. **Sınır kontrolü:** `n > 31` ya da `reg == nullptr` gelirse fonksiyon hiçbir şey yapmamalı, `bit_read` ise `false` dönmeli. Program asla çökmemeli.
2. **Döngüsüz tek bit işlemleri:** `bit_set`, `bit_clear`, `bit_toggle` ve `bit_read` içinde döngü kullanılmamalı. Her biri tek bir bit işlemi ifadesiyle çözülebilir.
3. **Atomik alan yazma:** `alan_yaz` geçersiz bir durumla karşılaşırsa register'ı **hiç değiştirmeden** `false` dönmeli. Önce kontrol, sonra yazma.
4. **32 bit kenar durumu:** `genislik == 32` olduğunda `1u << 32` ifadesi C++'ta **tanımsız davranıştır.** Bu durumu ayrıca ele almalısınız.
5. **Hazır araç yok:** `std::bitset`, `__builtin_popcount` ve benzeri hazır fonksiyonlar yasak. Sadece `<cstdint>` başlığını kullanabilirsiniz.

## Teslim Sınırları

| Sınır | Değer |
|---|---|
| Değiştirilecek dosya | Sadece `src/register.cpp` |
| İzin verilen başlıklar | `<cstdint>` |
| Yasak olanlar | `std::bitset`, `__builtin_*`, dinamik bellek (`new`, `malloc`) |
| Tek bit fonksiyonlarında döngü | Yasak |
| Derleme bayrakları | `-std=c++17 -Wall -Wextra -Wpedantic -Werror` |

## Teslim Süreci ve İstenenler

Klasörü `teslimler/<kullanici_adiniz>/odev01_register/` altına kopyalayıp orada çalışın ve Pull Request açın.

**Teslim etmeniz gerekenler:**

1. `src/register.cpp` dosyasının doldurulmuş hâli.
2. PR açıklamasında `make test` çıktısının ekran görüntüsü.
3. PR açıklamasında şu soruya iki üç cümlelik bir cevap: **`volatile` kelimesini kaldırsaydık gerçek bir mikrodenetleyicide ne tür bir sorun yaşayabilirdik?**

> **İpucu:** `bit_say` için her biti tek tek kontrol eden bir döngü yazabilirsiniz, bu kabul. Lakin `x & (x - 1)` ifadesinin ne yaptığını kâğıt üzerinde birkaç sayıyla deneyin. Sizi daha zarif bir çözüme götürecek.
