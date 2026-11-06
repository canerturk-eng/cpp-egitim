# Kurulum Rehberi

Derse gelmeden evvel bu sayfayı bitirmeye çalışın. Derste kurulumla uğraşan herkes ilk yarım saati kaçırıyor, malum. Daha önce hiç terminal açmadıysanız da sorun yok, en baştan alıyoruz.

Kuracağımız şeyler iki tane:

* **Derleyici:** Yazdığımız C++ kodunu makinenin anlayacağı dile çeviren program. Biz `g++` (GCC) ya da macOS tarafında `clang++` kullanacağız.
* **Editör:** Kodu yazdığımız yer. Tavsiyemiz **VS Code** ve onun **C/C++** eklentisi.

## Terminal Nedir?

Bilgisayara normalde fareyle tıklayarak iş yaptırırız. **Terminal** ise aynı işleri yazarak yaptırdığımız penceredir. Bir komut yazarsınız, Enter'a basarsınız, bilgisayar cevabını altına yazar. Derleyicimizin tıklanacak bir penceresi yok, onu terminalden çalıştıracağız. İlk başta tuhaf gelir, lakin birkaç komuttan sonra alışırsınız.

### Nasıl açılır?

| Sistem | Yol |
|---|---|
| Windows | Başlat menüsüne `PowerShell` yazıp **Windows PowerShell** uygulamasını açın |
| macOS | `Cmd + Boşluk` tuşlarına basın, `Terminal` yazıp Enter'a basın |
| Linux | Ekseriya `Ctrl + Alt + T` kâfidir, olmazsa uygulamalarda `Terminal` diye aratın |
| VS Code (hepsinde) | Üst menüden **Terminal > New Terminal** |

Açılan pencerede bir satır ve yanıp sönen bir imleç görürsünüz. Windows'ta şuna benzer:

```
PS C:\Users\ali>
```

Bu satıra **komut istemi** (prompt) denir ve o an hangi klasörde olduğunuzu gösterir. Yukarıdaki örnekte `C:\Users\ali` klasöründeyiz. Komutları bunun yanına yazıp Enter'a basıyoruz. Bu sayfadaki komutları yazarken baştaki `PS C:\Users\ali>` kısmını yazmıyorsunuz, o zaten ekranda hazır duruyor.

### Klasörler arasında gezinmek

Terminal her an bir klasörün içindedir. Çalıştırdığınız komutlar da o klasörde çalışır. Bu yüzden önce doğru klasöre gitmeyi öğrenmeliyiz.

| Ne yapar? | Windows PowerShell | macOS ve Linux |
|---|---|---|
| Hangi klasördeyim? | `pwd` | `pwd` |
| Bu klasörde neler var? | `dir` ya da `ls` | `ls` |
| Bir klasöre gir | `cd klasor_adi` | `cd klasor_adi` |
| Bir üst klasöre çık | `cd ..` | `cd ..` |
| Yeni klasör oluştur | `mkdir klasor_adi` | `mkdir klasor_adi` |
| Ekranı temizle | `cls` | `clear` |

Bir örnek üzerinden gidelim. Masaüstündeki bir klasöre girip içine bakıyoruz:

```powershell
pwd
cd Desktop
dir
cd itu_yonga_cpp_oop
dir
cd ..
```

Birkaç ipucu işinizi çok kolaylaştırır:

* **Tab tuşu:** Klasör adının ilk harflerini yazıp Tab'a basın, gerisini terminal tamamlar. Hem hızlıdır hem yazım hatasını önler.
* **Yukarı ok tuşu:** Daha önce yazdığınız komutları geri getirir. Aynı derleme komutunu yeniden yazmak zorunda kalmazsınız.
* **Birkaç klasör birden:** `cd 01_gun1/01_syntax` yazarak iki klasörü tek seferde geçebilirsiniz.
* **Boşluklu isimler:** Klasör adında boşluk varsa tırnak içine alın: `cd "Yeni Klasor"`. Kendi çalışma klasörlerinizde boşluk ve Türkçe karakter kullanmamanızı tavsiye ederiz, bazı araçlar bunlarda sorun çıkarıyor.
* **Takılan program:** Çalışan bir programı durdurmak için `Ctrl + C` tuşlarına basın.

> **Sıra sizde (2 dk):** Terminali açın, `pwd` ile nerede olduğunuza bakın. Masaüstüne geçip `deneme` adında bir klasör oluşturun, içine girin, sonra bir üst klasöre geri çıkın.

## Windows

Windows'ta en temiz yol MSYS2 üzerinden GCC kurmaktır.

