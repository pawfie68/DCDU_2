# DCDU
---------------------------------------------------------------------------------------------------------------------------------
To work with this application make sure that WxWidgets 3.1.3 and propper Environment Variables are added to the operating system,
Make sure to attach localization of the WxWidgets to compiler and linker options.
On linux system make sure to install libgtk-3.2-dev and then build wxWidgets, with config flag --use-gtk=3.2
---------------------------------------------------------------------------------------------------------------------------------
This app can be compiled in any IDE, when compiler and linker flags are properly positioned


examlple setup for Ubuntu -> Code::Blocks

Fastest setup under linux is to download code, create new empty project in CodeBlocks and import all *.h and *.cpp files


Global compiler settings -> Other compiler options
-l/usr/local/lib/wx/include/gtk3-unicode-3.2
-l/usr/local/include/wx-3.2
-D_FILE_OFFSET_BITS=64
-DWXUSINGDLL
-D_WXGTK
-ptrhead

Linker settings:
/usr/local/lib/libwx_baseu-3.2.so
/usr/local/lib/libwx_gtk3u_core-3.2.so


For Windows based platform download wxwidgets 3.2  source code from https://www.wxwidgets.org/downloads/
Then build it using your IDE ex. Visual Studio 2019 -> remember to set up language standard for c++ for at least C++ 11

After source code is succesfully builded (if not run batch build -> rebuild) set up the Environment Variables for Windows
For example, WXWIN could be "c:\wxWidgets\3.4.5" and <wx-lib-dir> could be c:\wxWidgets\3.4.5\lib\vc_x64_lib for 64-bit static libraries built with MSVC.

After framework is succesfully installed, the code should be compilable under both linuc and windows platforms.
---------------------------------------------------------------------------------------------------------------------------------
This cross platform app will display the GUI and maintain the data received in the form of XML file.
The app should handle different screen resolution
---------------------------------------------------------------------------------------------------------------------------------
Current status -> GUI is displaying correctly, 
1) Every element of the GUI scales correctly with the screen resolution,
2) Text placing demand some maintance -> additional panel should be placed at the top of the screen,
3) Basic Network handlers are implemented, connections, read and write to the serwer works correctly,
Received data are displayed on main panel. 
4) Functions for data processing, need to be developed
Done -> (5) Handling of config file need to be aded, some code was already prepared. (config.ini on Unix / Registry-Keys on Windows)
if alredy exist will be holding the IP adress and port of the server
6) IP setup frame will be added soon,
7) Button combination for initialization of IP setup frame will be added (MSG+ + left bottom button will fire the event)
8) Left and right bottom buttons will be used for creation of IP adress, left buttons - selecting the number, right buttons - chose value
---------------------------------------------------------------------------------------------------------------------------------
Virtual buttons will be replaced by physical ones after handling all other problems

