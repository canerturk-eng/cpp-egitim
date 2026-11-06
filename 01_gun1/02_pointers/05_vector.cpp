// 05_vector.cpp
// new/delete ile uğraşmak istemiyorsak: std::vector

#include <iostream>
#include <vector>

int main()
{
    std::vector<double> olcumler;          // boş başlar

    olcumler.push_back(1.2);
    olcumler.push_back(3.4);
    olcumler.push_back(2.8);

    std::cout << "Eleman sayisi: " << olcumler.size() << "\n";

    for (double x : olcumler) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Ham pointer gereken eski bir fonksiyona vermek için:
    double* ham = olcumler.data();
    std::cout << "Ilk eleman (data uzerinden): " << *ham << "\n";

    // Silmek yok! vector kapsam dışına çıkınca belleğini kendi temizler.
    return 0;
}
