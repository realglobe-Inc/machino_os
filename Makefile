# Makefile

# プログラム名とオブジェクトファイル名

CXX=g++
CFLAGS=-Wall -O2 -std=c++11
OBJS=./src/util.o ./src/ranged_values.o

# サフィックスルール適用対象の拡張子の定義
.SUFFIXES: .cpp .o

# ビルドターゲットの判定
target=$(shell uname -s | tr [:upper:] [:lower:])/$(shell uname -m | tr [:upper:] [:lower:])

.PHONY: all

all: by_date recent

by_date: $(OBJS) ./src/by_date.o
	mkdir -p ./build/${target}
	$(CXX) $(CFLAGS) -o ./build/${target}/$@ $^

recent: $(OBJS) ./src/recent.o
	mkdir -p ./build/${target}
	$(CXX) $(CFLAGS) -o ./build/${target}/$@ $^

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o ./$@

.PHONY: clean
clean:
	-rm ./build/${target}/by_date ./build/${target}/recent ./src/*.o

.PHONY: dummy
dummy:
	echo ${target}
