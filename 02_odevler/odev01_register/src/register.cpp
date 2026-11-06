// register.cpp
// Fonksiyonları burada dolduracaksınız. (void) satırlarını kendi kodunuzu yazınca silin.
#include "register.hpp"

void bit_set(volatile uint32_t* reg, unsigned n)
{
    // TODO
    (void)reg; (void)n;
}

void bit_clear(volatile uint32_t* reg, unsigned n)
{
    // TODO
    (void)reg; (void)n;
}

void bit_toggle(volatile uint32_t* reg, unsigned n)
{
    // TODO
    (void)reg; (void)n;
}

bool bit_read(const volatile uint32_t* reg, unsigned n)
{
    // TODO
    (void)reg; (void)n;
    return false;
}

bool alan_yaz(volatile uint32_t* reg, unsigned konum, unsigned genislik, uint32_t deger)
{
    // TODO
    (void)reg; (void)konum; (void)genislik; (void)deger;
    return false;
}

uint32_t alan_oku(const volatile uint32_t* reg, unsigned konum, unsigned genislik)
{
    // TODO
    (void)reg; (void)konum; (void)genislik;
    return 0;
}

unsigned bit_say(uint32_t deger)
{
    // TODO
    (void)deger;
    return 0;
}
