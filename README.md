Starting this project to learn some things about audio processing via Juce (and coincidentally C++!), the source files are all that's in this repo. If you want to build it yourself, download Projucer, create a new project with the 'basic plugin' setup and include dsp libs and import the files. I compiled on Windows using VS2022.
<br /><br />
TODOS:<br />
-~~fix stereo issue~~<br /> - this issue is caused by not adding "{2, 2}, {1, 2}" as the plugin channel config in projucer
-implement Lagrange 3rd order interpolation for the delayline to reduce audio artifacts when changing time<br />
-implement eq<br />
-touch up graphics by learning look and feel<br />
-switch from processorchain to audiograph for flexibility<br />
-figure out optimized ranges for sliders (you can get some weird effects with some of the current ranges, feature or bug, you decide)<br />
<br />
Helpful Links<br/>
<br />
https://docs.juce.com/master/index.html
<br />
https://wiki.analog.com/_media/resources/tools-software/sharc-audio-module/baremetal/audio_processing.zip
<br />
Big thanks to The Audio Programmer's line of Youtube videos as well!
<br /><br />
Quick Video of the plugin in action:

https://github.com/user-attachments/assets/82fb000d-e112-4106-b0f0-ebb069ba8fc5

