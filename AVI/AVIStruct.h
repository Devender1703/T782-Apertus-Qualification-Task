/* Copyright (C) 2019 Devendra Shaktawat
 *
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#ifndef AVI_H
#define AVI_H

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <cstring>

struct List
{
   uint32_t FourCC;
   uint32_t FileSize;
   uint32_t FileType;
};

struct Chunk
{
   uint32_t Ckid;
   uint32_t CkSize;
};

struct AVIMainHeader
{
   uint32_t fcc;
   uint32_t cb;
   uint32_t dwMicroSecPerFrame;
   uint32_t dwMaxBytesPerSec;
   uint32_t dwPaddingGranularity;
   uint32_t dwFlags;
   uint32_t dwTotalFrames;
   uint32_t dwInitialFrames;
   uint32_t dwStreams;
   uint32_t dwSuggestedBufferSize;
   uint32_t dwWidth;
   uint32_t dwHeight;
   uint32_t dwReserved[4];
};

struct AVIStreamHeader
{
   uint32_t FourCCType;
   uint32_t cb;
   uint32_t fccType; 
   uint32_t fccHandler;
   uint32_t Flags;
   uint16_t Priority;
   uint16_t Language;
   uint32_t InitialFrames;
   uint32_t Scale;
   uint32_t Rate;
   uint32_t Start;
   uint32_t Length;
   uint32_t SuggestedBufferSize;
   uint32_t Quality;
   uint32_t SampleSize;

   struct
   {
      uint8_t left;
      uint8_t top;
      uint8_t right;
      uint8_t bottom;
   } rcFrame;
};

struct BitmapInfoHeader
{     
   uint32_t Size;
   uint32_t Width;
   uint32_t Height;
   uint16_t Planes;
   uint16_t BitCount;
   uint32_t Compression;
   uint32_t SizeImage;
   uint32_t XPelsPerMeter;
   uint32_t YPelsPerMeter;
   uint32_t ClrUsed;
   uint32_t ClrImportant;
};

class AviCreator
{
   int height;
   int width;
   int frameCount;
   
public:

   AviCreator(int _height, int _width, int _frameCount)
   {  
      height     = _height;
      width      = _width;    
      frameCount = _frameCount;
   }
   
   std::unordered_map<std::string, unsigned long int> headerMap;

   unsigned long int FindSize(std::string);
   
   void AddNode(AviCreator, std::string, int&, uint8_t*);

   void MakeRiffHeader(int&, uint8_t*);

   void MakeHdrlHeader(int&, uint8_t*);

   void MakeAvihHeader(int&, uint8_t*);

   void MakeStrlHeader(int&, uint8_t*);

   void MakeStrhHeader(int&, uint8_t*); 
   
   void MakeStrfHeader(int&, uint8_t*);
   
   void MakeBitHeader(int&, uint8_t*);

   void MakeMoviHeader(int&, uint8_t*);

};

   uint32_t MakeFourCC(std::string);
   void MakeAVIFile(uint8_t*, int , int ,int);

#endif
