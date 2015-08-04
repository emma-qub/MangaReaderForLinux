#!/bin/bash

if [ $# -ne 1 ];
then
  echo "No manga name specified." 1>&2
  exit 1
fi


# get the name of the manga
manganame=$1

# build domain name
domain="http:\/\/eatmanga.com"

# get all chapters and their url
allchapters=`wget -q -O - http://eatmanga.com/Manga-Scan/$manganame | grep -Eo "(href=\"/Manga-Scan/$manganame/[^<]*)"`

allchaptersformated1=`echo $allchapters | sed -r "s/\shref=\"/|$domain/gi"`
allchaptersformated2=`echo $allchaptersformated1 | sed -r "s/href=\"/$domain/gi"`
allchaptersformated3=`echo $allchaptersformated2 | sed -r "s/\"[^\>]+>/;/gi"`

# separate
IFS='|' read -ra chaptersarray <<< "$allchaptersformated3"
for chapterurlandtitle in "${chaptersarray[@]}"; do
  echo $chapterurlandtitle
done

exit 0
