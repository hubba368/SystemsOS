# C and ASM Bootable OS

For the final year 'Systems Programming' module, we were tasked with developing a small bootable OS image, that will load a small library that would draw simple primitives to the screen.

The screen drawing functions were written in C. Below is a list of features of the library itself.

* Setting the colour of an individual Pixel.
* Drawing a line from one point to another, of a specific colour and thickness.
* Drawing rectangular shapes that can be both filled and unfilled.
* Clearing the screen to a particular colour.
* Drawing filled and unfilled rounded shapes.

The screen drawing library can draw in both Ring 0 (direct access to video memory) and Ring 3 (protected mode), as it was a requirement set by the specification. To achieve this functionality, a custom interrupt table was created, that Ring 3 can use to call Ring 0 functions.

The bootloader, developed in ASM, was previously created in a seperate assignment. When the OS is booted, it enters Ring 3 before displayed the drawing functions.

This program requires an emulator such as Bochs to be run.
