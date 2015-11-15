#!/bin/bash

if [ $# -ne 2 ];
then
  echo "Usage: $0 manganame distancemin" 1>&2
  exit 1
fi

# get manga name
manganame=$1

# get distance min
distancemin=$2

function levenshtein {
  if [ "$#" -ne "2" ]; then
		echo "Usage: $0 word1 word2" 1>&2
	elif [ "${#1}" -lt "${#2}" ]; then
		levenshtein "$2" "$1"
	else
		local str1len=$((${#1}))
		local str2len=$((${#2}))
		local d i j
		for i in $(seq 0 $(((str1len+1)*(str2len+1)))); do
			d[i]=0
		done
		for i in $(seq 0 $((str1len)));	do
			d[$((i+0*str1len))]=$i
		done
		for j in $(seq 0 $((str2len)));	do
			d[$((0+j*(str1len+1)))]=$j
		done

		for j in $(seq 1 $((str2len))); do
			for i in $(seq 1 $((str1len))); do
				[ "${1:i-1:1}" = "${2:j-1:1}" ] && local cost=0 || local cost=1
				local del=$((d[(i-1)+str1len*j]+1))
				local ins=$((d[i+str1len*(j-1)]+1))
				local alt=$((d[(i-1)+str1len*(j-1)]+cost))
				d[i+str1len*j]=$(echo -e "$del\n$ins\n$alt" | sort -n | head -1)
			done
		done
    result=${d[str1len+str1len*(str2len)]}
    if [ $result -le $distancemin ];
    then
      if [ "$1" == "$manganame" ];
      then
        echo "$1 $2 $result"
      else
        echo "$2 $1 $result"
      fi
    fi
	fi
}

mangascanpage=`wget -q -O - http://eatmanga.com/Manga-Scan`
manganamegrep=`echo $mangascanpage | grep -Eo "(href=\"/Manga-Scan/$manganame/$manganame[^\"]*)"`

if [ "$manganamegrep" == "" ];
then
  #TBR
  echo "$manganame has not been found."
  #/TBR

  mangascanchapters=`echo $mangascanpage | grep -Eo "(href=\"/Manga-Scan/[a-zA-Z0-9\-]+/[a-zA-Z0-9\-]+[^\"]*)"`
  mangascanformated1=`echo $mangascanchapters | sed -r "s/\/[a-zA-Z0-9\-]+\/\shref=\"\/Manga-Scan\//|/gi"`
  mangascanformated2=`echo $mangascanformated1 | sed -r "s/href=\"\/Manga-Scan\///gi"`
  mangascanformated3=`echo $mangascanformated2 | sed -r "s/\|[^|]+$//gi"`

  IFS='|' read -ra manganamesarray <<< "$mangascanformated3"
  k=0
  for currentmanganame in "${manganamesarray[@]}"; do
    let "k += 1"
    if [ $k -gt 200 ]; then
      exit 0
    fi
    levenshtein "$manganame" "$currentmanganame"
    #echo $currentmanganame
  done
else
  echo "$manganame $manganame 0"
fi

exit 0
