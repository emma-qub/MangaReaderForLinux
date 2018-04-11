#!/bin/bash

if [ $# -ne 1 ];
then
  echo "Usage: $0 manganame" 1>&2
  exit 1
fi


# get the name of the manga
manganame=$1

# get all chapters and their url
allchapters=`wget -q -O - http://mangastream.com/manga/$manganame | grep -Eo "(href=\"http://readms.net/r/$manganame/[^<]*>[0-9]+)"`

allchaptersformated1=`echo $allchapters | sed -r "s/\shref=\"/|/gi"`
allchaptersformated2=`echo $allchaptersformated1 | sed -r "s/href=\"//gi"`
allchaptersformated3=`echo $allchaptersformated2 | sed -r "s/\"[^\>]*>/;/gi"`

# separate
IFS='|' read -ra chaptersarray <<< "$allchaptersformated3"
for chapterurlandtitle in "${chaptersarray[@]}"; do
  echo $chapterurlandtitle
done

exit 0
