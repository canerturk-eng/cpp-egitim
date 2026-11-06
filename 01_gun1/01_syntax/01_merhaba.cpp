// 01_merhaba.cpp
// Programın iskeleti: ifade, noktalı virgül, süslü parantez bloğu, girinti ve yorum
//
// Derleme:    g++ -std=c++17 -Wall -Wextra 01_merhaba.cpp -o merhaba
// Çalıştırma: ./merhaba   (Windows PowerShell için: .\merhaba.exe)

/*
   Bu da çok satırlı yorumdur.
   Derleyici bu iki işaret arasındaki her şeyi görmezden gelir.
*/

#include <iostream>   // ekrana yazı yazmak için gereken kütüphane

int main()            // her program buradan başlar
{                     // main bloğu burada açılıyor
    std::cout << "Merhaba YONGA!" << std::endl;      // birinci ifade
    std::cout << "C++ ogrenmeye basladik.\n";        // ikinci ifade

    // std::cout << "Bu satir calismaz, zira basinda yorum isareti var.\n";

    return 0;         // 0 döndürmek işletim sistemine "her şey yolunda" demektir
}                     // main bloğu burada kapanıyor
