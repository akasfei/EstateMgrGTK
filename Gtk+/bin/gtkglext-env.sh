#!/bin/sh

export GTKGLEXT_PREFIX=C:/GtkGLExt/1.0

echo "Setting environment variables for GtkGLExt."
echo ""

case $TERM in
cygwin)
  GTKGLEXT_PATH=`cygpath -u ${GTKGLEXT_PREFIX}/bin`
  ;;
msys)
  GTKGLEXT_PATH=/${GTKGLEXT_PREFIX%:*}${GTKGLEXT_PREFIX#*:}/bin
  ;;
*)
  echo "Unknown TERM"
  ;;
esac

echo "export PATH=\"${GTKGLEXT_PATH}:\${PATH}\""
export PATH="${GTKGLEXT_PATH}:${PATH}"

echo "export PKG_CONFIG_PATH=\"${GTKGLEXT_PREFIX}/lib/pkgconfig;\${PKG_CONFIG_PATH}\""
export PKG_CONFIG_PATH="${GTKGLEXT_PREFIX}/lib/pkgconfig;${PKG_CONFIG_PATH}"

echo ""
