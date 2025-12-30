mkdir -p ./isodir/boot/grub/
mkdir ./isodir/boot/modules
cp ./grub.cfg ./isodir/boot/grub

cp ../build/kernel/goose ./isodir/boot

cp ../modules/test_module/build/test_module ./isodir/boot/modules

grub-mkrescue -o goose.iso ./isodir

rm -r ./isodir
