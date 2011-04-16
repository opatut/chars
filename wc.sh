#!/bin/bash
echo `find src/ -iname *pp -exec cat '{}' \; | wc -l` lines
echo `find src/ -iname *pp -exec cat '{}' \; | wc -w` words
echo `find src/ -iname *pp -exec cat '{}' \; | wc -c` chars

