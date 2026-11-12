// main.cpp
// Final projesinin vitrini: gürültülü bir sensör sinyali üretip bütün filtrelerden geçiriyoruz,
// hangisinin ne kadar iyi temizlediğini ölçüyoruz ve sonuçları CSV dosyasına yazıyoruz.
//
// Senaryo: 1 kHz ile örneklenen bir sensör, 5 Hz'lik yavaş bir sinyal ölçüyor.
// Üstüne iki tür bozulma biniyor:
//   1) Beyaz gürültü (her örnekte küçük rastgele sapma)
//   2) Dürtü gürültüsü (arada bir gelen kocaman sıçramalar)

#include <cstdio>
#include "filtre.hpp"
#include "sinyal.hpp"

constexpr size_t N  = 1000;        // 1 saniyelik veri
constexpr float  FS = 1000.0f;     // örnekleme frekansı (Hz)
constexpr float  F0 = 5.0f;        // asıl sinyalin frekansı (Hz)
constexpr size_t ATLA = 100;       // ilk 100 örnekte filtreler henüz oturmadı, ölçüme katmıyoruz

// Diziler büyük olduğu için stack yerine global (statik bellek) tutuyoruz.
float temiz[N];
float gurultulu[N];

int main()
{
    // ---- 1) Test sinyalini hazırla ----
    SinyalUreteci uretec(FS);
    uretec.sinus(temiz, N, F0, 1.0f);
    uretec.sinus(gurultulu, N, F0, 1.0f);
    uretec.gurultu_ekle(gurultulu, N, 0.15f);
    uretec.durtu_ekle(gurultulu, N, 0.01f, 3.0f);

    // ---- 2) Filtreleri oluştur ----
    HareketliOrtalama ortalama(21);
    MedyanFiltre medyan(15);

    constexpr size_t FIR_N = 61;
    float katsayilar[FIR_N];
    fir_alcak_geciren_tasarla(katsayilar, FIR_N, 15.0f, FS);
    FIRFiltre fir(katsayilar, FIR_N);

    IIRAlcakGeciren iir(IIRAlcakGeciren::alfa_hesapla(10.0f, FS));

    // Zincir: önce küçük bir medyan sıçramaları yok etsin, sonra ortalama beyaz gürültüyü yumuşatsın.
    // Ayrı nesneler kullanıyoruz, zira bir filtre nesnesi tek bir sinyal akışının geçmişini tutar.
    // Yukarıdaki 'ortalama'yı burada da kullansaydık iki akışın örnekleri birbirine karışırdı.
    MedyanFiltre zincir_medyan(5);
    HareketliOrtalama zincir_ortalama(21);
    FiltreZinciri zincir;
    zincir.ekle(&zincir_medyan);
    zincir.ekle(&zincir_ortalama);

    // ---- 3) Hepsini aynı dizide topla: çok biçimlilik burada ----
    Filtre* filtreler[] = {&ortalama, &medyan, &fir, &iir, &zincir};
    constexpr size_t F = sizeof(filtreler) / sizeof(filtreler[0]);

    static float cikislar[F][N];

    // ---- 4) Filtrele ve ölç ----
    std::printf("Sinyal: %.0f Hz sinus, fs = %.0f Hz, %zu ornek\n", F0, FS, N);
    std::printf("Gurultulu sinyalin RMS hatasi: %.4f\n\n", rms_fark(temiz, gurultulu, N, ATLA));

    std::printf("%-20s %8s %12s %18s\n", "Filtre", "Gecikme", "RMS hata", "RMS (gecikmesiz)");
    std::printf("%-20s %8s %12s %18s\n", "------", "-------", "--------", "----------------");

    for (size_t f = 0; f < F; f++) {
        filtreler[f]->isle_dizi(gurultulu, cikislar[f], N);

        size_t d = filtreler[f]->gecikme();
        std::printf("%-20s %8zu %12.4f %18.4f\n",
                    filtreler[f]->ad(),
                    d,
                    rms_fark(temiz, cikislar[f], N, ATLA),
                    rms_fark_gecikmeli(temiz, cikislar[f], N, d, ATLA));
    }

    // ---- 5) Grafik çizmek isteyenler için CSV ----
    const char* dosya_adi = "filtre_cikti.csv";
    std::FILE* dosya = std::fopen(dosya_adi, "w");
    if (dosya == nullptr) {
        std::printf("\n%s acilamadi\n", dosya_adi);
        return 1;
    }
    std::fprintf(dosya, "t,temiz,gurultulu");
    for (size_t f = 0; f < F; f++) std::fprintf(dosya, ",%s", filtreler[f]->ad());
    std::fprintf(dosya, "\n");

    for (size_t i = 0; i < N; i++) {
        std::fprintf(dosya, "%.4f,%.5f,%.5f", i / FS, temiz[i], gurultulu[i]);
        for (size_t f = 0; f < F; f++) std::fprintf(dosya, ",%.5f", cikislar[f][i]);
        std::fprintf(dosya, "\n");
    }
    std::fclose(dosya);
    std::printf("\nSonuclar %s dosyasina yazildi (Excel, MATLAB ya da Python ile cizebilirsiniz).\n",
                dosya_adi);

    return 0;
}
