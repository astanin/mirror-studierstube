#!/bin/sh

NAME=opentracker-2.0.0

rm -f ../$NAME
ln -s `pwd` ../$NAME
pushd .. > /dev/null
tar cfjh $NAME.tar.bz2 \
--exclude "*.7z" \
--exclude "*.Plo" \
--exclude "*.bz2" \
--exclude "*.dll" \
--exclude "*.gz" \
--exclude "*.la" \
--exclude "*.lib" \
--exclude "*.lo" \
--exclude "*.o" \
--exclude "*.os" \
--exclude "*.so" \
--exclude "*.vcproj" \
--exclude "*.zip" \
--exclude "*~" \
--exclude ".deps" \
--exclude ".libs" \
--exclude ".sconsign" \
--exclude ".svn" \
--exclude "Makefile*" \
--exclude "autom4te.cache" \
--exclude "build/*" \
--exclude "cfg" \
--exclude "config.log" \
--exclude "config.opts" \
--exclude "configure" \
--exclude "html" \
--exclude "libtool" \
--exclude $NAME.tar.bz2 \
$NAME
#tar cfjh $NAME.tar.bz2 $NAME
popd > /dev/null
mv ../$NAME.tar.bz2 .
rm ../$NAME

# the same with p7zip:
#tar xfj $NAME.tar.bz2
#rm -f $NAME.7z
#7z a $NAME.7z $NAME > /dev/null
#rm -rf $NAME
