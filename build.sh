nasm src/kernel-entry.asm -f elf -o build/kernel-entry.o || exit $?
nasm src/vga.asm -f elf -o build/vga.o || exit $?
nasm src/mem.asm -f elf -o build/mem.o || exit $?
nasm src/interrupts.asm -f elf -o build/interrupts.o || exit $?

clang++ -m32 -std=c++20 -Wall -Werror=return-type -O2 \
    -nostdlib -fno-builtin -nodefaultlibs \
    -static -fno-pic -fno-pie -ffreestanding \
    -fno-exceptions -fno-rtti \
    -fno-stack-protector -fno-asynchronous-unwind-tables \
    -c src/kernel.cpp -o build/kernel.o || exit $?

ld -m elf_i386 -o bin/kernel.bin -Ttext 0xfffc \
    build/kernel-entry.o build/kernel.o \
    build/vga.o build/mem.o build/interrupts.o \
    --oformat binary || exit $?

kernel_size=`stat bin/kernel.bin -c "%s"`
(( kernel_sectors=($kernel_size + 511) / 512 ))

cd src/boot
nasm boot.asm -D KERNEL_SECTOR_NUM=$kernel_sectors -f bin -o ../../bin/boot.bin || exit $?
cd ../..

# Combine the bootloader and kernel into an os image
cat bin/boot.bin bin/kernel.bin > os-image.bin || exit $?

# Pad out the os image to a full sector so the filesystem can
# be concatenated to it
boot_size=`stat os-image.bin -c "%s"`
(( boot_sectors=($boot_size + 511) / 512 ))
(( round_diff=$boot_sectors * 512 - $boot_size ))
dd if=/dev/zero bs=1 count=$round_diff >> os-image.bin

# Generate the filesystem
# The filenames have to be immediate
# because I'm too lazy to add path resolution to fsgen
cd fs
# clang++ -std=c++20 -Ofast -Wall -o fsgen src/fsgen.cpp
./fsgen foo bar
cd ..

# Combine the os image and filesystem for a complete 'installation'
cat os-image.bin fs/fs.bin > system.bin || exit $?

if [[ -z "${__OS_BUILD_FINI_CMD+x}" ]]; then
    echo "Done!"
else
    echo "Running extra build command..."
    $__OS_BUILD_FINI_CMD
    echo "Done!"
fi
