# My_UE_RealsensePlugin
Creating a plugin to access the new Generation of Intel Realsense cameras, expecially the D435, in the Unreal Engine, and attach it to a HMD for Augmented Reality.

Please download the [Realsense drivers](https://github.com/IntelRealSense/librealsense) manually.

# Installation
In our Unreal Project, create a _Plugins_ directory (if not existant yet).
In the Plugins directory, create an folder for this plugin (like _"RealsensePlugin"_).
Unzip or clone this repository into that folder. Reopen our Project.
Make sure the plugin is enabled and you can access the plugins´ content.

All default projects and blueprints are made for the Intel D435 specifically, however, it should be very easy to access other Realsense cameras.

# Try out
Open the _Maps_ folder of the Realsense Plugin content. There are 3 Maps. Start with _Test_Streams_ and see if everything works. (Depth stream not accessed yet due to unkown error during development).
_Test_RGB_AR_ and _Test_Stereo_AR_ both provide a not-very-good Augmented Reality Application. Therefore, the Realsense has to be attached to an HMD like the HTC VIVE (it is only tested with the vive). 

# Known issues
There is a *pose calculation problem*, since the position of the realsense differes from the HMD itself. This is not compensated, so the virtual objects move different than the real ones in the video stream. But this plugin probably won´t be developed further anytime soon...

But on the other Hand this plugin may provide a good starting point for someone trying to access a new realsense camera within Unreal, since the [deprecated Unreal Plugin](https://software.intel.com/en-us/articles/intel-realsense-sdk-plug-in-for-unreal-engine-4) is not compatible with the newer [Realsense SDK 2.0](https://software.intel.com/en-us/realsense/sdk) 2018
 (for D400 series).
