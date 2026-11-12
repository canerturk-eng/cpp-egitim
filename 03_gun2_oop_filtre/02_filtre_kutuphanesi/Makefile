# Kullanım:  make test    (kütüphaneyi ve testleri derle, testleri çalıştır)
#            make ornek   (örnek programı derle ve çalıştır, filtre_cikti.csv üretir)
#            make temiz   (derleme çıktılarını sil)
#
# make yoksa (Windows'ta sık olur) aynı işi tek satırla yapabilirsiniz:
#   g++ -std=c++17 -Wall -Wextra -I include -I test src/*.cpp test/test_filtre.cpp -o test_prog
#   g++ -std=c++17 -Wall -Wextra -I include src/*.cpp ornek/main.cpp -o ornek_prog

CXX      ?= g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Werror -O2 -I include -I test

KUTUPHANE := $(wildcard src/*.cpp)
BASLIKLAR := $(wildcard include/*.hpp)

test: build/test_prog
	./build/test_prog

ornek: build/ornek_prog
	./build/ornek_prog

build/test_prog: $(KUTUPHANE) test/test_filtre.cpp $(BASLIKLAR) test/mini_test.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(KUTUPHANE) test/test_filtre.cpp -o $@ -lm

build/ornek_prog: $(KUTUPHANE) ornek/main.cpp $(BASLIKLAR)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(KUTUPHANE) ornek/main.cpp -o $@ -lm

temiz:
	rm -rf build filtre_cikti.csv

.PHONY: test ornek temiz
