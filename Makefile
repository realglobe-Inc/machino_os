# Makefile

# プログラム名とオブジェクトファイル名

CXX=g++
CFLAGS=-Wall -O2 -std=c++11
OBJS=util.o ranged_values.o

# サフィックスルール適用対象の拡張子の定義
.SUFFIXES: .cpp .o

.PHONY: all

all: by_date recent

by_date: $(OBJS) by_date.o
	$(CXX) $(CFLAGS) -o $@ $^

recent: $(OBJS) recent.o
	$(CXX) $(CFLAGS) -o $@ $^

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

.PHONY: clean
clean:
	-rm by_date recent *.o
