/* Copyright (C) 2019 Devendra Shaktawat
 *
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#include <fstream>
#include <iostream>
#include <cstring>
#include "ConversionHelper.h"

void ConversionToImage::ExtractChannels(uint8_t *buffer)
{
   long evenOdd = 1, count1 = 0, count2 = 0;
   
   red = new uint8_t[grayWidth * grayHeight]; 
   green1 = new uint8_t[grayWidth * grayHeight]; 
   green2 = new uint8_t[grayWidth * grayHeight]; 
   blue = new uint8_t[grayWidth * grayHeight]; 
     
   for (int count = 0; count < width * height; count += 2)
   {
      if (count >= width * evenOdd)
      evenOdd++;

      if (evenOdd % 2 != 0)
      {
         red[count1]    = buffer[count];
         green1[count1] = buffer[count + 1];
         count1++;
      }
      else
      {
         blue[count2]   = buffer[count];
         green2[count2] = buffer[count + 1];
         count2++;
      }
   }
}

void ConversionToImage::BilinearInterpolation(uint8_t *buffer)
{
   long evenOdd = 1;
   debayeredChannel = new uint8_t[width * height * 3];
   
   for (long long count = 0; count < width * height; count += 2)
   {
      if (count >= width * evenOdd)
      evenOdd++;
 
      long triple = count * 3; 

      if (evenOdd % 2 != 0)
      {
        debayeredChannel[triple]     = buffer[count];
        debayeredChannel[triple + 1] = buffer[count + 1];
        debayeredChannel[triple + 2] = buffer[count + 4097];
        debayeredChannel[triple + 3] = buffer[count];
        debayeredChannel[triple + 4] = buffer[count + 1];
        debayeredChannel[triple + 5] = buffer[count + 4097];
      }
      else
      {
        debayeredChannel[triple]     = buffer[count - 4096];
        debayeredChannel[triple + 1] = buffer[count];
        debayeredChannel[triple + 2] = buffer[count + 1];
        debayeredChannel[triple + 3] = buffer[count - 4096];
        debayeredChannel[triple + 4] = buffer[count];
        debayeredChannel[triple + 5] = buffer[count + 1];
      }
   }
}

void ConversionToImage::PrintPixels(uint8_t *tile, std::string channelName)
{
   std::cout << "5*5 pixels for" << channelName <<" "<<"channel" << std::endl;
   

   for (int count = 0; count < 5; count++)
   {
      printf("%x ", tile[width * count + 0]);
      printf("%x ", tile[width * count + 1]);
      printf("%x ", tile[width * count + 2]);
      printf("%x ", tile[width * count + 3]);
      printf("%x ", tile[width * count + 4]);

      std::cout << std::endl;
   }
}

void ConversionToImage::ConvertlayerToImage(bool isDebayered, uint8_t *data, std::string channelName)
{
   std::ofstream file;

   file.open(channelName, std::ios::binary);
   
   if(!file)
   {
      std::cout << "Image file not opening" << std::endl;
      return;
   }

   file << (isDebayered ? "P6" : "P5") << std::endl;
   file << (isDebayered ? width : grayWidth) << std::endl;
   file << (isDebayered ? height : grayHeight) << std::endl;
   file << "255" << std::endl;
   
   int sizeToWrite; 
   (isDebayered ? sizeToWrite = width * height * 3 : sizeToWrite = grayWidth * grayHeight);
      
   file.write(reinterpret_cast<char*>(data), sizeToWrite);
   std::cout << channelName << " is ready" << std::endl;
}

