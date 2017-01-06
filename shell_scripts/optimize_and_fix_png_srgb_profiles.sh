#!bin/bash

find . -type f -iname '*.png' -exec pngcrush -ow -rem allb -reduce {} \;