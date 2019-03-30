/* Copyright (C) 2019 Devendra Shaktawat
 *
 * Anyone can use, distribute and modify this code under the
 * terms of the MIT license.
 */

#include "AVIStruct.h"

#define AVISize                 sizeof(List) - 2 * sizeof(uint32_t) + FindSize("hdrl") + FindSize("movi")
#define hdrlSize                sizeof(List) - 2 * sizeof(uint32_t) + FindSize("avih") + FindSize("strl")      
#define strlSize                sizeof(List) - 2 * sizeof(uint32_t) + FindSize("strh") + FindSize("strf")            
#define AVIStreamHeaderSize     sizeof(AVIStreamHeader)
#define BitmapInfoHeaderSize    sizeof(Chunk) - sizeof(uint32_t) + sizeof(BitmapInfoHeader)
#define AVIMainHeaderSize       sizeof(AVIMainHeader)
#define MoviSize                (sizeof(List) - 2 * sizeof(uint32_t) + framecount * sizeof(Chunk) + value * 3)


uint32_t MakeFourCC(std::string type)
{

   uint32_t fourCC = (uint32_t)type[3] << 24 |
                     (uint32_t)type[2] << 16 |
                     (uint32_t)type[1] << 8 |
                     (uint32_t)type[0];

   return fourCC;

}

unsigned long int AviCreator::FindSize(std::string type)
{
   
   if(type == "avi ")
   {
      header_map["avi "] = AVISize; 
      return (header_map["avi "] + 2 * sizeof(uint32_t));
   }
   
   if(type == "hdrl")
   {
      header_map["hdrl"] = hdrlSize;
      return ( header_map["hdrl"] + 2 * sizeof(uint32_t));
   }
    
   if(type == "strl")
   {
      header_map["strl"] = strlSize;                                 
      return (header_map["strl"]  + 2 * sizeof(uint32_t));
   }
    
   if(type == "strh") 
   {
      header_map["strh"] = AVIStreamHeaderSize;
      return header_map["strh"]; 
   }
  
   if(type == "strf")
   {
      header_map["strf"] =  BitmapInfoHeaderSize;
      return header_map["strf"] + sizeof(uint32_t);
   }
  
   if(type == "avih")
   {
      header_map["avih"] = AVIMainHeaderSize;
      return header_map["avih"];
   }
  
   if(type == "movi")
   {
       unsigned long int value = framecount * height * width;
       header_map["movi"] = MoviSize;
       return header_map["movi"] + 2 * sizeof(uint32_t);
   }

   return 0;
}  

void AviCreator::AddNode(AviCreator Avi, std::string type, int &offset, uint8_t *buffer)
{
   
   if(type == "avi ")
   {
      Avi.MakeRiffHeader(offset, buffer); 
      AddNode(Avi, "hdrl", offset, buffer);
      AddNode(Avi, "movi", offset, buffer);
   }
   
   if(type == "hdrl")
   {
      Avi.MakeHdrlHeader(offset, buffer);
      AddNode(Avi, "avih", offset, buffer);
      AddNode(Avi, "strl", offset, buffer);
   }
    
   if(type == "strl")
   {
      Avi.MakeStrlHeader(offset,  buffer);
      AddNode(Avi, "strh", offset, buffer);
      AddNode(Avi, "strf", offset, buffer);                              
   }
    
   if(type == "strh") 
   {
      Avi.MakeStrhHeader(offset, buffer);
   }
  
   if(type == "strf")
   {
      Avi.MakeStrfHeader(offset, buffer); 
      Avi.MakeBitHeader(offset, buffer);  
   }
  
   if(type == "avih")
   {
      Avi.MakeAvihHeader(offset, buffer);
   }
  
   if(type == "movi")
   {
      Avi.MakeMoviHeader(offset, buffer);
   }
   
}  

