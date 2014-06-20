==============
DAPLUGPASS 1.1
==============

Manage your passwords simply with your Daplug dongle.

Developper: Saada BENAMAR <s.benamar@plug-up.com>
Plug-up International - 2014


## How to compile ##

This is a Qt project (Qt Creator 2.8.1 based on Qt 5.1.1).

- Download and uncompress project ZIP file. 

- Copy the .pro file corresponding to your OS (WIN32/LINUX) from the "<project directory>/projectFile/" to "<project directory>/".

- Uncompress "libs.zip" file. The directory "libs" contains the required libraries.

- Open the project file from Qt Creator.

- qMake, Rebuild then Run.

- If GL error : sudo apt-get install libglu1-mesa-dev.

## Note ##

- On windows, copy the "libusb-1.0.dll" from "<project directory>/libs/WIN32/libusb/" to the runtime directory (with the executable file).

- On Windows, you may encounter a problem with the Daplug dongle driver in WINUSB mode. So you can install driver using Zadig (www.http://zadig.akeo.ie/). If you can't fix the problem using Zadig, you can use a Daplug dongle in HID mode instead.

- On Linux, add proper udev rules corresponding to a Daplug dongle.

## Tested on ##

- Windows 7 pro x86, Windows 7 pro x64 (only using HID dongles).
- Ubuntu 13.10 x64.

Should work on others.
