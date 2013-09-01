# This is a shell script that calls functions and scripts from
# tml@iki.fi's personal work environment. It is not expected to be
# usable unmodified by others, and is included only for reference.

MOD=glib
VER=2.20.3
REV=1
ARCH=win32

THIS=${MOD}_${VER}-${REV}_${ARCH}

RUNZIP=${MOD}_${VER}-${REV}_${ARCH}.zip
DEVZIP=${MOD}-dev_${VER}-${REV}_${ARCH}.zip

HEX=`echo $THIS | md5sum | cut -d' ' -f1`
TARGET=c:/devel/target/$HEX

usedev
usemsvs6

(

set -x

PROXY_LIBINTL=`latest --arch=${ARCH} proxy-libintl`

# Avoid the silly "relink" stuff in libtool
sed -e 's/need_relink=yes/need_relink=no # no way --tml/' <ltmain.sh >ltmain.temp && mv ltmain.temp ltmain.sh

# Avoid using "file" in libtool. Otherwise libtool won't create a
# shared library, and give the warning "Trying to link with static lib
# archive [...] But I can only do this if you have shared version of
# the library, which you do not appear to have."

# I know what I am doing, I do want to link with a static
# libintl. (The proxy-libintl libintl wrapper.)

sed -e 's!file /!dont-want-to-use-file!' <configure >configure.temp && mv configure.temp configure &&

patch -p0 <<'EOF'
EOF

CC='gcc -mtune=pentium3 -mthreads' CPPFLAGS="-I/devel/dist/${ARCH}/${PROXY_LIBINTL}/include" LDFLAGS="-L/devel/dist/${ARCH}/${PROXY_LIBINTL}/lib -Wl,--exclude-libs=libintl.a -Wl,--enable-auto-image-base" CFLAGS=-O2 ./configure --disable-gtk-doc --prefix=$TARGET &&
libtoolcacheize &&
make glibconfig.h.win32 &&
make glibconfig.h &&
mv glibconfig.h glibconfig.h.autogened &&
cp glibconfig.h.win32 glibconfig.h &&
PATH="/devel/target/$HEX/bin:$PATH" make -j3 install &&

./glib-zip &&

mv /tmp/glib-$VER.zip /tmp/$RUNZIP &&
mv /tmp/glib-dev-$VER.zip /tmp/$DEVZIP

) 2>&1 | tee /devel/src/tml/packaging/$THIS.log

(cd /devel && zip /tmp/$DEVZIP src/tml/packaging/$THIS.{sh,log}) &&
manifestify /tmp/$RUNZIP /tmp/$DEVZIP
