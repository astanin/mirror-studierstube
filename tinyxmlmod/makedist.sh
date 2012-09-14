#!/bin/sh

NAME=tinyxmlmod-1.0

rm -f ../$NAME
ln -s `pwd` ../$NAME
pushd .. > /dev/null
tar cvfjh $NAME.tar.bz2 \
--exclude "*.dll" \
--exclude "*.o" \
--exclude "*.os" \
--exclude "*.vcproj" \
--exclude "*~" \
--exclude ".svn" \
--exclude "config.*" \
--exclude $NAME.tar.bz2 \
--exclude .sconf_temp \
--exclude .sconsign \
--exclude bin \
--exclude build.opts \
--exclude lib \
$NAME
popd > /dev/null
mv ../$NAME.tar.bz2 .
rm ../$NAME
