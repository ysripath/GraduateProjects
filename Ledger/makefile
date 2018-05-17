all:
	g++ -std=c++11 main.cpp parser.cpp debug.cpp record.cpp util.cpp -w -o ledger -L$(CURDIR)/openssl/lib/ -lssl -lcrypto -I$(CURDIR)/openssl/include/
clean:
	rm -f ledger
