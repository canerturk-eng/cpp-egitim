# Ödev Haftası (5 ile 11 Kasım arası)

Birinci günde öğrendiklerimizi elektronik ve haberleşme problemlerine uygulama vakti. Beş ödevin her biri gerçek bir mühendislik parçasının küçük bir modeli ve hepsi 12 Kasım'daki dijital filtre projesine bir şekilde bağlanıyor.

| No | Ödev | Konu | Zorluk | Final projesine katkısı |
|:---:|---|---|:---:|---|
| 01 | [Register Simülatörü](odev01_register/README.md) | Bit işlemleri, pointer | Kolay | Donanım seviyesinde düşünmek |
| 02 | [ADC ve Kuantizasyon](odev02_adc/README.md) | Tipler, diziler, sayısal hesap | Orta | Filtreye girecek test sinyalleri |
| 03 | [Ring Buffer](odev03_ring_buffer/README.md) | Diziler, modüler aritmetik | Orta | Filtrenin örnek geçmişi |
| 04 | [Hareketli Ortalama ve Medyan Filtre](odev04_filtre/README.md) | Sıralama, pencereleme | Zor | Filtrelerin ilk hâli |
| 05 | [Hamming(7,4) Kodu](odev05_hamming/README.md) | Bit işlemleri, haberleşme | Orta | Bonus, haberleşme tarafı |

**Son teslim: 11 Kasım 2026, saat 23.59**

Hepsini yapmak zorunda değilsiniz, lakin 12 Kasım'a **en az 01, 03 ve 04 numaralı ödevleri** bitirmiş olarak gelirseniz final projesinde çok rahat edersiniz. Tavsiye ettiğimiz sıra numara sırası.

## Her Ödevin Klasör Yapısı

```
odev0X_ad/
├── README.md          Görevin tanımı, isterler ve kurallar
├── Makefile           Derleme ve test (dokunmayın)
├── include/*.hpp      Arayüz dosyası (dokunmayın)
├── src/*.cpp          SİZİN DOLDURACAĞINIZ TEK DOSYA
└── test/              Otomatik testler (dokunmayın)
```

Her ödevde yapmanız gereken iş aynı: `src` klasöründeki `.cpp` dosyasındaki `TODO` yazan fonksiyonları doldurmak. Fonksiyonların ne yapması gerektiği hem README dosyasında hem de `include` altındaki başlık dosyasının yorumlarında yazıyor.

## ▶Test Etmek

Ödev klasörünün içindeyken:

```bash
make test
```

Windows'ta MSYS2 kurduysanız `make` çalışır. Çalışmazsa aynı işi şu komutla yapabilirsiniz (örnek ödev 01 için):

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -O2 -I include -I test src/*.cpp test/*.cpp -o test_prog
./test_prog
```

İskelet kodu ilk çalıştırdığınızda testlerin çoğu kalacak, bu normal. Siz fonksiyonları doldurdukça "Kalan" sayısı düşecek. Hedef sıfır.

## Teslim Usulü

1. Ana repoyu kendi GitHub hesabınıza **fork** edin.
2. Ödev klasörünü `teslimler/<github_kullanici_adiniz>/` altına **kopyalayın**. Mesela ödev 01 için:
   ```
   teslimler/ahmetyilmaz/odev01_register/
   ```
3. Sadece bu kopya üzerinde çalışın. `02_odevler` altındaki asıl dosyalara dokunmayın, yoksa Pull Request'iniz başkalarınınkiyle çakışır.
4. İşiniz bitince ana repoya bir **Pull Request** açın.
   * **Başlık:** `[Odev01] Ad Soyad` (birden fazla ödevi tek PR'da gönderecekseniz `[Odev01,03,04] Ad Soyad`)
   * **Açıklama:** `make test` çıktısının ekran görüntüsü ve ödevin README dosyasında istenen diğer şeyler.

## Herkes İçin Geçerli Sınırlar

Her ödevin kendine has kuralları kendi README dosyasında. Bunlar ise hepsinde geçerli:

| # | Kural | Neden? |
|:---:|---|---|
| 1 | Sadece `src` altındaki dosyayı değiştirin. `include`, `test` ve `Makefile` aynı kalmalı. | Herkesin aynı arayüze göre değerlendirilmesi için |
| 2 | Kod `-Wall -Wextra -Wpedantic -Werror` ile **uyarısız** derlenmeli. | Uyarı, bir hatanın habercisidir |
| 3 | `make test` bütün testleri geçmeli. Kısmi teslim kabul, ama kalan testleri PR açıklamasında belirtin. | |
| 4 | `src` dosyanıza `main` fonksiyonu yazmayın. Kendi denemeleriniz için ayrı bir klasör açın. | Test dosyasının kendi `main` fonksiyonu var |
| 5 | Yardımcı fonksiyon yazabilirsiniz, ama `static` olarak `src` dosyasının içinde kalsın. | Arayüz değişmesin |
| 6 | Testleri "kandırmak" için özel durum yazmak (test girdisini tanıyıp cevabı ezbere döndürmek) yasak. | Bunu fark ederiz, inanın. |
| 7 | PR yalnızca `teslimler/<kullanici_adiniz>/` altındaki dosyaları içermeli. | |

## Değerlendirme

| Ölçüt | Ağırlık |
|---|:---:|
| Otomatik testlerin geçmesi | %60 |
| Ödeve özel kurallara uyum | %20 |
| Okunabilirlik (anlamlı isimler, gerektiği kadar yorum) | %10 |
| PR açıklaması ve istenen ekler | %10 |

Bu bir yarışma değil, notlar sadece size geri bildirim olsun diye. Takıldığınız yerde kulübün iletişim grubuna yazın, birbirinize de yardım edin. Yalnız kod paylaşmak yerine fikir paylaşın, zira asıl öğrenme o fikri koda dökerken oluyor.
