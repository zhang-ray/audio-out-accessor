pwd
mkdir AppDir
mkdir AppDir/usr/
mkdir AppDir/usr/bin/
cp ../build/examples/01.oscilloscope AppDir/AppRun
cp ../build/examples/01.oscilloscope AppDir/usr/bin/oscilloscope
echo "[Desktop Entry]\nName=oscilloscope\nExec=oscilloscope\nIcon=oscilloscope\nType=Application" > AppDir/oscilloscope.desktop
convert -size 256x256 xc:transparent AppDir/oscilloscope.png
wget "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
chmod a+x appimagetool-x86_64.AppImage
./appimagetool-x86_64.AppImage AppDir
