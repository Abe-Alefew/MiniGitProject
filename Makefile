CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

OPENSSL_INC ?= C:/Users/$(USERNAME)/scoop/apps/openssl/current/include
OPENSSL_LIB ?= C:/Users/$(USERNAME)/scoop/apps/openssl/current/lib

ifeq ("$(wildcard $(OPENSSL_INC)/openssl/sha.h)","")
$(error OpenSSL headers not found at $(OPENSSL_INC). Please install OpenSSL or set OPENSSL_INC and OPENSSL_LIB manually.)
endif

ifeq ("$(wildcard $(OPENSSL_LIB)/libcrypto.lib)","")
ifeq ("$(wildcard $(OPENSSL_LIB)/libcrypto.a)","")
$(error OpenSSL libraries not found at $(OPENSSL_LIB). Please install OpenSSL or set OPENSSL_LIB manually.)
endif
endif

CXXFLAGS += -I"$(OPENSSL_INC)"
LDFLAGS = -L"$(OPENSSL_LIB)" -lssl -lcrypto

SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
OUT = minigit

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OUT)

test: $(OUT)
	@echo "Running sample tests..."
	@$(OUT) init
	@echo Hello > testfile.txt
	@$(OUT) add testfile.txt
	@echo "Tests completed."
