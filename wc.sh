#!/bin/bash
LOC=`find src/ -iname *pp -exec cat '{}' \; | wc -l`
WOC=`find src/ -iname *pp -exec cat '{}' \; | wc -w`
COC=`find src/ -iname *pp -exec cat '{}' \; | wc -c`

F='data/gui/pages/* data/gui/xml/*custom*'

LOX=`cat $F | wc -l`
WOX=`cat $F | wc -w`
COX=`cat $F | wc -c`

echo code:  $LOC lines, $WOC words, $COC chars
echo data:  $LOX lines, $WOX words, $COX chars
