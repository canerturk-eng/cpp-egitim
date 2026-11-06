# İTÜ Yonga | C++ Ödev 04: Hareketli Ortalama ve Medyan Filtre

## Görevin Amacı

Sensörden okunan hiçbir sinyal temiz değildir. Isıl gürültü, elektromanyetik parazit, kötü bir lehim... Hepsi sinyale karışır. Bu ödevin amacı, sinyal işlemenin en temel iki gürültü giderme yöntemini yazmak ve aralarındaki farkı kendi gözünüzle görmektir:

* **Hareketli ortalama:** Beyaz gürültüyü yumuşatmakta usta, lakin ani sıçramalara karşı zayıf.
* **Medyan filtre:** Ani sıçramaları (spike, impulse noise) tamamen silebilir. Birinci günde sıraladığımız sensör örneğinin sinyalin tamamına uygulanmış hâli.

Bu iki fonksiyon, 12 Kasım'da yazacağımız filtre sınıflarının ilk taslağı olacak.

## Teknik İsterler ve Mimari

### Hareketli Ortalama (Nedensel)

Her çıkış örneği, o ana kadar gelen son $M$ giriş örneğinin ortalamasıdır:

$$
y[i] = \frac{1}{L} \sum_{j=i-L+1}^{i} x[j], \qquad L = \min(i + 1,\ M)
$$

"Nedensel" demek, filtrenin sadece şimdiki ve geçmiş örnekleri kullandığı, geleceğe bakmadığı anlamına gelir. Gerçek zamanlı bir sistemde gelecek örnek henüz elimizde olmadığı için başka çaremiz yok. İlk örneklerde pencere henüz dolmadığı için eldeki kadarının ortalaması alınır ($L$ bunu sağlıyor).

**Verimlilik meselesi:** Her çıkış için pencerenin tamamını baştan toplarsanız maliyet $O(n \cdot M)$ olur. Halbuki bir önceki toplamı bilirseniz:

$$
S[i] = S[i-1] + x[i] - x[i-M]
$$

Yani her adımda bir örnek ekleyip bir örnek çıkarmak yeter. Maliyet $O(n)$ olur.

### Medyan Filtre (Merkezi)

Her çıkış örneği, giriş örneğinin etrafındaki $M = 2k+1$ örneğin medyanıdır:

$$
y[i] = \text{medyan}\big(x[i-k],\ \dots,\ x[i],\ \dots,\ x[i+k]\big)
$$

Dizinin dışına taşan indeksler en yakın kenar örneğiyle doldurulur. Yani $i - k < 0$ ise $x[0]$, $i + k \geq n$ ise $x[n-1]$ kullanılır.

> **Örnek:** `giris = {5, 1, 9, 2, 8, 3}` ve `pencere = 3` için ilk pencere `[5 5 1]` olur (soldaki eksik örnek $x[0]$ ile dolduruldu), medyanı 5'tir. Bütün çıkış `{5, 5, 2, 8, 3, 3}` olur.

## Fonksiyon Haritası (Interface)

| Fonksiyon | Girdi | Çıktı | Açıklama |
|---|---|:---:|---|
| `insertion_sort` | `dizi`, `n` | | Diziyi yerinde küçükten büyüğe sıralar |
| `hareketli_ortalama` | `giris`, `cikis`, `n`, `pencere` | `bool` | Nedensel hareketli ortalama, geçersizse `false` |
| `medyan_filtre` | `giris`, `cikis`, `n`, `pencere` | `bool` | Merkezi medyan, geçersizse `false` |

Sabitler (`ORTALAMA_MAKS_PENCERE = 64`, `MEDYAN_MAKS_PENCERE = 31`) ve tam imzalar [`include/filtre.hpp`](include/filtre.hpp) dosyasında.

## Tasarım Kuralları

1. **Kendi sıralamanız:** Medyanı bulmak için birinci günde yazdığımız **insertion sort** algoritmasını kullanın. `std::sort`, `std::nth_element` ve benzeri hazır fonksiyonlar yasak.
2. **O(n) hareketli ortalama:** Kayan toplam yöntemini kullanın. Test, 2 milyon örnekli bir sinyali filtreliyor ve süresini ekrana yazıyor. Toplamı `double` tipinde tutmanızı tavsiye ederiz, uzun sinyallerde `float` hatası birikir.
3. **Dinamik bellek yok:** Medyan penceresi için `float tampon[MEDYAN_MAKS_PENCERE]` gibi sabit boyutlu yerel bir dizi kullanın. `new`, `malloc` ve STL kapları yasak.
4. **Giriş bozulmamalı:** `giris` dizisi `const` olarak geliyor. Sıralamayı girişin kendisi üzerinde değil, pencerenin bir kopyası üzerinde yapın.
5. **Aynı dizi yasak:** `medyan_filtre` için `giris == cikis` gelirse `false` dönün. Neden yasak olduğunu düşünün: $y[i]$ yazıldıktan sonra $y[i+1]$ hesaplanırken eski $x[i]$ lazım olacak.
6. **Geçersiz pencere:** Ortalama için 0 ya da 64 üstü, medyan için çift sayı, 0 ya da 31 üstü pencere gelirse çıkışa dokunmadan `false` dönün.

## Teslim Sınırları

| Sınır | Değer |
|---|---|
| Değiştirilecek dosya | Sadece `src/filtre.cpp` |
| İzin verilen başlıklar | `<cstddef>`, `<cmath>` |
| Yasak olanlar | `<algorithm>` içindeki her şey, `new`, `malloc`, STL kapları |
| Hareketli ortalamada iç içe döngü | Yasak (tek döngü, kayan toplam) |
| Derleme bayrakları | `-std=c++17 -Wall -Wextra -Wpedantic -Werror` |

## Teslim Süreci ve İstenenler

Klasörü `teslimler/<kullanici_adiniz>/odev04_filtre/` altına kopyalayıp orada çalışın ve Pull Request açın.

**Teslim etmeniz gerekenler:**

1. `src/filtre.cpp` dosyasının doldurulmuş hâli.
2. PR açıklamasında `make test` çıktısının ekran görüntüsü. Parazitli sensör senaryosundaki ortalama ve medyan değerleri ile 2 milyon örneklik performans süresi görünsün.
3. **Bonus:** Ödev 02'deki `sinus_uret` fonksiyonuyla bir sinüs üretin, üzerine rastgele gürültü ve birkaç büyük sıçrama ekleyin. İki filtreyi de uygulayıp sonucu grafikle PR açıklamasına ekleyin.

> **İpucu:** Medyan filtrede indeks hesabını `size_t` ile yaparsanız `i - k` ifadesi negatif olamaz ve başa sarar. İndeksi `long` gibi işaretli bir tipte hesaplayıp sonra sınırlara sıkıştırmak en temiz yol.
