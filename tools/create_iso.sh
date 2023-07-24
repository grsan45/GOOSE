mkdir -p ./isodir/boot/grub/
cp ./grub.cfg ./isodir/boot/grub

cp ../build/goose ./isodir/boot

grub-mkrescue -o goose.iso ./isodir

rm -r ./isodir
