#!/bin/bash

old="$(grep APP_VERSION version.h)"
new="#define APP_VERSION \"$1\""
sed -i "s/$old/$new/g" version.h

# falta configurar gitcon mi github y probar de hacer un commit y un tag
