pwd
mkdir AppDir
mkdir AppDir/usr/
mkdir AppDir/usr/bin/
wget "https://github.com/AppImage/AppImageKit/releases/download/10/AppRun-x86_64" -O AppDir/AppRun 
chmod a+x AppDir/AppRun
cp ../build/examples/01.oscilloscope AppDir/usr/bin/oscilloscope
echo -e "[Desktop Entry]\nName=oscilloscope\nExec=oscilloscope\nIcon=oscilloscope\nType=Application\nCategories=AudioVideo;Audio;" > AppDir/oscilloscope.desktop
convert -size 256x256 xc:transparent AppDir/oscilloscope.png
wget "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
chmod a+x linuxdeploy-x86_64.AppImage
./linuxdeploy-x86_64.AppImage --appdir=AppDir
wget "https://github.com/AppImage/AppImageKit/releases/download/10/appimagetool-x86_64.AppImage"
chmod a+x appimagetool-x86_64.AppImage
./appimagetool-x86_64.AppImage AppDir
