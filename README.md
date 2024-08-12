Starting this project to learn some things about audio processing via Juce (and coincidentally C++!), the source files are all that's in this repo. If you want to build it yourself, download Projucer, create a new project with the 'basic plugin' setup and include dsp libs and import the files. I compiled on Windows using VS2022.

TODOS:
-implement Lagrange 3rd order interpolation for the delayline to reduce audio artifacts when changing time
-touch up graphics by learning look and feel
-switch from processorchain to audiograph for flexibility
-implement proper treestate for variable controls
-figure out optimized ranges for sliders (you can get some weird effects with some of the current ranges, feature or bug, you decide)

Quick Video of the plugin in action:

https://github.com/user-attachments/assets/82fb000d-e112-4106-b0f0-ebb069ba8fc5

