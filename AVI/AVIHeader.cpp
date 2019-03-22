/* Copyright (C) 2019 Devendra Shaktawat
 *
 * Anyone can use, distribute and modify this code under the
 * terms of the MIT license.
 */

#include "AVIStruct.h"

using namespace std;

List AviCreator::MakeRiffHeader()
{
   List riff;

   riff.FourCC = MakeFourCC("RIFF");
   riff.FileSize = header_map["avi "];
   riff.FileType = MakeFourCC("AVI ");

   return riff;   

}

List AviCreator::MakeHdrlHeader()
{
   List hdrl;

   hdrl.FourCC = MakeFourCC("LIST");
   hdrl.FileSize = header_map["hdrl"];
   hdrl.FileType = MakeFourCC("hdrl");

   return hdrl;

}

AVIMainHeader AviCreator::MakeAvihHeader()
{
   AVIMainHeader avih;
   
   avih.fcc = MakeFourCC("avih");
   avih.cb = header_map["avih"] - 8;
   avih.dwMicroSecPerFrame = 100;
   avih.dwMaxBytesPerSec = 1000;
   avih.dwPaddingGranularity = 0;
   avih.dwFlags = 1;
   avih.dwTotalFrames = framecount;  
   avih.dwInitialFrames = 0;
   avih.dwStreams = 1;
   avih.dwSuggestedBufferSize = 100000;
   avih.dwWidth = width;
   avih.dwHeight = height;
   avih.dwReserved[0] = 0;
   avih.dwReserved[1] = 0;
   avih.dwReserved[2] = 0;
   avih.dwReserved[3] = 0;

   return avih;

}

List AviCreator::MakeStrlHeader()
{
   List strl;
   
   strl.FourCC = MakeFourCC("LIST");
   strl.FileSize = header_map["strl"];
   strl.FileType = MakeFourCC("strl");

   return strl;

}

AVIStreamHeader AviCreator::MakeStrhHeader()
{
   AVIStreamHeader strh;
  
   strh.FourCCType = MakeFourCC("strh");
   strh.cb = header_map["strh"] - 8;
   strh.fccType = MakeFourCC("vids");
   strh.fccHandler = 0;
   strh.Flags = 0;
   strh.Priority = 0;
   strh.Language = 0;
   strh.InitialFrames = 0;
   strh.Scale = 1;
   strh.Rate = 30;
   strh.Start = 0;
   strh.Length = strh.Rate / strh.Scale;
   strh.SuggestedBufferSize = 10000000;
   strh.Quality = 0;
   strh.SampleSize = 120;

   strh.rcFrame.left = 0;
   strh.rcFrame.right = width;
   strh.rcFrame.top = 0;
   strh.rcFrame.bottom = height;

   return strh;   

} 

Chunk AviCreator::MakeStrfHeader()
{ 
   Chunk strf;  
   
   strf.Ckid = MakeFourCC("strf");
   strf.CkSize = header_map["strf"];
   
   return strf;
}

BitmapInfoHeader AviCreator::MakeBitHeader()
{
   BitmapInfoHeader vidf;
  
   vidf.Size = sizeof(BitmapInfoHeader);
   vidf.Width = width;
   vidf.Height = height;
   vidf.Planes = 1;
   vidf.BitCount = 24;
   vidf.Compression = 0;
   vidf.SizeImage = 0;
   vidf.XPelsPerMeter = 2835;
   vidf.YPelsPerMeter = 2835;
   vidf.ClrUsed = 0;
   vidf.ClrImportant = 0;

   return vidf;

}

List AviCreator::MakeMoviHeader()
{
   List movi;

   movi.FourCC = MakeFourCC("LIST");
   movi.FileSize = header_map["movi"];
   movi.FileType = MakeFourCC("movi");

   return movi;

}

void MakeAVIFile(uint8_t* frame, int _width, int _height, int _framecount)
{     
   AviCreator Avi(_height, _width, _framecount);
   Avi.FindSize("avi ");

   std::ofstream avi;
   avi.open("sample.avi", ios::binary);

   if (!avi)
   {
     std::cout << "AVI file not opening" << endl;
     return;
   }

   long long int size = 0;

   size = sizeof(List)               // RIFF
          + sizeof(List)             // hdrl
          + sizeof(AVIMainHeader)    // avih
          + sizeof(List)             // strl
          + sizeof(AVIStreamHeader)  // strh
          + sizeof(Chunk)            // strf
          + sizeof(BitmapInfoHeader) // bitmapinfoheader
          + sizeof(List);            // movi;

   uint8_t *AVIFile = new uint8_t[size + 100];   // add 10  to case of segmentation fault  

   List riff = Avi.MakeRiffHeader();
   List hdrl = Avi.MakeHdrlHeader();
   AVIMainHeader avih = Avi.MakeAvihHeader();
   List strl = Avi.MakeStrlHeader();
   AVIStreamHeader strh = Avi.MakeStrhHeader(); 
   Chunk strf = Avi.MakeStrfHeader(); 
   BitmapInfoHeader bit = Avi.MakeBitHeader();
   List movi = Avi.MakeMoviHeader();

   long long int length1 = 0;

   std::memcpy(&AVIFile[length1], (const void *)&riff, sizeof(List));
   length1 += sizeof(List);

   std::memcpy(&AVIFile[length1], (const void *)&hdrl, sizeof(List));
   length1 += sizeof(List);

   std::memcpy(&AVIFile[length1], (const void *)&avih, sizeof(AVIMainHeader));
   length1 += sizeof(AVIMainHeader);

   std::memcpy(&AVIFile[length1], (const void *)&strl, sizeof(List));
   length1 += sizeof(List);

   std::memcpy(&AVIFile[length1], (const void *)&strh, sizeof(AVIStreamHeader));
   length1 += sizeof(AVIStreamHeader);
   
   std::memcpy(&AVIFile[length1], (const void *)&strf, sizeof(Chunk));
   length1 += sizeof(Chunk);

   std::memcpy(&AVIFile[length1], (const void *)&bit, sizeof(BitmapInfoHeader));
   length1 += sizeof(BitmapInfoHeader);

   std::memcpy(&AVIFile[length1], (const void *)&movi, sizeof(List));
   length1 += sizeof(List);

   avi.write(reinterpret_cast<char *>(AVIFile), length1);

   Chunk vidf1;
   vidf1.Ckid = MakeFourCC("00db");
   vidf1.CkSize = _width * _height * 3;

   for (int count = 0; count < _framecount; count++)
   {
      avi.write(reinterpret_cast<char *>(&vidf1), sizeof(vidf1));        //  writing video data (chunk)
      avi.write(reinterpret_cast<char *>(frame), _width * _height * 3); //  actual frame
   }

   avi.close();

   delete[] AVIFile;
   delete[] frame;

   cout << "The avi file is ready" << endl;    
   
   
}

