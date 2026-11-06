// mini_test.hpp
// Ödevler için küçük bir test aracı. Bu dosyayı DEĞİŞTİRMEYİN.
#pragma once
#include <cstdio>
#include <cmath>

inline int g_gecen = 0;
inline int g_kalan = 0;

#define KONTROL(kosul)                                                        \
    do {                                                                      \
        if (kosul) { ++g_gecen; }                                             \
        else {                                                                \
            ++g_kalan;                                                        \
            std::printf("  [HATA] satir %d:  %s\n", __LINE__, #kosul);        \
        }                                                                     \
    } while (0)

#define YAKIN(a, b, tol) KONTROL(std::fabs((double)(a) - (double)(b)) <= (tol))

#define BOLUM(ad) std::printf("\n== %s ==\n", ad)

inline int sonuc_bildir()
{
    std::printf("\n------------------------------\n");
    std::printf("Gecen: %d   Kalan: %d\n", g_gecen, g_kalan);
    if (g_kalan == 0) std::printf("Tum testler gecti, eline saglik!\n");
    return g_kalan == 0 ? 0 : 1;
}
