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
     
   int width  = atoi(argv[1]);
   int height = atoi(argv[2]);
   int framecount = atoi(argv[3]); 
 
   raw.seekg(0, std::ios::end);
   long long int length = raw.tellg();
   raw.seekg(0, std::ios::beg);
       
   uint8_t *buffer = new uint8_t[length];
   raw.read((char *)buffer, length);
    
   uint8_t *mainbuffer = new uint8_t[width * height];
    
   int count1= 0;
   for (int count= 0; count < length; count += 3)
   {
     mainbuffer[count1] = buffer[count];
     mainbuffer[count1 + 1] = ((buffer[count + 1] & (0x0F)) << 4 | (buffer[count + 2] >> 4));
     count1+= 2;
   }

   raw.close();
    
   ConversionToImage Image(width, height); 
  
   Image.ExtractChannels(mainbuffer);
   
   Image.PrintPixels(Image.red, "Red");   
   Image.PrintPixels(Image.green1, "green1");
   Image.PrintPixels(Image.green2, "green2");
   Image.PrintPixels(Image.blue, "blue");

   Image.ConvertlayerToPgm(Image.red, "Red");
   Image.ConvertlayerToPgm(Image.green1, "Green1");
   Image.ConvertlayerToPgm(Image.green2, "Green2");
   Image.ConvertlayerToPgm(Image.blue, "Blue");

   Image.BilinearInterpolation(mainbuffer);
   Image.ConvertlayerToPpm("Final_image");
 
   MakeAVIFile(Image.final_image, width, height, framecount);

   return 0;
}
