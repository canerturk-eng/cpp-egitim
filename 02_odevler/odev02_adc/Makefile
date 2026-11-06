# Bu dosyayı değiştirmenize gerek yok.
# Kullanım:  make test   (derle ve testleri çalıştır)
#            make temiz  (derleme çıktılarını sil)

CXX      ?= g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Werror -O2 -I include -I test

KAYNAK := $(wildcard src/*.cpp) $(wildcard test/*.cpp)

test: build/test_prog
	./build/test_prog

build/test_prog: $(KAYNAK) $(wildcard include/*.hpp)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(KAYNAK) -o $@ -lm

temiz:
	rm -rf build

.PHONY: test temiz
