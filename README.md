# İTÜ Yonga | Temel C++ ve OOP Eğitimi

İTÜ Elektronik ve Haberleşme Mühendisliği Kulübü için hazırlanmış iki günlük bir eğitim reposudur. Evvela dili ve belleği tanıyacağız, ardından nesne yönelimli düşünmeye geçip bir dijital filtre tasarlayacağız.

Bu repoyu bir öğrenci olarak öğrencilere yazdık. Yani hocadan ders dinler gibi değil, bir üst dönemden arkadaşınızın notlarını okur gibi okuyun. Bir yeri anlamazsanız sorun, zira bizim de ilk gördüğümüzde anlamadığımız yerler tam olarak oralar.

## Takvim

| Tarih | Oturum | İçerik |
|:---:|:---:|---|
| 5 Kasım | Gün 1 | Kurulum, C++ sözdizimi, bellek ve pointerlar, sıralama algoritmaları |
| 5 ile 11 Kasım arası | Ödev haftası | Elektronik ve haberleşme odaklı 5 uygulamalı ödev |
| 12 Kasım | Gün 2 | Sınıflar, kapsülleme, kalıtım, çok biçimlilik ve final projesi olarak dijital filtre |

## Repo Yapısı

```
itu_yonga_cpp_oop/
├── 00_kurulum/              Windows, macOS ve Linux kurulum rehberi
├── 01_gun1/                 Birinci günün ders notu ve canlı demo kodları
│   ├── 01_syntax/
│   ├── 02_pointers/
│   └── 03_sorting/
├── 02_odevler/              Haftalık ödevler (her birinin kendi README dosyası var)
│   ├── odev01_register/
│   ├── odev02_adc/
│   ├── odev03_ring_buffer/
│   ├── odev04_filtre/
│   └── odev05_hamming/
├── 03_gun2_oop_filtre/      İkinci gün ve final projesi
└── teslimler/               Ödev teslimleriniz buraya gelecek
```

## Nereden Başlamalıyım?

1. Önce [00_kurulum](00_kurulum/README.md) rehberini takip edip derleyicinizi kurun.
2. Ardından [01_gun1](01_gun1/README.md) ders notunu okuyun. Derste anlattığımız her şey orada yazılı.
3. Ders bitince [02_odevler](02_odevler/README.md) altındaki ödevlere geçin.

## Teslim Usulü (kısaca)

Repoyu kendi hesabınıza **fork** edin, ödevi `teslimler/<github_kullanici_adiniz>/` altına kopyalayıp orada çözün ve ana repoya **Pull Request** açın. Ayrıntılar [02_odevler](02_odevler/README.md) sayfasında.

**Son teslim: 11 Kasım 2026, saat 23.59**

## Ufak Bir Rica

Kodu kopyalayıp yapıştırmak sizi yalnızca o ödevden kurtarır, lakin bir sonraki ödevde yine aynı yerde takılırsınız. Yapay zekâ araçlarını kullanabilirsiniz, fakat teslim ettiğiniz her satırı açıklayabilecek durumda olun. 12 Kasım'da birkaç kişiye "bu satır ne yapıyor" diye soracağız.