1. [msys2.org](https://www.msys2.org) adresinden kurulum dosyasını indirip kurun. Varsayılan klasör `C:\msys64` olarak kalsın.
2. Kurulum bitince açılan **MSYS2 UCRT64** terminaline şunu yazın. Onay sorduğunda Enter'a basmanız kâfi:
   ```bash
   pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb make
   ```
3. Şimdi Windows'a bu programların nerede olduğunu söyleyeceğiz. Başlat menüsüne "ortam değişkenleri" yazıp **Sistem ortam değişkenlerini düzenleyin** seçeneğini açın. **Ortam Değişkenleri** düğmesine basın, üstteki listeden **Path** satırını seçip **Düzenle** deyin. **Yeni** ile şu iki satırı ayrı ayrı ekleyin:
   ```
   C:\msys64\ucrt64\bin
   C:\msys64\usr\bin
   ```
   Birincisinde `g++`, ikincisinde `make` var. Ödevler `make` ile test edildiği için ikisi de lazım.
4. Açık terminalleri kapatın, yeni bir PowerShell açıp kontrol edin:
   ```powershell
   g++ --version
   make --version
   ```
   Sürüm bilgisi görüyorsanız tamamdır. "tanınmıyor" diyorsa Path adımını tekrar kontrol edin, terminali kapatıp açmayı da unutmayın.

## macOS

Terminale tek satır yazmanız kâfi:

```bash
xcode-select --install
```

Açılan pencerede kurulumu onaylayın. macOS'ta `g++` yazdığınızda arka planda aslında `clang++` çalışır, bizim için hiçbir farkı yok.

## Linux (Ubuntu ve türevleri)

```bash
sudo apt update
sudo apt install build-essential gdb
```

`sudo` komutu yönetici izni ister. Şifrenizi yazarken ekranda hiçbir şey görünmez, bu normaldir, yazıp Enter'a basın.

## VS Code

1. [code.visualstudio.com](https://code.visualstudio.com) adresinden VS Code'u kurun.
2. Sol taraftaki Eklentiler (Extensions) kısmından Microsoft'un **C/C++** eklentisini kurun.
3. Repoyu VS Code ile açın: **File > Open Folder**.

## Repoyu Bilgisayarınıza Almak

Derste takip etmek için en kolay yol, repo sayfasındaki yeşil **Code** düğmesinden **Download ZIP** demek ve inen dosyayı Masaüstüne çıkarmak. Git biliyorsanız `git clone` da olur. Ödev tesliminde fork ve Pull Request gerekecek, onun ayrıntıları [02_odevler](../02_odevler/README.md) sayfasında.

## İlk Programımız

Terminalde `01_gun1/01_syntax` klasörüne gidin. Repoyu Masaüstüne çıkardıysanız şöyle:

```bash
cd Desktop/itu_yonga_cpp_oop/01_gun1/01_syntax
```

`ls` ya da `dir` ile içeride `01_merhaba.cpp` dosyasını gördüğünüzden emin olun, sonra derleyip çalıştırın:

```bash
g++ -std=c++17 -Wall -Wextra 01_merhaba.cpp -o merhaba
./merhaba
```

Windows PowerShell'de ikinci satır `.\merhaba.exe` olacak.

Birinci satır derleme, ikinci satır çalıştırmadır. Derleme başarılıysa hiçbir mesaj çıkmaz, sessizlik burada iyi haberdir. Klasöre tekrar bakarsanız yeni bir `merhaba` dosyası oluştuğunu görürsünüz.

Ekranda bir merhaba mesajı görüyorsanız kurulum bitti demektir.

### Bu bayraklar ne işe yarıyor?

| Bayrak | Anlamı |
|---|---|
| `-std=c++17` | C++17 standardını kullan. Eğitim boyunca hep bunu kullanacağız. |
| `-Wall -Wextra` | Bütün mantıklı uyarıları aç. Uyarı, derleyicinin "burada bir terslik olabilir" demesidir, ciddiye alın. |
| `-o merhaba` | Çıkan programın adı `merhaba` olsun. Yazmazsanız adı `a.out` ya da `a.exe` olur. |

### Sık karşılaşılan sorunlar

| Mesaj | Sebep | Çözüm |
|---|---|---|
| `g++` tanınmıyor ya da `command not found` | Derleyici kurulu değil ya da Path eksik | Kurulum adımlarını kontrol edin, terminali yeniden açın |
| `No such file or directory` | Yanlış klasördesiniz ya da dosya adı hatalı | `pwd` ve `dir` (ya da `ls`) ile nerede olduğunuza bakın |
| Program eski hâliyle çalışıyor | Kaydedip yeniden derlemediniz | Dosyayı kaydedin, derleme komutunu tekrar çalıştırın |

## Kurulum Olmadıysa

Derste vakit kaybetmeyin, tarayıcıdan devam edin:

* [Compiler Explorer (godbolt.org)](https://godbolt.org): Derleyicinin ürettiği assembly çıktısını da gösterir, bilhassa merak edenler için harika.
* [OnlineGDB](https://www.onlinegdb.com/online_c++_compiler): Programı çalıştırıp girdi verebildiğiniz basit bir ortam.

Ödevlerden önce ise kurulumu mutlaka tamamlayın, zira ödevler `make` ile test ediliyor.
