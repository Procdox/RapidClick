# RapidClick
 A simple autoclicker

## Installation
Built with:
- Windows 10 Pro
- MSVC 2017
- QT 5.12.3

Steps:
- Compile a release build
- Copy the produced .exe and the following .ddl's from the QT installation to the desired location
  - Qt5Core.dll
  - Qt5Gui.dll
  - Qt5Widgets.dll

## Usage

The Activate key will run the autoclicker.
The Output key will fire every ~50ms when the clicker is running.
Double clicking either input will let you set a mouse button as the activate or output key.

The mode can be used to select if the Activate key should need to be held to run, or if it should toggle running.

The clicker will try not allow itself to be triggered when the mouse is over it.
The clicker may uncheck the enabled button if something goes wrong or if the Activate Key is the same as the Output Key.

