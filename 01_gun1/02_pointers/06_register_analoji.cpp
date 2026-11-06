// 06_register_analoji.cpp
// Mikrodenetleyicide donanıma nasıl yazılır? (PC'de simüle ediyoruz)
//
// Gerçek bir STM32'de kod şöyle görünür:
//     volatile uint32_t* GPIOA_ODR = reinterpret_cast<volatile uint32_t*>(0x40020014);
//     *GPIOA_ODR |= (1u << 5);     // PA5 pinindeki LED yandı!
//
// Bilgisayarımızda o adreste bir donanım olmadığı için oraya yazarsak program çöker.
// O yüzden "sahte" bir register oluşturup pointer ile ona yazıyoruz.

#include <iostream>
#include <bitset>
#include <cstdint>

int main()
{
    uint32_t sahte_gpio = 0;                         // donanım register'ı yerine geçiyor
    volatile uint32_t* GPIOA_ODR = &sahte_gpio;      // volatile: "bu değer kendiliğinden değişebilir, optimize etme"

    *GPIOA_ODR |= (1u << 5);                         // PA5 = 1
    std::cout << "LED yandi   : " << std::bitset<16>(*GPIOA_ODR) << "\n";

    *GPIOA_ODR &= ~(1u << 5);                        // PA5 = 0
    std::cout << "LED sondu   : " << std::bitset<16>(*GPIOA_ODR) << "\n";

    return 0;
}
