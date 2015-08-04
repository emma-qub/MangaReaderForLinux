#!/bin/bash

if [ $# -eq 0 ];
then
    echo "No manga specified." 1>&2
    exit 1
else
    # get the name of the manga
    manganame=$1
    # count how many chapters exist and setting the last chapter
    last=`wget -q -O - http://eatmanga.com/Manga-Scan/$manganame/ | grep -Eo "([0-9]+)" | head -n 1 | cut -d' ' -f3`
    echo $last
    chapternameonwebsite=`wget -q -O - http://eatmanga.com/Manga-Scan/$manganame/ | grep -Eo "(/Manga-Scan/$manganame/[^\"]*)" | grep "$chapter" | head -n 1 | cut -d'/' -f4`
    echo $chapternameonwebsite
fi
exit 0
