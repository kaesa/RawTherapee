#!/bin/bash

SYSTEM=ucrt64

mkdir bundle
cp -R release/* bundle/

DLLS=($(ldd "release/rawtherapee.exe" | grep "/$SYSTEM/bin/" | awk '{print($1)'}))
also_copy=("gspawn-win64-helper.exe" "gspawn-win64-helper-console.exe" "gdbus.exe")

for file in ${also_copy[@]}
do
    cp /$SYSTEM/bin/$file bundle/
done
for file in ${DLLS[*]}
do
    cp /$SYSTEM/bin/$file bundle/
done

mkdir -p "bundle/share/icons/Adwaita/symbolic"
adwaita=("actions" "devices" "mimetypes" "places" "status" "ui")
for dir in ${adwaita[@]}
do
    cp -r /$SYSTEM/share/icons/Adwaita/symbolic/$dir bundle/share/icons/Adwaita/symbolic/$dir
done
cp /$SYSTEM/share/icons/Adwaita/index.theme bundle/share/icons/Adwaita
#Other files
mkdir -p bundle/lib
cp -r /$SYSTEM/lib/gdk-pixbuf-2.0 bundle/lib/gdk-pixbuf-2.0
mkdir -p bundle/share/glib-2.0/schemas
cp -r /$SYSTEM/share/glib-2.0/schemas/gschemas.compiled bundle/share/glib-2.0/schemas
mkdir -p bundle/share/lensfun
cp -r /$SYSTEM/share/lensfun/version_1 bundle/share/lensfun/

mkdir -p "bundle/share/gtk-3.0/"
echo -e '[Settings]\ngtk-button-images=1' > "bundle/share/gtk-3.0/settings.ini"
