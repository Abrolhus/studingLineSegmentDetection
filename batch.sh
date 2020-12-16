#!/bin/bash
outputDir=out/
buildDir=build

make
mkdir $outputDir
for var in "$@"
do
    justName="$(echo $var | grep -Eo [^/]+$)"
    echo "justName: $justName"
    echo "path: $var"
    echo $outputFolder$justName
    ./main $var $outputDir$justName
    echo "Image saved to $outputDir$justName"
done

