# Serial Interface for the Arduino with floating number support  
 <b>Summary</b>:Arduino Serial Interface that supports a command line interface with floating point numeric data support<br>
 SERIAL BAUD RATE: 9600
 ## Scope
This project was designed to provide more flexibility to the Arduino after the firmware had been uploaded.  This interface can easily be tailored for different applications and allows different functions and variables to be set from a serial interface.  This allows for example, data logging to be started and stopped when called from other programs such as <b>matlab</b> or <b>python</b>.  At the [University of Western Australia](https://www.uwa.edu.au/), I use this code extensively to control equipment that might undertake (for example) current monitoring, heater control and laser control just to name a few.

At start up (and when the splash screen is invoke by typing '?') the following appears<br>
<b> Data Logging ................. By [Adrian Keating](http://www.web.uwa.edu.au/person/adrian.keating) (C) 2017, VERSION:1.00<br>
Command Set (use either upper or lower case, commands with numbers end in x or CR):<br>
? - Help<br>
In - Function requiring float input, x.x=float<br>
Hx - Function requiring integer input, n=integer, n=0: turn LED off, n=0:turn LED on<br>
C - Calibration Routine<br>
L - Start Data Logging<br>
X - Stop Data Logging<br>
V - Verbose mode<br>
S - Silent mode (no labels on data)</b><br>

Users should [edit the first line of the .ino file](./SerialArdCMD/SerialArdCMD.ino) to indicate the function of their code and the VERSION number (edit the <code>#define VERSION</code> line in the code).   Please keep an acknowledge of my original contribution in the source code. Keeping track of version numbers from the splash screen is an important way to track your code development.  Each of the letters used for the commands defines a condition within the <code>switch</code> statement within the <code> void loop()</code>.  Edit these letters to best represent the functions you desire.

Commands sent can be either lower or upper case (case is ignored) and all commands can be terminated with either a newline, carriage return or the symbol <code>;</code>. Using the separator <code>;</code> allows multiple commands to be sent to the Arduino for sequential execution, for example v;l;h1;i55.5;x;  which would (in the following order
* enter verbose mode
* start the loop which calls the <code>logdata()</code> function
* turns on the LED
* sets the float parameter for to 'i' command to 55.5
* stops the loop which calls the <code>logdata()</code> function

This would then send to the serial monitor:<br>
<code>Integer = 1 <br>
Float = 55.500<br>
Time(s)= 0.05 </code><br>
 where the Time indicates the time taken for this loop to run.
 
An important addition here is the support for float (and integers by recasting into int) using the <code>ReturnFloat()</code> function.  This function:
* allows floating point number by entering a number with a decimal point.  Exponent format is not yet supported, for example in the above, I55.5  calls the 'I' switch statement and set the <code> myfloat</code> variable to 55.5.  This could then be used in the code for various functions, allowing the interface to change parameters within the code (or EEPROM) as required.  For example, I can control the current applied to a laser diode through this interface.
* allows for white spaces before the number. For example, the above command could have been issued as I55.5 or I 55.5 or I  55.5
* allows for negative numbers.  For example, the above command could have been issued as I-55.5 if -555. was desired.  Note that this routine supports (or more rightly ignores) + symbols and allows cascaded sign-logic so that the command I-55.5  could have also been issued using I---55.5 or I-+55.5.  This is useful where the code generating the commands may be just be dumb and concatenate sign strings - unlikely but I wanted to trap it anyway.


 ## Functions
(<b>Current commands</b> - these are easily changed in the switch statement - see code)<br>
<b>s</b> - enter silent mode.  In this mode, no print statement are send out to the serial coms port.  usefull where running code at the highest speed is required.<br>
<b>v</b> - enter vebose mode.  This is useful during debugging to and check the code.<br>
<b>l</b> - start loggin (or what ever function is place in the <code>logdata()</code> function)<br>
<b>?</b> - prints out the splash screen (instructions for the commands and the version number)<br>
<b>x</b> - entering x stops data logging or whatever routine is set to run in the loop.  With Verbose on, this command returns the time taken since the ‘l’ command was given to start the logging (or loop).  Continuing to send the 'x' command reports the new time since the logging/loop command was given.  Note however the actual logging/loop was stopped after the first issue of the 'x' command.<br>

