# Gün 2 | Nesne Yönelimli Programlama ve Dijital Filtre Projesi

**Tarih:** 12 Kasım 2026

Bu klasörün içeriği ödev haftasının ardından eklenecek. Şimdilik ne beklemeniz gerektiğini kabaca yazalım.

## Gündem

* **Sınıflar ve nesneler:** `struct` ile `class` arasındaki fark, yapıcı ve yıkıcı fonksiyonlar
* **Kapsülleme:** Ring buffer'ınızın içini dışarıdan gizlemek
* **RAII:** Belleği yıkıcı fonksiyona emanet etmek, `delete` derdinden kurtulmak
* **Kalıtım ve çok biçimlilik:** Bütün filtreler için ortak bir `Filtre` arayüzü ve sanal fonksiyonlar
* **Final projesi:** FIR ve IIR filtrelerini de içeren küçük bir dijital filtre kütüphanesi

## Bu Haftanın Ödevleriyle Bağlantısı

| Ödev | 12 Kasım'da neye dönüşecek? |
|---|---|
| 02 ADC | Filtreyi test edeceğimiz sinyal üreteci |
| 03 Ring Buffer | `RingBuffer` sınıfı, filtrelerin geçmiş örnek deposu |
| 04 Filtre | `HareketliOrtalama` ve `MedyanFiltre` sınıfları |

Hâsılı, bu hafta yazdığınız fonksiyonları çöpe atmayacağız, onları sınıfların içine taşıyacağız.
