#!/bin/bash

# get manga name
manganame=$1

# WGET
mangascanpage=`wget -q -O - http://eatmanga.com/Manga-Scan`
manganamegrep=`echo $mangascanpage | grep -Eo "(href=\"/Manga-Scan/$manganame/$manganame[^\"]*)"`

# Result
if [ "$manganamegrep" == "" ];
then
  exit 1
else
  exit 0
fi
