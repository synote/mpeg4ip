<span id="top"></span>
# MPEG4IP

May, 2005

* [MPEG4IP Project](#Project)
* [Overview](#Overview)
* [Legal](#legal)
* [Darwin Quicktime Streaming Server](#dss)
* [Prerequisites for Building](#pre)
* [Building and Installing](#build)
* [Bootstrap Options](#config)
* [OS Supported](#os)
* [Executables](#executables)
* [Executables Notes](#exnotes)
* [Directory Structure](#directory)
* [Standards Compliance](#standards)
* [Latency in mpeg4ip](#latency)
* [Player Information](#player)
* [Known Problems](#problems)
* [Contributing Back](#contrib)

<span id="Project"></span>
## MPEG4IP Project

The MPEG4IP project provides a standards-based system for encoding, streaming, and playing encoded audio, video and text. To achieve this we've integrated a number of existing open source packages, and created some original code to fill in the gaps.

Please note this project is intended for developers who are interested in standards based audio and video, and Internet streaming. It is not intended for end-users. Please read all the legal information in the file [COPYING](../COPYING)!

Also note that the primary development focus of this project is the Linux platform. If you're going to use the package on other platforms, especially non-UNIX platforms, you'll probably have some work to do.

Please use the SourceForge site to report problems, suggest enhancements, ask questions, etc. The URL is [http://www.sourceforge.net/projects/mpeg4ip](http://www.sourceforge.net/projects/mpeg4ip)

There is also a project web site at [http://www.mpeg4ip.net/](http://www.mpeg4ip.net/) that has some general information on MPEG4IP.

We also have a guide to MPEG4IP donated by everwicked. See [doc/MPEG4IP_Guide.pdf](MPEG4IP_Guide.pdf).

[Back to top](#top)

<span id="Overview"></span>
## Overview

There are two ways to use MPEG4IP to create content:

The older method assumes that you've somehow managed to capture raw audio and/or video into a file. That's the starting point from which you can use the MPEG4IP encoding tools to create an MP4 file. The simplest method being to use the 'mp4encode' script. Detailed instructions for this script and the individual tools that it uses are in [doc/encoding.htm](encoding.htm)

**Note that this might be a bit out of date; we're no longer including some of the packages like xvid and faac - they are seperate downloads, so the command lines may change. See those packages for specifics on command line.**

The newer method is an integrated live encoding tool called mp4live. This program is designed to make it easy to create MP4 files or transmit live audio/video streams over the network. It can even do both things simultaneously! The key requirement to use this tool is to have Linux, a video capture device and a Video for Linux (v4l) or Video for Linux2 (v4l2) driver for it. So far we've tested with the bttv driver for Brooktree based video capture cards, and the qce driver for Logitech QuickCam Express USB webcams. Both of these solutions can be acquired for about $50 US! Please see the file [MP4LIVE_README.html](m4rm.html) for more information about mp4live.

Once an MP4 file is prepared, it can be placed in the content directory of a streaming server. We typically use Apple's Darwin Streaming Server, but any server that understands MP4 files (or hinted Quicktime files) can be used.

When the content is encoded and available on the server, you can run the player. Start 'gmp4player' and then enter the RTSP URL to the server and the content, (command line works too). E.g.
    `$ gmp4player rtsp://myserver.mydomain.com/mycontent.mp4`

Although we're focused on streaming, the player will also playback from a local file. E.g
    `$ gmp4player mycontent.mp4`

That's not all! The player is not limited to local playback of MP4 files. It can also read AVI, CMP, DIVX, AAC, MP3, MPEG 1/2 and WAV. This is useful for debugging since the encoded data can be check independently of the MP4 file container, and known good content, such as your favorite MP3, can be used to verify that the player is working correctly with your hardware. E.g.
    `$ gmp4player mymusic.mp3`

For the latest list of playback codecs, see the [feature](../FEATURES.html) list.

And that's still not all... You can set up your player to run a playlist by creating a simple text file with the extension .mp4plist, .m4u, or .mxu, with each item (file or stream) on it's own line.

Note: If you prefer a no UI version of the player, or your system does not include GTK or GLIB, just the bare video window, 'mp4player' is available to fit that need.

[Back to top](#top)

<span id="legal"></span>
## Legal:

Please see the file [COPYING](../COPYING).

[Back to top](#top)

<span id="dss"></span>
## Darwin Quicktime Streaming Server

Please note that the Apple Darwin Quicktime Streaming Server is NOT distributed with mpeg4ip. It can be downloaded from [Apple](http://www.apple.com/), as either source or pre-built binaries. For those who choose the source option, please see the documentation that accompanies the server on how to build and configure it for your environment.

Note the default content directory is /usr/local/movies and that you can configure the darwin server by pointing your browser at port 1220\. (ie: localhost:1220)

There are some sample mp4 files included with the Darwin Streaming server. [Envivio](http://www.envivio.com/), also has some sample mp4 files. We suggest first downloading one of these samples and try opening the file with gmp4player. If that works, then try copying the file to the streaming server's content directory (e.g. /usr/local/movies), and enter the appropriate RTSP URL in gmp4player. You may have to "hint" the files before they will stream.

If you should have problems with DSS, please feel free to contact the developers over at Apple - they're a friendly bunch. They have 2 mailing lists - one for [users](http://lists.apple.com/mailman/listinfo/streaming-server-users) and the other for [developers](http://lists.apple.com/mailman/listinfo/streaming-server-developers). Give them a try.

There is a known problem (November, 2004) with running DSS and mp4live on the same system. Hopefully it will be fixed soon, or please see the sourceforge forums for more information

[Back to top](#top)

<span id="pre"></span>
## Prerequisites for Building

This section will discuss prerequisites for building mpeg4ip (ie: packages that you will first want to install to build correctly). This does not include Windows. To build on Windows, see [these instructions.](#windoze)

Specifics on how to build using these libraries will be discussed [below.](#build)

All builds require the [SDL](http://www.libsdl.org) development libraries. SDL version 1.2.7 or later is required; please be sure to install the development library, if you are using RPMs.

If you have a x86 based platform, you will want to install nasm. If you do not install it, you may not be able to decode or encode at the frame rates desired. See the [nasm site](http://nasm.sourceforge.net) for information.

If you are building either the player, or mp4live, you will want to install GTK-2\. We no longer support GTK-1.2\. Make sure to install the development packages, as well.

If you are building the player, you may want the following packages:

*   [ffmpeg](http://ffmpeg.sourceforge.net) with AMR narrow-band and wide-band packages
*   [xvidcore](http://www.xvid.org) (versions 0.9.8 or later)
*   [libmpeg2](http://libmpeg2.sourceforge.net) (highly recommended for mpeg2 decoding)
    note: for gcc 4.0, you will need to configure with --enable-shared=yes
    you may also need to remove some inlines from libmpeg2/cpu_accel.c
*   [libid3](http://tangent.org/index.pl?node=libID3) - id3 tags for mp3 files
*   [liba52](http://liba52.sourceforge.net) - AC3 decoding for mpeg files and mpeg2 transport streams
    note: for gcc 4.0, you will need to configure with --enable-shared=yes
*   [faad2](http://www.audiocoding.com) [(install after installing mpeg4ip)](#faad2)

The following packages are optional for mp4live, but will enable more functionality:

*   [faac](http://www.audiocoding.com), version 1.20.1 or later
    If you are using a released or CVS version after version 1.24, use the --with=mp4v2=no option
*   [lame](http://lame.sourceforge.net) (mp3 encoding)
*   [twolame](http://www.twolame.org) (mpeg1, layer 2 encoding)
*   [ffmpeg](http://ffmpeg.sourceforge.net) (H.263, mpeg-4, mpeg-2 video content, AMR, mpeg-2 audio)
*   [xvid](http://www.xvid.org) (mpeg-4 video encoding)
*   [x264](http://www.videolan.org/x264.html) (H.264 video encoding)
*   V4L2 interface (comes with 2.6 kernel, Fedora Core is a special case)
*   [libSRTP](http://srtp.sourceforge.net/srtp.html) (Adds encryption and authentication to RTP streams)
    Must be version 1.5 or later.

The developer recommends ffmpeg for mpeg-4 encoding. Please see the [instructions below](#ffmpeg) for how to install with ffmpeg.

If you are going to encode mpeg-4 or H.264, you will also want to include faac for aac encoding; most other players will not play other types of audio with those video encodings.
faac versions after 1.24 will require the --with-mp4v2=no argument to the configure statement.

[Back to top](#top)

<span id="build"></span>
## Building and Installing

Please read the section on prerequisites and this section completely before building, or you may have to do it twice; mpeg4ip has a number of optional packages that should be installed before starting the build process.

If you are building on Windows, please see these [instructions.](#windoze)

On a linux or unix base system, you should only have to issue the following:

> <div><kbd>./bootstrap <params for configure>
> make
> make install (needs root privileges).
> </kbd></div>

Several systems have differing requirements. Please see the section on [Operating Systems Supported](#os) before continuing.

If you wish to build mpeg4ip without mp4live, you can give the `--disable-mp4live` or `--disable-server` arguments. Other systems will automatically detect that you are not building on Linux and will not build mp4live.

Mp4live also works best with the V4L2 interface, which is not installed on most linux distributions with a 2.4 kernel. mp4live will work without the V4L2 interface, but you will have long term audio/video synchronization problems (where long term could be minutes or hours). We give a little information on [how we've installed V4L2](Building_mpeg4ip_with_V4L2_support.html). 2.6 series kernels should have V4L2 built in.

Fedora Core 3 and below do not include the correct include files to activate V4L2\. At this point, we believe that the V4L2 is supported after installing the videodev.h and videodev2.h files, but have not verified it. We have modified the configure so it will print out a message during configure that specifies the directory where you might locate the correct files.

To install with ffmpeg version 0.4.8 or earlier, enter the argument`--enable-ffmpeg=<directory>` to the bootstrap or cvs_bootstrap. `<directory>` is required and must be the absolute path to where the ffmpeg code has been compiled - do not use ~.

For version 0.4.9 or later, we recommend that you install the ffmpeg libraries and include files. When installing ffmpeg, use the additional `make installlib` command. mpeg4ip should then automatically detect that the ffmpeg library exists.

If you install ffmpeg, it is not recommend to link it with any external libraries other than the AMR libraries below. If you do, you will have to add the extra libraries to the Makefiles that use ffmpeg (in player/plugin/video/ffmpeg, player/plugin/audio/ffmpeg, and server/mp4live).

Note: as of April, 2006, the `make installlib` no longer appears to be required. A regular `make install` seems to be enough.

If you wish AMR decoding, you will want to build ffmpeg with AMR support - see their documentation for that. (Basically, use their --enable-amr_wb and --enable-amr_nb code, get the code and unzip it in the correct sub-directory - for the amr_float directory, you'll have to move the code from c-code to amr_float).

To build ffmpeg on Mac OS X, build with normal options. Edit the config.mak, and change LDFLAGS to a NULL string - i.e.
`LDFLAGS=`.

If you get an error when linking, remove the -mdynamic-no-pic from OPTFLAGS in config.mak, and rebuild ffmpeg entirely

You may also have to run `ranlib /usr/local/lib/libavcodec.a` after installing it, if you get an error that it (/usr/local/lib/libavcodec.a) is out of date.

We haven't been successful linking ffmpeg builds of 2007 on the Mac.

We've tested with ffmpeg version 0.4.7, 0.4.8 and 0.4.9-pre1\. See the [ffmpeg](http://ffmpeg.sourceforge.net) site for information on how to download. Use of CVS versions are at the discretion of the user. If you have problems with compiling with a CVS version of ffmpeg, please use a released version. If you would like to do mpeg2 encoding, you will need ffmpeg.

If you want to install faad2, please do it after you install mpeg4ip. If you have previously installed it, you must uninstall it (and most likely uninstall faac, as well). The faad2 plugin will overwrite the aac_plugin.

If you don't have root privileges, but still wish to install the distribution to a directory to which you do have write permission, then here is an example of how to do that:

> <div><kbd>mkdir -p $HOME/local/bin $HOME/local/lib
> ./bootstrap --prefix=$HOME/local
> make
> make install
> </kbd></div>

You will also have to add ~/bin to your path, and ~/lib to your LD_LIBRARY_PATH

If this process works for you, you can skip down to the [next section](#config) on Bootstrap Options. If this does not work, or you have downloaded from CVS, the below applies.

We've built the distribution using GNU autoconf, automake and libtool. We have attempted to follow the GNU conventions for open source packages. This is complicated by the fact that we build on many other packages. Where the package was already using the GNU tools, we left things alone. Where the package was using it's own Makefile, we left things alone if it was a complicated Makefile. If it was straightforward we replaced it with an equivalent automake file.

If you have to install any of these tools, find out where the others are installed (which autoconf). If the path does not start with /usr/local, use the `configure --prefix=<path before /bin on other packages>`

For example, if autoconf is in /usr/bin, use the `configure --prefix=/usr` command when installing automake or libtools.

Note: libtool is sometimes not installed on Linux systems (autoconf, and automake generally are). You can download libtool from a GNU mirror site.

In order to compile mpeg4ip from CVS, we require the following tools:
[gcc 3.2 or greater](http://www.gnu.org/software/gcc/)
[libtool 1.5](http://www.gnu.org/software/libtool/libtool.html)
[autoconf 2.57](http://www.gnu.org/software/autoconf/autoconf.html)
[automake 1.7.6](http://www.gnu.org/software/automake/automake.html)

If you don't have these tools and are trying to compile from CVS, don't complain - update to these tools, then let us know. There may be problems or warnings with later tools - we don't keep up with the latest and greatest tools most of the time. Please try to fix these yourself before continuing, and [feed back](#contrib) the changes using the Sourceforge forums.

Be sure to read the OS specific section later before continuing here.

In general, the code should be portable, but as someone once said "There's no such thing as portable code, just code that has been ported." When you find problems please be sure to use the SourceForge site to tell us what you encountered, and hopefully how you fixed it.

To build:

> <div>`./cvs_bootstrap <arguments to pass to configure scripts>
> make
> make install (optional, typically need root privileges)
> `</div>

The bootstrap script will pass any arguments to the configure scripts.

For the curious, the bootstrap script invokes the configure scripts of the included packages that have them, and then our own top level configure script is generated and run. At the end of this process all the Makefile's are ready, and setup in the correct hierarchy. If this doesn't work for you, you're free to hack as needed ;-)

If you are using cvs_bootstrap and are running into problems where your GTK configurations are not found, you will need to find the aclocal files associated with gtk and glib (in particular glib-2.0.m4, and gtk-2.0.m4). These are usually in a share/aclocal directory somewhere on your system.

We currently search /usr/local, /usr/contrib, /opt/gnome /usr /opt/local. If these do not work for you, find the above files (<samp>find . -name glib-2.0.m4 -print</samp>), and add the path (without the share/aclocal) to cvs_bootstrap.

[Back to top](#top)

<span id="config"></span>
## Bootstrap Options

Two options of potential interest are `--disable-server` and `--disable-player` which disable the building of the server and player respectively. By default both server and player are built.

Also, the `--enable-ffmpeg` option (described above) is very useful if you are using mp4live and have an older version of ffmpeg.

If you are building on a system with an Intel x86 CPU clone, you may need to specify the configuration option `--enable-mmx=no`. The configuration script automatically detects an x86 target CPU and enables MMX assembly code in the build (if the NASM assembler is available). If your CPU doesn't support MMX instructions you will want to disable this feature. The configure script will also check for the minimum version of nasm supported; we require 0.98.19 or greater.

To build with IPv6 support, use the `--enable-ipv6` command option.

[Back to top](#top)

<span id="os"></span>
## OS Supported

Currently, we have compiled and tested on the following platforms: linux, freeBSD, BSD/OS, Solaris and windows. For all varieties of *nux, X11 is required.

To date we've built on Red Hat Linux 6.1 to 9.0 with the native compilers, on 7.1 and 7.2 with gcc 3.0, and on 7.3 and later with gcc 3.1. We also support 9.0 with the native compiler (gcc 3.2) and gcc 3.3\.

As of January, 2004, we're migrating to SuSE 9.0 due to problems we're seeing with RedHat 9 and the update process.

**General Linux/UNIX**

If you have built a previous version of mpeg4ip, do a make uninstall first, or go in and remove libsndfile from your shared libraries directory, unless you have another version installed.

libSRTP: After building and installing the libSRTP pkg according to its intructions, run cvs_bootstrap or configure so mpeg4ip will find and (statically) include the libSRTP pkg. If it is not found or if --disable-srtp is passed to configure, the libSRTP dependent calls with be conditionally compiled out.

**SuSE 9.0**

When building with SuSE 9.0, you need to install a number of packages before you start.

If you're installing with SuSE 9.0 professional, you can install everything you need off the CD's. For a list of packages that I used, see the suse90pro.installed file in our distribution, or in CVS. These include packages that I needed to install the 2.6.1 kernel and use the make gconfig configuration.

If you're installing SuSE 9.0 personal edition, you should include all compiler packages, and the following packages. I recommend to install them in this order:

*   nasm - we installed nasm-0.98.38 in /usr (use the --prefix=/usr option to configure)
*   pkgconfig - we installed pkgconfig-0.15.0 in /opt/gnome
*   glib - we installed glib-2.2.3 in /opt/gnome
*   atk - we installed atk-1.2.4 in /opt/gnome
*   pango - we installed pango-1.2.5 in /opt/gnome
*   gtk+ - we installed gtk+-2.2.4 in /opt/gnome

Be sure to install the GLIB/GTK family over the existing installs - for the most part, be sure to use configure --prefix=/opt/gnome.

If you want to run gmp4player with sound, you will need to disable the aRTs sound server. Start the Control Center, select Sound & Multimedia, then select Sound System. Uncheck the box that says "Start aRts soundserver on KDE startup", and hit Apply. You will need to log out, then log back in.

If you have any other way to get sound with gmp4player without the above, let us know.

One note with SuSE 9.0 - it looks like they tried to install V4L2, but it does not work. They installed an incorrect version of videodev2.h. We have only installed V4L2 as part of the 2.6 kernel (see the [guide here](Building_mpeg4ip_with_V4L2_support.html)).

**Slackware**

When building on Slackware-8.0, you need to consider the following (from maersk):

> <div>The Slackware distribution does not as standard come with shared libraries for libXv and libXxf86dga. In other cases, where you have upgraded from xfree86 version 4.0 to 4.1, you may not have compiled the shared versions. The fix to this is in general is this:
> 
> `# pushd /usr/X11R6/lib
> # rm -f libXv.so libXxf86dga.so
> # ld --whole-archive -shared -o \
> libXv.so libXv.a
> # ld --whole-archive -shared -o \
> libXxf86dga.so libXxf86dga.a
> # popd
> `
> 
> </div>

Note that you need to be root to do this.

**Solaris**

When building on Solaris, libtool and gnu make must be installed and used. If libtool is installed, you make get a warning message that common libraries made with gcc less than 3.0 might have problems, ignore it, but don't take binary libraries from any other machines.

**FreeBSD 5.1**

There looks to be an error when building on FreeBSD 5.1 with gcc 3.2\. There is a warning in the fstream.h include file. To build, you will need to remove the -Werror from BILLSCPPWARNING in configure.in, then rebuild.

**Debian Woody**

Please see the FAQ included with the package (mp4liveOnDebian.FAQ). This was contributed by hell at netexpert dot hu.

<div id="windoze">**Windows**</div>

**NOTE:** We don't usually support the Windows code in CVS. So, unless you're explicitly told to use CVS, **don't**.

As of 1.0, we're supporting Windows less and less. You can read the code yourself - if you have trouble doing so, our windows code is not for you.

For Windows, we now require Visual Studios 2005, with the Windows SDK and Direct X.

We also have the Visual Studio 6.0 projects included, but they most likely do not work. You will need to install nasm in the VC98/bin directory before compiling. You should get nasm-0.98.22-win32.zip from the nasm web site. You will need to rename nasmw.exe to nasm.exe. We require Service Pack 5.

We also require you to install the SDL project into where ever you are compiling the mpeg4ip project. Download the latest [SDL Development Library](http://www.libsdl.org), Windows Visual C++ zip file and extract it into the mpeg4ip top level directory. You must rename it SDL, instead of SDL-1.2.x. You should get docs, an include directory and a lib directory. You may also have to install lib/SDL.dll into a Windows directory (c:\winnt\system32 or the equivalent.

If you're looking at these directions, and going "Huh?" - this project may not be right for you. Don't contact us. Like I said, we're supporting Windows less and less.

If you're using Visual Studios 2005, use the tools.sln or player.sln solutions. The executables and DLLs will be built into the Debug and Release directories under mpeg4ip.

We recommend installing DirectX 8.1 or later. If that is not possible, and you have problems with video, try uncommenting out #define OLD_SURFACE in player/src/video.cpp. Other than that, you'll have to figure it out yourself - libsdl.org is a good resource.

For Visual Studio 6 (which no longer works), use the encoding60.dsw project for encoding tools, and the player/src/player60.dsw for the player. You may have problems with certain projects not being able to load. This is due to a problem with the way Windows ends lines in their files (using a Ctrl-M, instead of a CR-LF like DOS). For some reason, I have never had these problems. You can use a unix to DOS file converter, or just open the .dsp or .dsw in Wordpad and save it.

To run mp4player other than in Visual Studio, install mp4player.exe into a directory on your Window's path (install them into the same directory). You will also need to install all the plugin .dlls into the same directory as well. Look for the *_plugin projects in player60.dsw.

In encoding60.dsw and player60.dsw, if you do a batch build, you will see several projects that do not build correctly - these common with IPv6\. This is as designed. If you want these to build, please see the appropriate package creator for the correct steps - they are not supported in mpeg4ip.

For windows GUI player, see the player README.

**Mac OSX**

We only support 10.3.2 (Panther) and later. You should have the tools installed.

You should install SDL-1.2.7 (or greater) from source, not from the binaries. We require the sdl-config script, which is not installed with the binaries.

We will build the version native to your platform. To build universal binarys, use the `--enable-universal-binary` and the `--disable-depedency-tracking` flags in either your bootstrap or cvs_bootstrap command line.

You may get an error that libtoolize is not installed correctly. Mac OSX comes with libtool installed as glibtool. Automake (which doesn't come included with OSX) does not correct its scripts.

To fix this problem, do a link of libtoolize to glibtoolize (ie: `ln -s libtoolize glibtoolize` wherever glibtoolize is installed).

The player works on MacOSX, but we have not developed a GUI for it - there is some problem with SDL that requires the video to be in the main thread, like windows.

Right now, it works well enough for testing.

[Back to top](#top)

<span id="exnotes"></span>
## Executables

If you ran 'make install' with the defaults, then all the MPEG4IP executables will end up in /usr/local/bin.

For server side (or content creation) tools, you get:

| mp4live | Integrated, live encoding to file or network - Linux only |
| avi2raw | Extracts raw audio/video tracks from an AVI file |
| lboxcrop | Vertically crops raw video to a new aspect ratio |
| mp4venc | Encodes raw video into MPEG-4 encoded video using ISO codec - not available on all systems |
| mp4creator | Creates and hints audio/video tracks to an mp4 file. Can also be used to extract tracks and reconstruct AAC headers, and can encrypt to the ISMACryp specification. It will also assemble 3GPP files, but not hint them. |
| xvidenc | Encodes raw video into MPEG-4 encoded video using the Xvid codec. Not availble with xvid 1.0 (yet) |

These are described in more detail in doc/encoding.htm

A few debugging tools are also included:

|exec   |function|
|:-------|:--------|
| mp4dump | Utility to dump MP4 file meta-information in text form |
| mp4trackdump | Utility to dump MP4 file track information in text form |
| mp4info | Utility to display MP4 file summary |
| mp4videoinfo | Utility to dump information about MP4 file video tracks |
| avidump | Utility to display AVI file summary |
| yuvdump | Utility to display a raw video file on the screen |
| mpeg_ps_info | Utility to display streams in an mpeg program stream or vob file |
| mpeg_ps_extract | Utility to extract elementary streams in an mpeg program stream or vob file |

For playback, you get:

| gmp4player | Simple graphical interface player |
| mp4player | Bare video window with sync'ed audio |

[Back to top](#top)

<span id="executables"></span>
## Executable Notes

**FAAD**

We do not have the complete version of FAAD, so the faad standalone decoder will not build. FAAD as of faad2-2.0-rc3 contains a gmp4player plugin - we recommend download and using that. We've included the latest version of the plugin code in player/plugin/audio/faad2, if you have compile problems. Copy the files into the faad/plugins/mpeg4ip directory. You may have to remove the rpm rule in the top level Makefile to get it to make.

[Back to top](#top)

<span id="directory"></span>
## Directory Structure

If you're going to start hacking, a map of the territory may prove useful:  

**mpeg4ip** - top level project directory

**config** - autoconf files

**doc** - the minimal doc we've written so far

```
|-> encoding - how to encode content
|-> ietf - copies of the relevant RFC's
|-> mcast - how to multicast
|-> mp4v2 - man pages for mp4v2 library
|-> programs - man pages for core programs
```

**include** - project wide includes

**common** - shared code

```
|-> video
       |--> iso-mpeg4 - flattened directory version of ISO MPEG-4 video encoder/decoder
```

**lib** - project wide libraries

```
|-> SDLAudio - Simple DirectMedia Layer(modified audio only)
|-> audio - audio format conversions
|-> avi - AVI file format
|-> config_file - Configuration file utility
|-> gnu
       |--> getopt - gnu getopt routines
|-> ismacryp - API library for ISMACrypt functions
|-> mp4 - MP4 (aka MOV/Quicktime) file format library
|-> mp4v2 - new MP4 library written from scratch
       |--> test - contains some test programs
       |--> util - contains new mp4dump and mp4extract utilities
|-> mpeg2ps - mpeg program stream library/utilites
|-> mpeg2t - mpeg2 transport stream utilities
|-> msg_queue - SDL based Inter-thread messages utlity
|-> rtp - UCL RTP
|-> srtp - interface with libSRTP
|-> sdp - Our own SDP
|-> win32 - libary files need for MS Windows
```

**player** - player specific code

```
|-> lib - libraries specifically for the player
       |--> audio
             |---> celp - celp decoder
             |---> faad - FAAD AAC decoder
             |---> mp3util - MP3 utilities for RFC 3119
             |---> portaudio - MAC audio rendering library
       |--> libhttp - Our own http client
       |--> mp4util - MP4 audio config and sdp decoding library
       |--> rtsp - Our own RTSP client
|-> plugin - home of player plugins
       |--> audio - audio plugins
             |---> a52dec - liba52 decoder plugin
             |---> celp - celp decoder plugin
             |---> faad2 - code for faad2 audio plugin
             |---> g711 - g711 audio plugin
             |---> ffmpeg - ffmpeg audio plugin
             |---> raw - raw audio plugin
       |--> rtp - rtp bytestream plugins
             |---> h261 - h261 rtp plugin
             |---> h264 - h264 rtp plugin
             |---> isma_audio - isma audio format plugin
             |---> isma_href - isma HREF format plugin
             |---> isma_video - ISMACryp video RTP format plugin
             |---> rfc2429 - H.263 rfc2429 video RTP format plugin
             |---> rfc3267 - AMR audio RTP format plugin
       |--> text - text plugins
             |---> href - ISMA href deocoder plugin
             |---> plain - plain text deocoder plugin
       |--> video - video plugins
             |---> ffmpeg - ffmpeg video plugin
             |---> raw - raw video plugin
             |---> mpeg3 - mpeg1/2 video plugin
             |---> mpeg2dec - mpeg1/2 libmpeg2 video plugin
             |---> xvid - xvid video plugin
             |---> h261 - h261 decoder
             |---> mpeg2dec - libmpeg2 plugin
|-> src - the player executable
       |--> codec
             |---> aa - aac plugin
             |---> mp3 - mp3 plugin
             |---> mpeg4 - mpeg4 ISO decoder plugin
             |---> wav - wav plugin
       |--> win_common - common windows code
       |--> win_client - windows client
       |--> win_gui - windows gui code.
```

**server** - server specific code

```
|-> audio
       |--> faac - AAC encoder program (obsolete)
|-> mp4creator - create and hint A/V tracks to an mp4 file
|-> mp4live - mp4 live encoding interface
       |--> gui - gtk gui for mp4 live.
|-> util
       |--> avi2raw - extract raw A/V data from AVI files
       |--> avidump - dump AVI meta information.
       |--> lboxcrop - vertically crop raw video
       |--> mp4encode - front-end script to simplify encoding process
       |--> rgb2yuv - rgb to yuv converter
       |--> xvidenc - command line interface to Xvid MPEG-4 encoder
|-> video
       |--> H26L - ITU H.26L TML 9.4 reference video encoder (EXPERIMENTAL)
```

**util** - generally useful utilities

```
|-> h264 - displays (some) H.264 bitstream information
|-> iptv - read Cisco IP/TV programs from a content manager
|-> mpeg2 - dumps out frame info for mpeg2 elementary streams
|-> mpeg4vol - display mpeg-4 VOL
|-> yuv - simple tools for examining raw video
```

[Back to top](#top)

<span id="standards"></span>
## Standards Compliance

We're not only supporters of open source, we're supporters of open standards! We've attempted to use the publically defined standards as much as possible. Here's what we believe we are following. If you find something non-compliant, please let us know. We certainly will want to fix it.

Here are the citations:

* ISO/IEC 14496-1:2001 MPEG-4 Systems (includes MP4 file format)
* ISO/IEC 14496-2:2000 MPEG-4 Video
* ISO/IEC 14496-3:1999 MPEG-4 Audio (includes AAC)
* ISO/IEC 14496-10:2004 Advanced Video Codec
* ISO/IEC 14496-12:2004 ISO base media file format
* ISO/IEC 14496-14:2004 MP4 File Format
* ISO/IEC 14496-15:2004 AVC file format
* ISO/IEC 11172-3:1993 MPEG-1 Audio (includes MP3)
* ISO/IEC 13818-3:1998 MPEG-2 Audio (includes extensions to MP3)
* ISO/IEC 13818-7:1997 MPEG-2 AAC

The ISO/IEC documents must be purchased from either ISO ([www.iso.ch](http://www.iso.ch)) or one of the national bodies. In the US, ANSI is the representative body, and provides an online store under [www.ansi.org](http://www.ansi.org)

|RFC    |description|
|:-------|:--------|
| IETF RFC 3550 | RTP (obsoletes RFC 1889) |
| IETF RFC 2326 | RTSP |
| IETF RFC 2327 | SDP |
| IETF RFC 2250 | RTP Payload for MPEG-1/2|
| IETF RFC 3016 | RTP Payload for MPEG-4 Audio/Visual <br>Note: we're implementing the MPEG-4 video part
| IETF RFC 3119 | A More Loss-Tolerant RTP Payload Format for MP3 Audio |
| IETF RFC 3267 | RTP Payload for Tranport and <br>File Storage Format for the Adaptive Multi-Rate (AMR) and <br>Adaptive Multi-Rate Wideband (AMR-WB) Audio Codecs
| IETF RFC 3551 | RTP Profile for Audio and Video Conferences with Minimal Control
| IETF RFC 3640 | RTP Payload for Tranport of MPEG-4 Elementary Streams <br>_Note: we're implementing the AAC and Celp audio part.
| IETF RFC 3984 | RTP Payload Format for H.264 Video

The IETF RFCs can be found in doc/ietf, or online at the [ietf](http://ietf.org/rfc)

The MP4 file format is derived from Apple's QuickTime file format. That specification is Apple Computer QuickTime File Format, June 28 2000 and is available through the [QT developer site](http://developer.apple.com/quicktime/).

We're also involved in the [Internet Streaming Media Alliance](http://www.isma.tv) (ISMA) which seeks to standardize the protocols and formats used for streaming. We believe MPEG4IP is interoperable with that organization's 1.0 and 2.0 Technical Specification. For more information see [http://www.isma.tv](http://www.isma.tv)

[Back to top](#top)

<span id="latency"></span>
## Latency in mpeg4ip

There have been enough questions to merit a place in the README about the latency in mpeg4ip.

First of all, mpeg4ip is not designed for video conferencing or real time display of data. Most streaming products are not (look at Real or Quicktime's buffering schemes - QT buffers 3.0 seconds of data, while Real can buffer up to 30-40 seconds).

When you need to look at latency in a streaming environment, you need to look at each potential piece. In the simplest case (mp4live to mp4player), there are 5 potential places where latency can take place - at mp4live, in the kernel IP transmit stack, in the network, in the kernel IP receive stack and in the player.

Mp4live has very little latency. There is no latency from when the video frame is encoded with mpeg4\. With MPEG-2, the B frames require a 2 frame latency period. In audio, mpeg style codecs usually require 2 or 3 frames to begin generating output. (if using AAC, frames tend to be 1024 samples. MP3 tends to be 1152 samples, but can change based on sampling frequency and bit rate. Samples can be converted to seconds by dividing by the sample rate).

You can remove some of this latency by not bundling multiple frames in an RTP packet. This can be set using the rtpMaxFramesPerPacket in the Audio Profile configuration file in mp4live. You can also use G.711 or AMR codecs.

The kernel IP stacks can have some latency built in, as well. Probably not too much, but you should be aware that it can exist (look at a sniffer trace of DSS output and you'll see what I mean).

Network latency can occur, as well. Collisions, etc, can happen, especially if server and player are not on the same network.

If you're using a streaming server as a relay, latency will be introduced there, as well.

Finally, the player has latency as well. For streaming, we tend to buffer 2 seconds for each stream. This is changeable by setting the RtpBufferTimeMsec variable in the .gmp4player_rc file (for Windows users, you'll have to change the registry to do so). The value is in milliseconds, and you need to set it to a non-zero value (0 indicates the default value).

[Back to top](#top)

<span id="player"></span>
## Player Information

See the [PLAYER_README.html](prm.html) in doc.

[Back to top](#top)

<span id="problems"></span>
## Known Problems

*   Encoded previews in mp4live do not work with xvidcore releases.
*   Encoded previews for mpeg2 with ffmpeg will look very weird - this is due to results returned with B frames.
*   If you have a large (greater than 2G) that does not play locally with Quicktime, recreate it using 1.0RC3 or later. Quicktime does not correctly support 64 bit time atoms in a mp4 file.
*   We do not support gcc 2.95.3\. There is a problem that makes it think that when a long is defined as an int, and time_t is defined as an int, that `%ld` should not be the correct format. You can try to fix these by removing the l when you get the warning.
*   There have been some changes to the MPEG4 AAC DTS headers in the latest version of the MPEG4 specification. This renders previous .aac files pretty much obsolete. We have made this change to mpeg4ip as of the 0.9.9 version. If you need to use older aac files, you can use the `--aac-old-file-format` parameter to mp4creator. If you need to generate new aac files, you can use the `mp4creator --aac-old-file-format`, then extract the track with the `mp4creator --extract` command.
*   If you're playing through a NAT box, you may have to specify the default client IP ports. Use the command `RtpIpPortMin=<port>` and `RtpIpPortMax=<port>` in the .gmp4player_rc file created in your home directory. The IETF recommends a range of 6970 to 6999.
*   The ISO MPEG-4 video codec bitrate control feature is broken. We've heard that a fix is available, so we're trying to track that down.
*   The player for windows is sketchy at best. We have included project files that should build for Dev Studio 6.0\. We recommend installing DirectX 8.1 or later. If that is not possible, and you have problems with video, try uncommenting out `#define OLD_SURFACE` in player/src/video.cpp.
*   If you have problems with choppy playback, and you're using Windows 98, forget about it. The timer tick time is too slow (55 msec) for us to use effectively. Try Quicktime or Real with Envivio, or update to a newer Windows OS.
*   FreeBSD based OS's have a problem with thread delays. This can cause the player to skip rendering many frames. If you have this problem update to the latest version of FreeBSD - the problem still can occur, but is reduced.
    This is due to an error in the thread scheduling code that causes a problem with a delay of less than the thread scheduler quantum (200 msec in some versions, 20 in others). Since the average delay used is 9 to 10 msec (less as we get closer to the video rendering time), this can have a great effect on video playback.
    You can get around this error by rebuilding your libc, after changing the `THREAD_SCHED_USECS` to 20000 (or lower) from 200000 in thread_private.h.
*   If you're running on Linux, and trying to play a raw audio file, and notice that you get garbage, try setting the `LimitAudioSdlBuffer` config variable to 1 in the .gmp4player_rc file. This seems to occur on a Soundblaster Live, Red Hat 7.3 machine.
*   As the RTP standard (or new draft) suggests, the round trip delay can be approximated at the sender side by subtracting the intercepted time of a RTCP SR and the delay at the client side from the intercepted time of the corresponding RTCP RR. That is:
    RRT = Time_RR - Time_SR - Time_delay_at_client.
    However, as the Windows run-time library only provides up to 1 millisecond resolution (0.001 sec), such measurement on Windows machines on a LAN may be very inaccurate.
    A typical RRT between two hosts on a LAN is around 0.5 millisecond. Measurements between Linux/Unix boxes have no such a problem.
*   It appears that mpeg audio created and hinted with our mp4creator do not work correctly with Quicktime when streaming. This is most likely because we use a dynamic RTP payload number and a RTP TS frequency that matches the sampling frequency. QT seems to only like an RTP payload of 14 and a RTP TS frequency of 90000\. (This is with QT 6.0). This will work with mpeg 1/2 files, or if you use `-timescale=90000` when using mp4creator with mp3 files. If you're using mp4live to stream audio, use the `rtpUseMp4RtpPayload14` config file option.
    Neither QT or Real appear to understand mpeg1/2 or mp3 content in a .mp4 container file for local playback.
*   mp4live will not work with an audio only stream that needs to take its output from a TV tuner - we will not enable the TV tuner output.
    To make this work, try issuing the following commands before running mp4live:
    `v4lctl -c <video device> mute off`
    `v4lctl -c <video device> volume 100%`
    
<span id="contrib"></span>
## Contributing Back

We welcome all contributions back to mpeg4ip. Email is the best way to accomplish this.

Please do a `diff -c -w` on each file you've changed against a fresh tarball that you've worked on, or better yet, CVS. Please refrain from changing any generated file (such as configure, ltmain.sh, any Makefile or Makefile.in) - instead, please help figure out how to change the configure.in scripts.

Please understand that we are trying to keep this a multi-platform project, so compiling on multiple platforms is very much welcome.

You can use the forums or mailing list before you start to work out how to do a certain project.

[Back to top](#top)