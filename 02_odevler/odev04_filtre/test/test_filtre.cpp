// test_filtre.cpp
// Bu dosyayı DEĞİŞTİRMEYİN.
#include "filtre.hpp"
#include "mini_test.hpp"
#include <chrono>

static bool diziler_yakin(const float* a, const float* b, size_t n, float tol)
{
    for (size_t i = 0; i < n; i++) {
        if (std::fabs(a[i] - b[i]) > tol) {
            std::printf("  indeks %zu: beklenen %g, gelen %g\n", i, (double)b[i], (double)a[i]);
            return false;
        }
    }
    return true;
}

int main()
{
    BOLUM("insertion_sort");
    {
        float d[] = {3.5f, -1.0f, 7.0f, 0.0f, 3.5f, 2.0f};
        float b[] = {-1.0f, 0.0f, 2.0f, 3.5f, 3.5f, 7.0f};
        insertion_sort(d, 6);
        KONTROL(diziler_yakin(d, b, 6, 0.0f));
        float tek[] = {4.0f};
        insertion_sort(tek, 1);
        KONTROL(tek[0] == 4.0f);
        insertion_sort(nullptr, 5);   // çökmemeli
    }

    BOLUM("hareketli_ortalama (nedensel)");
    {
        float g[] = {1, 2, 3, 4, 5, 6};
        float c[6];
        float b[] = {1.0f, 1.5f, 2.0f, 3.0f, 4.0f, 5.0f};   // pencere 3
        KONTROL(hareketli_ortalama(g, c, 6, 3));
        KONTROL(diziler_yakin(c, b, 6, 1e-5f));

        KONTROL(hareketli_ortalama(g, c, 6, 1));            // pencere 1: çıkış = giriş
        KONTROL(diziler_yakin(c, g, 6, 1e-6f));

        float b10[] = {1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f}; // pencere > n
        KONTROL(hareketli_ortalama(g, c, 6, 10));
        KONTROL(diziler_yakin(c, b10, 6, 1e-5f));

        KONTROL(hareketli_ortalama(g, c, 6, 0) == false);
        KONTROL(hareketli_ortalama(g, c, 6, ORTALAMA_MAKS_PENCERE + 1) == false);
        KONTROL(hareketli_ortalama(nullptr, c, 6, 3) == false);
        KONTROL(hareketli_ortalama(g, nullptr, 6, 3) == false);
    }

    BOLUM("medyan_filtre (merkezi, kenar tekrari)");
    {
        float g[] = {5, 1, 9, 2, 8, 3};
        float c[6];
        // pencere 3 icin pencereler: [5 5 1] [5 1 9] [1 9 2] [9 2 8] [2 8 3] [8 3 3]
        float b[] = {5, 5, 2, 8, 3, 3};
        KONTROL(medyan_filtre(g, c, 6, 3));
        KONTROL(diziler_yakin(c, b, 6, 0.0f));
        KONTROL(g[0] == 5 && g[2] == 9);                    // giriş bozulmamalı

        KONTROL(medyan_filtre(g, c, 6, 1));
        KONTROL(diziler_yakin(c, g, 6, 0.0f));

        KONTROL(medyan_filtre(g, c, 6, 4) == false);        // çift pencere
        KONTROL(medyan_filtre(g, c, 6, 0) == false);
        KONTROL(medyan_filtre(g, c, 6, 33) == false);
        KONTROL(medyan_filtre(g, g, 6, 3) == false);        // aynı dizi
        KONTROL(medyan_filtre(nullptr, c, 6, 3) == false);
    }

    BOLUM("Senaryo: parazitli sicaklik sensoru");
    {
        float sensor[] = {25, 26, 24, 25, 999, 25, 26, 24, 25, -400, 25, 26};
        float med[12], ort[12];
        KONTROL(medyan_filtre(sensor, med, 12, 5));
        KONTROL(hareketli_ortalama(sensor, ort, 12, 5));
        bool medyan_temiz = true;
        for (float v : med) if (v < 24 || v > 26) medyan_temiz = false;
        KONTROL(medyan_temiz);                              // medyan sıçramaları tamamen silmeli
        KONTROL(ort[4] > 100.0f);                           // ortalama ise sıçramadan etkilenir
        std::printf("  i=4  ham=%g  ortalama=%g  medyan=%g\n",
                    (double)sensor[4], (double)ort[4], (double)med[4]);
    }

    BOLUM("Performans: hareketli ortalama O(n) olmali");
    {
        constexpr size_t N = 2000000;
        static float g[N], c[N];
        for (size_t i = 0; i < N; i++) g[i] = (float)(i % 100);
        auto t0 = std::chrono::steady_clock::now();
        KONTROL(hareketli_ortalama(g, c, N, 64));
        auto t1 = std::chrono::steady_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::printf("  2 milyon ornek, pencere 64: %.1f ms\n", ms);
        YAKIN(c[N - 1], 67.5, 1e-2);                        // son 64 örnek: 36..99
    }

    return sonuc_bildir();
}
