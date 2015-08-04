#!/bin/bash

if [ $# -eq 0 ];
then
    echo "No manga specified." 1>&2
    exit 1
else
    # get the name of the manga
    manganame=$1
    manganamewithspace=`echo ${manganame//_/ }`
    # count how many chapters exist and setting the last chapter
    last=`wget -q --user-agent="Mozilla/5.0 (Windows NT 5.2; rv:2.0.1) Gecko/20100101 Firefox/4.0.1" -O - http://mangafox.me/manga/$manganame/ | grep -Eoi "(Chapter\s[0-9]{1,3}(\s-\s[0-9]{1,3})?)" | head -n1 | grep -Eo "([0-9]{1,3}$)"`
    echo $last
    # get chapters list
    chapterlist=`wget -q --user-agent="Mozilla/5.0 (Windows NT 5.2; rv:2.0.1) Gecko/20100101 Firefox/4.0.1" -O - http://mangafox.me/manga/$manganame/ | grep -Eoi "($manganamewithspace\s[0-9]{1,3})" | sed "s/Nisekoi/\nNisekoi/gi"`
    #echo $chapterlist | sed -r "s/([0-9]{1,3})\s/\1\n/gi"
fi
exit 0
