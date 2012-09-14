#!/bin/sh

NAME=stb4Applications-1.0

rm -f ../$NAME
ln -s `pwd` ../$NAME
pushd .. > /dev/null
tar cfjh $NAME.tar.bz2 \
--exclude "*.bz2" \
--exclude "*.dll" \
--exclude "*.gz" \
--exclude "*.o" \
--exclude "*.old*" \
--exclude "*.os" \
--exclude "*.so*" \
--exclude "*.vcproj" \
--exclude "*.zip" \
--exclude "*~" \
--exclude ".sconsign" \
--exclude ".svn" \
--exclude "Makefile*" \
--exclude "config.opts" \
--exclude "html" \
--exclude "stb-4.0" \
$NAME
popd > /dev/null
mv ../$NAME.tar.bz2 .
rm ../$NAME

#--exclude "build" \
