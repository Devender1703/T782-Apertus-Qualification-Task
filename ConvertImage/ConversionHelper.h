/* Copyright (C) 2019 Devendra Shaktawat
 *
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#ifndef HEADER_H
#define HEADER_H

#include <string>

class ConversionToImage
{
   int width;
   int height;
   int graywidth;
   int grayheight;    

   public: 

   uint8_t *final_image;
   uint8_t *red;
   uint8_t *green1;
   uint8_t *green2;
   uint8_t *blue;

   ConversionToImage(int _width, int _height)
   {
     width = _width;
     height = _height;
     graywidth = _width/2;
     grayheight = _height/2;
   }
 
   void ConvertlayerToPgm(uint8_t *, std::string);
 
   void ConvertlayerToPpm(std::string);

   void ExtractChannels(uint8_t *);

   void BilinearInterpolation(uint8_t *);

   void PrintPixels(uint8_t *, std::string);
};

#endif
