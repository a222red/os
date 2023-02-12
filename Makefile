CXX := clang++
override CXXFLAGS := -m32 -std=c++20 -Wall -Werror=return-type -O2 \
	-nostdlib -fno-builtin -nodefaultlibs \
	-static -fno-pic -fno-pie -ffreestanding \
	-fno-exceptions -fno-rtti \
	-fno-stack-protector -fno-asynchronous-unwind-tables

ASMS := $(wildcard src/*.asm)
ASM_OBJS := $(subst src/, build/, $(patsubst %.asm, %.o, $(wildcard src/*.asm)))

.PHONY: all clean

all: os-image.bin

clean:
	rm $(wildcard build/*.o)
	rm $(wildcard bin/*.bin)

os-image.bin: bin/kernel.bin bin/boot.bin
	cat bin/boot.bin bin/kernel.bin > os-image.bin

bin/kernel.bin: $(ASM_OBJS) build/kernel.o
	ld -m elf_i386 -o bin/kernel.bin -Ttext 0xfffc \
		build/kernel-entry.o build/kernel.o \
    build/vga.o build/mem.o build/interrupts.o \
		--oformat binary

build/kernel.o: src/kernel.cpp
	$(CXX) $(CXXFLAGS) -c src/kernel.cpp -o build/kernel.o

build/%.o: src/%.asm
	nasm $< -f elf -o $@

bin/boot.bin:
	(( kernel_sectors=(`stat bin/kernel.bin -c "%s"` + 511) / 512 ));\
	cd boot/src;\
	nasm boot.asm -D KERNEL_SECTOR_NUM=$$kernel_sectors -f bin -o ../../bin/boot.bin
