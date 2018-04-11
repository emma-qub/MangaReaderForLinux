#!/bin/bash

if [ $# -ne 3 ];
then
  echo "Usage: $0 url pathtomanga chaptername"
  exit 1
fi

# get url
url=$1

#get url directory (remove '1' at the end)
url_directory=${url::-1}

# get path to manga
pathtomanga=$2

# get chapter name
chaptername=$3

# count how many pages the chapter contains
nbpages=`wget -q -O - $url | grep -Eo "(Last Page \([0-9]+)" | grep -Eo "([0-9]+)"`

# create a directory to record the current chapter in
chapterdir="$pathtomanga/$chaptername"
mkdir -p $chapterdir

# get every pages of the current chapter
for j in $( seq 1 $nbpages ); do
  extension=`wget -q -O - $url_directory$j | grep -Eo "(//img.readms.net/cdn/manga/[^\"]+)" | cut -d'/' -f8 | cut -d'.' -f2`
  formatedPage=`printf "%.3d" $j`
  url_to_download=`wget -q -O - $url/$j | grep -Eo "(//img.readms.net/cdn/manga/[^\"]+)" | head -n 1`
  url_to_download="https:"$url_to_download
  echo $url_to_download | wget -i - -O $chapterdir/$formatedPage.$extension
  echo "$j/$nbpages"
done

exit 0
