// 10_bitwise.cpp
// Bit işlemleri: elektronikçinin en yakın dostu
//
// Derleme: g++ -std=c++17 -Wall -Wextra 10_bitwise.cpp -o bitwise

#include <iostream>
#include <bitset>
#include <cstdint>

void yazdir(const char* ad, uint8_t x)
{
    std::cout << ad << " = " << std::bitset<8>(x) << "  (" << +x << ")\n";
}

int main()
{
    uint8_t a = 0b11001010;
    uint8_t b = 0b10101010;

    yazdir("a      ", a);
    yazdir("b      ", b);
    yazdir("a & b  ", a & b);    // VE: ikisi de 1 ise 1
    yazdir("a | b  ", a | b);    // VEYA: biri 1 ise 1
    yazdir("a ^ b  ", a ^ b);    // XOR: farklıysa 1
    yazdir("~a     ", static_cast<uint8_t>(~a));   // DEĞİL
    yazdir("a << 2 ", static_cast<uint8_t>(a << 2));
    yazdir("a >> 3 ", a >> 3);

    std::cout << "\n=== Register gibi kullanalim ===\n";
    uint8_t led_port = 0;

    led_port |=  (1u << 3);                  // 3. biti 1 yap (SET)
    yazdir("set 3  ", led_port);

    led_port |=  (1u << 5);
    yazdir("set 5  ", led_port);

    led_port &= static_cast<uint8_t>(~(1u << 3));   // 3. biti 0 yap (CLEAR)
    yazdir("clr 3  ", led_port);

    led_port ^=  (1u << 0);                  // 0. biti tersle (TOGGLE)
    yazdir("tgl 0  ", led_port);

    bool bes_acik_mi = (led_port >> 5) & 1u; // 5. biti oku (READ)
    std::cout << "5. bit acik mi? " << bes_acik_mi << "\n";

    // Maske ile bir alan okumak: 0b1101'0110 içinden 2..5 arası 4 biti al
    uint8_t veri = 0b11010110;
    uint8_t alan = (veri >> 2) & 0b1111;
    yazdir("alan   ", alan);

    return 0;
}
