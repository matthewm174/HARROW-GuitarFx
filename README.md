# HARROW
Starting this project to learn some things about audio processing via Juce, the source files are all that's in this repo. If you want to build it yourself, download Projucer, create a new project with the 'basic plugin' setup and include dsp libs and import the files.  
I compiled on Windows using VS2022.  

Now, you can also just download the vst3 in the release section.  


## Features:
- [x] 7 distortion types with an unreasonable amount of gain, as well as distortion thresholds for ceiling (slam) and floor (squash)
- [x] cab emulation
- [x] 2 band EQ
- [x] tube emulation
- [x] Pre- and Post- noise gates to reduce noise
- [x] LP filter at the beginning of the chain to tighten
- [x] saveable presets 

Helpful Links  

https://docs.juce.com/master/index.html  

https://wiki.analog.com/_media/resources/tools-software/sharc-audio-module/baremetal/audio_processing.zip  
https://ccrma.stanford.edu/~dtyeh/papers/DavidYehThesissinglesided.pdf
  
https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
  
Big thanks to The Audio Programmer's line of Youtube videos as well!
  

