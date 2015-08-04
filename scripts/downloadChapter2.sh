#!/bin/bash

if [ $# -ne 2 ];
then
  echo "Usage:"
  echo "downloadChapter.sh url path"
  exit 1
fi

url=$1

path=$2
pathformated=`echo $path | sed -r "s/\s/-/gi"`

pages=`wget -q --user-agent="Mozilla/5.0 (Windows NT 5.2; rv:2.0.1) Gecko/20100101 Firefox/4.0.1" -O - $url | grep -Eo "(lstImages.push\(\"http://[^\"]*)" | cut -d'"' -f2`
pagesformated=`echo $pages | sed -r "s/\s(http)/;\1/gi"`

mkdir -p $pathformated

IFS=';' read -ra imagesarray <<< "$pagesformated"
for image in "${imagesarray[@]}"; do
  filename=`echo ${image##*/}`
  echo $filename
  wget "$image" -O "$pathformated/$filename"
done

exit 0
