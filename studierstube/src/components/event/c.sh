#!/bin/sh

for i in `find -name "*.cpp"`; do
  a=`echo $i | sed -e "s/cpp/cxx/"`
  mv $i $a
done

