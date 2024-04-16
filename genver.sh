#!/bin/bash

read -t 10 -p "Have you saved all changes and committed before running this script? (y/N): " RES
lres="${RES,,}"
if [[ $lres == "y" || $lres == "yes" ]]
then
    # get last version
    read -ra old <<< "$(grep APP_VERSION version.h)"
    old_version=${old[2]}
    echo "Last version is: $old_version"
    
    # get new version
    read -r -p "Input the new version: " new_version
    echo "$new_version"

    # replace version on version.h
    sed -i "s/$old_version/\"$new_version\"/g" version.h

    # compile version
    qmake QTradingview2.pro -o builds
    cd builds && make && make clean

    # git
    git add builds/QTradingview2
    git add ../version.h
    git commit -m "Creating version v$new_version..."
    git tag "v$new_version"
    git push origin "v$new_version"

else
    echo "Exiting..."
fi
