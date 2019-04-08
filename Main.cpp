/* Copyright (C) 2019 Devendra Shaktawat
 *
 * Anyone can use, distribute and modify this code under the
 * terms of the MIT license.
 */

#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include "ConvertImage/ConversionHelper.h"
#include "AVI/AVIStruct.h"

int main(int argc, char* argv[])
{  
   if(argc != 5)
   {
     std::cout << "ERROR : Argument missing" <<std::endl;
     return 0;
   }   

   std::ifstream raw;
   raw.open(argv[4], std::ios::binary);
    
   if (!raw)
   {
     std::cout << "ERROR: file is not opening" << std::endl;
     return 0;
   } 
     
   int width      = atoi(argv[1]);
   int height     = atoi(argv[2]);
   int frameCount = atoi(argv[3]); 
 
   raw.seekg(0, std::ios::end);
   long long int length = raw.tellg();
   raw.seekg(0, std::ios::beg);
       
   uint8_t *buffer = new uint8_t[length];
   raw.read((char *)buffer, length);
   
   uint8_t *mainBuffer = new uint8_t[width * height];
  
   int count1= 0;
   for (int count= 0; count < length; count += 3)
   {
     mainBuffer[count1]     = buffer[count];
     mainBuffer[count1 + 1] = ((buffer[count + 1] & (0x0F)) << 4 | (buffer[count + 2] >> 4));
     count1 += 2;
   }

   raw.close();
    
   ConversionToImage image(width, height); 
  
   image.ExtractChannels(mainBuffer);
   
   image.PrintPixels(image.red, "Red");   
   image.PrintPixels(image.green1, "green1");
   image.PrintPixels(image.green2, "green2");
   image.PrintPixels(image.blue, "blue");

   image.ConvertlayerToImage(false, image.red, "Red.pgm");
   image.ConvertlayerToImage(false, image.green1, "Green1.pgm");
   image.ConvertlayerToImage(false, image.green2, "Green2.pgm");
   image.ConvertlayerToImage(false, image.blue, "Blue.pgm");

   image.BilinearInterpolation(mainBuffer);
   image.ConvertlayerToImage(true, image.debayeredChannel, "DebayeredImage.ppm");
 
   MakeAVIFile(image.debayeredChannel, width, height, frameCount);

   return 0;
}
