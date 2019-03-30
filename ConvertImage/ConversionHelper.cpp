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
   long long int evenOdd = 1, count1 = 0, count2 = 0;
   
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
   long long evenOdd = 1;
   
   final_image = new uint8_t[width * height * 3];
   
   for (long long count = 0; count < width * height; count += 2)
   {
      if (count >= width * evenOdd)
      evenOdd++;

      if (evenOdd % 2 != 0)
      {
        final_image[count * 3]     = buffer[count];
        final_image[count * 3 + 1] = buffer[count + 1];
        final_image[count * 3 + 2] = buffer[count + 4097];
        final_image[count * 3 + 3] = buffer[count];
        final_image[count * 3 + 4] = buffer[count + 1];
        final_image[count * 3 + 5] = buffer[count + 4097];
      }
      else
      {
        final_image[count * 3]     = buffer[count - 4096];
        final_image[count * 3 + 1] = buffer[count];
        final_image[count * 3 + 2] = buffer[count + 1];
        final_image[count * 3 + 3] = buffer[count - 4096];
        final_image[count * 3 + 4] = buffer[count];
        final_image[count * 3 + 5] = buffer[count + 1];
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

void ConversionToImage::ConvertlayerToPgm(uint8_t *GrayScale, std::string ChannelName)
{
   std::ofstream file;

   file.open(ChannelName, std::ios::binary);

   file << "P5" << std::endl;
   file << graywidth << std::endl;
   file << grayheight << std::endl;
   file << "255" << std::endl;

   file.write(reinterpret_cast<char *>(GrayScale), graywidth * grayheight);

   std::cout << ChannelName << ".pgm is ready" << std::endl;
}

void ConversionToImage::ConvertlayerToPpm(std::string ChannelName)
{
   std::ofstream file;

   file.open(ChannelName, std::ios::binary);

   file << "P6" << std::endl;
   file << width << std::endl;
   file << height << std::endl;
   file << "255" << std::endl;
   file.write(reinterpret_cast<char *>(final_image), width * height * 3);

   std::cout << ChannelName << ".ppm is ready" << std::endl;
}

