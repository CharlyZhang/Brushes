#!/bin/bash
  
if [ "$#" != "2" ]; then
  
  #echo "Usage: `basename $0` dir filter"
  echo "Usage: `basename $0` inDir outDir"
  
  exit
  
fi
  
dir=$1
  
filter=$2
out=$2 
  
echo "input directory: " $1

#for file in `find $dir -name "$2"`; do
for file in ${dir}/*; do
  filename=`basename $file`
  echo "converting... " "$filename"
  outfile=${out}"/"${filename}
  iconv -f gb2312 -t utf8 $file >> $outfile  
done

