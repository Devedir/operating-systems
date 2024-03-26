#!/bin/bash

if [[ $# -eq 0 ]]; then
	echo "This script must take a parameter: the exercise number id (01, 02, ...)!"
	exit 1
fi

# Change me!
name=DabekWojciech

echo -e "Cleaning...\n"

echo "Make cleans..."
shopt -s globstar
for f in ./cw$1/**/Makefile; do
	make -C "${f%/*}" clean;
done

echo -e "\nRemoving CLion mess..."
find . -name ".idea" -type d -exec rm -rfv {} +

echo -e "\nAll tidy!\n"

echo "Archiving your work..."
output="${name}-cw$1.tar.gz"
cd "cw$1"
tar cvzf "../${output}" *
cd ..

echo -e "\nDone, your work is ready to send!"
echo "  -> ${output}"

exit 0
