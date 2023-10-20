AC_PREREQ([2.69])
AC_INIT([goose], [0.1.0], [http://grsan.xyz])
AC_CONFIG_SRCDIR([src/kernel.c])

CC=${CC-$srcdir/toolchain/i686-goose/bin/i686-goose-gcc}
CCAS=${CCAS-$srcdir/toolchain/i686-goose/bin/i686-goose-gcc}

CFLAGS="-ffreestanding -nostdlib -gdwarf-4 -m32 -ggdb3 -Wall -Wextra -Werror -std=c17"
LDFLAGS="-nostdlib -lgcc"
ASFLAGS=""

PACKAGE_CFLAGS="-ffreestanding -O2 -Wall -Werror -std=c17"
PACKAGE_LDFLAGS="-nostdlib -static-libgcc -T $srcdir/src/linker.ld -ffreestanding -O2"
PACKAGE_ASFLAGS=""

PACKAGE_LDADD="$srcdir/src/display/font.o -lgcc"

AC_SUBST([PACKAGE_CFLAGS])
AC_SUBST([PACKAGE_LDFLAGS])
AC_SUBST([PACKAGE_ASFLAGS])
AC_SUBST([PACKAGE_LDADD])

AC_PROG_CC
AM_PROG_AS

AM_INIT_AUTOMAKE(foreign)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT