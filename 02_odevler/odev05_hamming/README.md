# İTÜ Yonga | C++ Ödev 05: Hamming(7,4) Hata Düzeltme Kodu

## Görevin Amacı

Gürültülü bir kanaldan veri gönderdiğinizde bazı bitler ters dönebilir. Bir uydu bağlantısında, bir RAM çipinde (ECC bellekler) ya da bir QR kodda bu hatayı sadece **tespit etmek** yetmez, alıcının hatayı **kendi başına düzeltmesi** gerekir.

Bu ödevin amacı, 1950 yılında Richard Hamming'in bulduğu ve hâlâ kullanılan **Hamming(7,4)** kodunu yazmaktır. 4 bitlik veriye 3 parite biti eklenir ve 7 bitin herhangi birinde oluşan tek bir hata alıcı tarafında düzeltilebilir.

## Teknik İsterler ve Mimari

### Bit Düzeni

7 bitlik kod sözcüğünde konumlar 1'den 7'ye numaralanır. **2'nin kuvveti olan konumlar (1, 2, 4) parite bitleridir**, geri kalanlar veri bitleridir:

| Hamming konumu | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| İçerik | p1 | p2 | **d1** | p3 | **d2** | **d3** | **d4** |
| `uint8_t` içindeki bit | bit0 | bit1 | bit2 | bit3 | bit4 | bit5 | bit6 |

4 bitlik girişte ise `d1 = bit0`, `d2 = bit1`, `d3 = bit2`, `d4 = bit3`.

### Kodlama

Her parite biti, konum numarasının ikili gösteriminde kendi bitini içeren konumları kapsar:

$$
p_1 = d_1 \oplus d_2 \oplus d_4 \qquad \text{(konum 3, 5, 7)}
$$

$$
p_2 = d_1 \oplus d_3 \oplus d_4 \qquad \text{(konum 3, 6, 7)}
$$

$$
p_3 = d_2 \oplus d_3 \oplus d_4 \qquad \text{(konum 5, 6, 7)}
$$

### Çözme ve Sendrom

Alıcı, gelen 7 bit üzerinden üç kontrol hesaplar:

$$
s_1 = c_1 \oplus c_3 \oplus c_5 \oplus c_7 \qquad
s_2 = c_2 \oplus c_3 \oplus c_6 \oplus c_7 \qquad
s_3 = c_4 \oplus c_5 \oplus c_6 \oplus c_7
$$

İşin güzel kısmı şu: $s_3 s_2 s_1$ üçlüsünü ikili bir sayı olarak okursanız **hatalı bitin konumunu** doğrudan elde edersiniz. Sendrom 0 ise hata yoktur.

$$
\text{sendrom} = s_1 + 2\,s_2 + 4\,s_3
$$

> **Örnek:** Veri `0xB` (yani `d1=1 d2=1 d3=0 d4=1`) için `p1=1 p2=0 p3=0` olur ve kod sözcüğü `0x55` (`1010101`) çıkar. Kanalda 6. bit ters dönerse alıcı sendromu 6 hesaplar, o biti geri çevirir ve `0xB` verisini doğru şekilde kurtarır.

## Fonksiyon Haritası (Interface)

| Fonksiyon | Girdi | Çıktı | Açıklama |
|---|---|:---:|---|
| `hamming_kodla` | `veri4` | `uint8_t` | 4 bit veriyi 7 bitlik kod sözcüğüne çevirir |
| `hamming_coz` | `kod7`, `hata_konumu*` | `uint8_t` | Tek bit hatasını düzeltir, 4 bit veriyi döner, sendromu pointer ile yazar |
| `hamming_mesafesi` | `a`, `b` | `unsigned` | İki bayt arasında farklı olan bit sayısı |

Tam imzalar [`include/hamming.hpp`](include/hamming.hpp) dosyasında.

## Tasarım Kuralları

1. **Bit düzenine sadakat:** Yukarıdaki tablo bağlayıcıdır. Bitleri başka sırayla yerleştirirseniz kodunuz kendi içinde çalışsa bile testleri geçemez, zira karşı taraftaki alıcı başka bir düzen bekliyor olacaktır. Haberleşmede standart, her şeydir.
2. **Kullanılmayan bitler:** `veri4` içindeki üst 4 bit ve `kod7` içindeki bit7 yok sayılmalı. Kodlarken bit7 her zaman 0 olmalı.
3. **İsteğe bağlı çıktı:** `hata_konumu` pointer'ı `nullptr` gelirse içine yazmayın, sadece veriyi döndürün.
4. **Tablo yok:** 16 elemanlı hazır bir kod tablosu yazıp cevabı oradan döndürmek yasak. Parite bitlerini XOR ile hesaplayın.
5. **Hazır araç yok:** `std::bitset` ve `__builtin_popcount` yasak.

## Teslim Sınırları

| Sınır | Değer |
|---|---|
| Değiştirilecek dosya | Sadece `src/hamming.cpp` |
| İzin verilen başlıklar | `<cstdint>` |
| Yasak olanlar | `std::bitset`, `__builtin_*`, sabit kod tablosu, dinamik bellek |
| Derleme bayrakları | `-std=c++17 -Wall -Wextra -Wpedantic -Werror` |

## Teslim Süreci ve İstenenler

Klasörü `teslimler/<kullanici_adiniz>/odev05_hamming/` altına kopyalayıp orada çalışın ve Pull Request açın.

**Teslim etmeniz gerekenler:**

1. `src/hamming.cpp` dosyasının doldurulmuş hâli.
2. PR açıklamasında `make test` çıktısının ekran görüntüsü.
3. PR açıklamasında şu soruya kısa bir cevap: **Kod sözcüğünde aynı anda iki bit ters dönerse ne olur?** Kendiniz bir örnek üzerinde deneyip sonucu yazın.
4. **Bonus:** Ayrı bir `bonus/` klasöründe, her biti $p$ olasılığıyla ters çeviren bir **ikili simetrik kanal (BSC)** simülasyonu yazın. $p = 0{,}001$ ile $p = 0{,}1$ arasında birkaç değer için kodlamalı ve kodlamasız bit hata oranlarını (BER) karşılaştıran bir tablo ya da grafik ekleyin.

> **İpucu:** Konumları 1'den, bitleri 0'dan numaralamak kafa karıştırabilir. Kodun başına `konum` alıp ilgili biti döndüren küçük bir `static` yardımcı fonksiyon yazarsanız gerisi kendiliğinden toparlanır.
