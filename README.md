weaponfinder
============
weaponfinder is a modification for detecting weapon objects on San Andreas: Multiplayer servers.

Unlike other modifications that do basically the same thing, weaponfinder is
* **efficient**: it uses a Direct3D overlay that doesn't affect game performance,
* **easy to understand and modify**: it's written in C, not SCM,
* **intuitive**: the color of the GUI overlay changes depending on how close the object is,
* **customizable**: the font size can be changed at will, making it usable even at low resolutions.

Installation
------------
You need the latest version of SAMPFUNCS to use this mod.

* Download the SAMPFUNCS C++ API [here](http://blast.hk/threads/6498/).
* Copy *game_api* and *SAMPAPI* into */weaponfinder/*.
* Open the project in Visual Studio and build the solution.
* Copy the binary file from */bin/* to your SAMPFUNCS directory.

Help
----
Use the */w(eapon)f(inder)* command in-game for general help.

*/wf toggle* turns the overlay on or off. */wf fontsize* sets the font size of the GUI. */wf color* turns the heat map on or off.

License
-------
weaponfinder is an open source project covered by the MIT License. See LICENSE for more information or visit http://opensource.org/licenses/MIT.
