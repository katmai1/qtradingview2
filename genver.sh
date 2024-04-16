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
    sed -i "s/$old_version/$new_version/g" version.h

    # compile version
    qmake QTradingview2.pro -o builds
    cd builds && make && make clean
    
else
    echo "Exiting..."
fi

# old="$(grep APP_VERSION version.h)"
# new="#define APP_VERSION \"$1\""
# sed -i "s/$old/$new/g" version.h
# qmake QTradingview2.pro -o builds
# cd builds && make && make clean

# falta configurar gitcon mi github y probar de hacer un commit y un tag
# git describe --tags --abbrev=0
# git add version.h
# git commit -m "Creando version $1..."
# git tag "v$1"
# git push origin "v$1"