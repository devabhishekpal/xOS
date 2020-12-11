#!/bin/bash
export PATH=$PATH:$HOME/Desktop/xOS/cross-compiler/bin
rm -r build
rm xos.iso
rm ./iso/boot/xos.bin
mkdir build
cd src
make
cd ..
cp ./build/xos.bin ./iso/boot
grub-mkrescue -o xos.iso ./iso
qemu-system-x86_64 xos.iso