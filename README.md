# Bypass LKM
Use this app to bypass loadable kernel module authentication on Android phones.  This is handy when you're developing your own modules or you've mixed and matched OS and firmware versions.

A few offsets are [included](https://github.com/asm/bypasslkm/blob/master/app/src/main/jni/bypasslkm.c#L23-L28) but if your phone is not listed, you'll need to follow the instructions [here](http://forum.xda-developers.com/showthread.php?t=2578566) to find your offset (requires a disassembler).

Use Android Studio (with the NDK installed) to build this app.  Good luck!

