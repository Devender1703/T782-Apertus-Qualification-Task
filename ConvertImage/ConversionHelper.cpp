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
   
   red = new uint8_t[graywidth * grayheight]; 
   green1 = new uint8_t[graywidth * grayheight]; 
   green2 = new uint8_t[graywidth * grayheight]; 
   blue = new uint8_t[graywidth * grayheight]; 
     
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
   debayeredchannel = new uint8_t[width * height * 3];
   
   for (long long count = 0; count < width * height; count += 2)
   {
      if (count >= width * evenOdd)
      evenOdd++;
 
      long triple = count * 3; 

      if (evenOdd % 2 != 0)
      {
        debayeredchannel[triple]     = buffer[count];
        debayeredchannel[triple + 1] = buffer[count + 1];
        debayeredchannel[triple + 2] = buffer[count + 4097];
        debayeredchannel[triple + 3] = buffer[count];
        debayeredchannel[triple + 4] = buffer[count + 1];
        debayeredchannel[triple + 5] = buffer[count + 4097];
      }
      else
      {
        debayeredchannel[triple]     = buffer[count - 4096];
        debayeredchannel[triple + 1] = buffer[count];
        debayeredchannel[triple + 2] = buffer[count + 1];
        debayeredchannel[triple + 3] = buffer[count - 4096];
        debayeredchannel[triple + 4] = buffer[count];
        debayeredchannel[triple + 5] = buffer[count + 1];
      }
   }
}

void ConversionToImage::PrintPixels(uint8_t *tile, std::string channelname)
{
   std::cout << "5*5 pixels for" << channelname <<" "<<"channel" << std::endl;

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

void ConversionToImage::ConvertlayerToImage(bool isdebayered, uint8_t *data, std::string ChannelName)
{
   std::ofstream file;

   file.open(ChannelName, std::ios::binary);
   
   if(!file)
   {
      std::cout << "Image file not opening" << std::endl;
      return;
   }

   file << (isdebayered ? "P6" : "P5");
   file << std::endl;
   file << (isdebayered ? width : graywidth);
   file << std::endl;
   file << (isdebayered ? height : grayheight);
   file << std::endl;
   file << "255" << std::endl;
   
   int size_to_write; 
   (isdebayered ? size_to_write = width * height * 3 : size_to_write = graywidth * grayheight);
      
   file.write(reinterpret_cast<char*>(data), size_to_write);
   std::cout << ChannelName << ".pgm is ready" << std::endl;
}

