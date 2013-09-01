# This is a shell script that calls functions and scripts from
# tml@iki.fi's personal work environment. It is not expected to be
# usable unmodified by others, and is included only for reference.

MOD=pango
VER=1.24.2
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

DEPS=`latest --arch=${ARCH} glib libpng cairo fontconfig freetype`
PROXY_LIBINTL=`latest --arch=${ARCH} proxy-libintl`

PKG_CONFIG_PATH=/dummy
for D in $DEPS; do
    PATH=/devel/dist/${ARCH}/$D/bin:$PATH
    PKG_CONFIG_PATH=/devel/dist/${ARCH}/$D/lib/pkgconfig:$PKG_CONFIG_PATH
done

# Don't do any relinking crap in libtool.

sed -e 's/need_relink=yes/need_relink=no # no way --tml/' <ltmain.sh >ltmain.temp && mv ltmain.temp ltmain.sh

# Avoid using "file" in libtool. Otherwise libtool won't create a
# shared library, and give the warning "Trying to link with static lib
# archive [...] But I can only do this if you have shared version of
# the library, which you do not appear to have." 

# I know what I am doing, I do want to link with a static libintl
# now. (The proxy-libintl libintl wrapper.) (Although Pango as such
# doesn't use any functions from libintl, the glib pc file insists on
# specifying -lintl on its Libs line, so libtool needs to find
# libintl.)

sed -e 's!file /!dont-want-to-use-file!' <configure >configure.temp && mv configure.temp configure

patch -p0 <<\EOF
EOF

CC='gcc -mtune=pentium3 -mthreads' LDFLAGS="-L/devel/dist/${ARCH}/${PROXY_LIBINTL}/lib -Wl,--exclude-libs=libintl.a -Wl,--enable-auto-image-base" CFLAGS=-O2 ./configure --enable-debug=yes --disable-gtk-doc --without-x --prefix=c:/devel/target/$HEX --enable-explicit-deps=no --with-included-modules=yes &&
libtoolcacheize &&

PATH=/devel/target/$HEX/bin:.libs:$PATH make install &&

./pango-zip.sh &&

cd $TARGET

zip /tmp/${MOD}-dev-${VER}.zip bin/pango-view.exe
zip /tmp/${MOD}-dev-${VER}.zip share/man/man1/*.1

mv /tmp/${MOD}-${VER}.zip /tmp/$RUNZIP &&
mv /tmp/${MOD}-dev-${VER}.zip /tmp/$DEVZIP

) 2>&1 | tee /devel/src/tml/packaging/$THIS.log

(cd /devel && zip /tmp/$DEVZIP src/tml/packaging/$THIS.{sh,log}) &&
manifestify /tmp/$RUNZIP /tmp/$DEVZIP
