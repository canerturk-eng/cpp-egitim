# İTÜ Yonga | C++ Ödev 02: ADC ve Kuantizasyon Simülatörü

## Görevin Amacı

Gerçek dünya analogdur, işlemci ise sadece sayılarla çalışır. Aradaki köprüyü **ADC (Analog to Digital Converter)** kurar. Bu ödevin amacı, bir ADC'nin yaptığı **örnekleme** ve **kuantizasyon** işlemlerini C++ ile simüle etmek ve kuantizasyonun sinyale kattığı hatayı ölçmektir.

Ödevin sonunda, sinyal işleme derslerinde ezberlenen meşhur **"her bit 6 dB kazandırır"** kuralını kendi kodunuzla doğrulamış olacaksınız.

## Teknik İsterler ve Mimari

Bir ADC, $0$ ile $V_{ref}$ arasındaki voltajı $N$ bitlik bir koda çevirir. Bu aralık $2^N$ eşit basamağa bölünür.

### Çözünürlük ve Kodlama

Bir basamağın (LSB) voltaj karşılığı:

$$
\text{LSB} = \frac{V_{ref}}{2^{N}}
$$

Analog voltajdan dijital koda geçiş (sonuç $[0,\ 2^N - 1]$ aralığına sıkıştırılır):

$$
\text{kod} = \left\lfloor \frac{V_{in}}{\text{LSB}} \right\rfloor
$$

Koddan voltaja geri dönerken basamağın **ortasını** alıyoruz, böylece hata $\pm\frac{\text{LSB}}{2}$ sınırında kalır:

$$
V_{out} = \left(\text{kod} + \tfrac{1}{2}\right) \cdot \text{LSB}
$$

### Test Sinyali

$$
x[i] = \text{ofset} + A \cdot \sin\!\left(2\pi \frac{f}{f_s}\, i\right), \qquad i = 0, 1, \dots, n-1
$$

### Hata Ölçümü

$$
\text{RMS hata} = \sqrt{\frac{1}{n}\sum_{i=0}^{n-1}\big(a[i] - b[i]\big)^2}
$$

$$
\text{SQNR}_{dB} = 10 \log_{10}\frac{\sum \big(x[i] - \bar{x}\big)^2}{\sum \big(x[i] - x_q[i]\big)^2}
$$

Tam ölçekli bir sinüs için teorik değer:

$$
\text{SQNR}_{dB} \approx 6.02\,N + 1.76
$$

> **Örnek:** `vref = 3.3` ve `bit = 12` iken LSB yaklaşık 0,806 mV olur. 1,0 V'luk bir girişin kodu 1241 çıkar. Aynı sinyali 8 bit yerine 12 bitle kuantize ederseniz SQNR yaklaşık 24 dB artar.

## Fonksiyon Haritası (Interface)

| Fonksiyon | Girdi | Çıktı | Açıklama |
|---|---|:---:|---|
| `lsb_hesapla` | `vref`, `bit` | `double` | Bir basamağın voltaj karşılığı |
| `adc_kodla` | `volt`, `vref`, `bit` | `uint16_t` | Voltajı koda çevirir, sınırlara sıkıştırır |
| `adc_coz` | `kod`, `vref`, `bit` | `double` | Kodu basamak ortası voltajına çevirir |
| `sinus_uret` | `tampon`, `n`, `frekans`, `fs`, `genlik`, `ofset` | | Tampona örneklenmiş sinüs yazar |
| `rms_hata` | `a`, `b`, `n` | `double` | İki dizinin RMS farkı |
| `sqnr_db` | `orijinal`, `kuantize`, `n` | `double` | Sinyal kuantizasyon gürültüsü oranı |

Tam imzalar [`include/adc.hpp`](include/adc.hpp) dosyasında.

## Tasarım Kuralları

1. **Geçerli aralık:** `bit` 1 ile 16 arasında olmalı ve `vref > 0` olmalı. Değilse fonksiyonlar 0 dönmeli. 16 bitten büyük bir kod zaten `uint16_t` tipine sığmaz.
2. **Sıkıştırma (clamping):** Negatif voltajın kodu 0, $V_{ref}$ ve üstündeki voltajın kodu $2^N - 1$ olmalı. Gerçek ADC'ler de böyle davranır, taşma olmaz.
3. **Dizi aktarımı:** Diziler fonksiyonlara ham pointer ve boyut olarak gelir. Okunan dizilerin `const` olduğuna dikkat edin. `nullptr` ve `n == 0` durumlarında program çökmemeli.
4. **Tip dönüşümleri:** Tam sayı ve ondalıklı sayı arasındaki her geçişi `static_cast` ile açıkça yazın. `-Wpedantic` bayrağı bu konuda epey titiz.
5. **Dinamik bellek yok:** `new`, `malloc` ve `std::vector` kullanmayın. Bütün işlemler size verilen tamponlar üzerinde yapılmalı.

## Teslim Sınırları

| Sınır | Değer |
|---|---|
| Değiştirilecek dosya | Sadece `src/adc.cpp` |
| İzin verilen başlıklar | `<cstdint>`, `<cstddef>`, `<cmath>` |
| Yasak olanlar | `new`, `malloc`, `std::vector` ve diğer STL kapları |
| π değeri | `std::acos(-1.0)` ya da kendi tanımladığınız sabit (`M_PI` her platformda yok) |
| Derleme bayrakları | `-std=c++17 -Wall -Wextra -Wpedantic -Werror` |

## Teslim Süreci ve İstenenler

Klasörü `teslimler/<kullanici_adiniz>/odev02_adc/` altına kopyalayıp orada çalışın ve Pull Request açın.

**Teslim etmeniz gerekenler:**

1. `src/adc.cpp` dosyasının doldurulmuş hâli.
2. PR açıklamasında `make test` çıktısının ekran görüntüsü. Çıktıdaki 8 ve 12 bitlik SQNR değerleri görünsün.
3. **Bonus:** Ayrı bir `bonus/` klasöründe kendi `main` fonksiyonunuzu yazın. 4, 8 ve 12 bit için orijinal ve kuantize sinyali bir CSV dosyasına yazdırın, Python ya da Excel ile çizdirip grafiği PR açıklamasına ekleyin. 4 bitte merdiven basamaklarını gözünüzle göreceksiniz.

> **İpucu:** Kodlarken `floor` kullanmak, çözerken basamak ortasını almak bir tesadüf değil. Çözerken `+ 0.5` eklemeyi unutursanız hatanız hep aynı yöne kayar ve SQNR testi yaklaşık 6 dB eksik çıkar. Neden 6 dB, bir düşünün.
