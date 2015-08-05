#!/bin/bash

if [ $# -ne 3 ];
then
  echo "Usage: downloadChapter.sh url pathtomanga chaptername"
  exit 1
fi

# get url
url=$1

# get path to manga
pathtomanga=$2

# get chapter name
chaptername=$3

# count how many pages the chapter contains
nbpages=`wget -q -O - $url | grep -Eo "( of [0-9][^\"]+)" | head -n 1 | cut -d'&' -f1 | cut -d' ' -f3`

# create a directory to record the current chapter in
chapterdir="$pathtomanga/$chaptername"
mkdir -p $chapterdir

# get every pages of the current chapter
for j in $( seq 1 $nbpages ); do
  extension=`wget -q -O - $url/page-$j | grep -Eo "(http://s[0-9].eatmanga.com/mangas/Manga-Scan/[^\"]+)" | head -n 1 | cut -d'/' -f8 | cut -d'.' -f2`
  formatedPage=`printf "%.3d" $j`
  wget -q -O - $url/page-$j | grep -Eo "(http://s[0-9].eatmanga.com/mangas/Manga-Scan/[^\"]+)" | head -n 1 | wget -i - -O $chapterdir/$formatedPage.$extension
  echo "$j/$nbpages"
done

exit 0
