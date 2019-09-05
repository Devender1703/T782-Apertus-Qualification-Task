# Apertus Qualification Task

The applications process a RAW12 sample file contained in **sample** directory. The code extract grayscale channels from sample.raw12 
in the form of Red , Green and blue channels. The final image is debayered using Bilinear interpolation. All the images are stored into .ppm 
format and grayscale in .pgm. 

The code also writes a .avi file which inculdes n frames of final debayered image. The output file **sample.avi** can be opened in 
video players (Mplayers or vlc player)

### Required Tools 

For building purpose , The code uses cmake tools (preferred version if 3.5.0 or below)

installation command for linux 

```
sudo apt -y install cmake
```
```
cmake --version
```

For running sample.avi , we need advance video editing tool e.g VLC

### Building  instructions 

```
git clone ..
cd to cloned repo directory
cmake /home/user/repo-master/ 
make 
./Challenge width height framecount pathOFfile
```

Note : pass path of cmakeLists.txt for cmake.


