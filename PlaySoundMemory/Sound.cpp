#include "stdafx.h"
#include "Sound.h"

#include <windows.h>
#include <mmsystem.h>
#include <fstream>

Sound::Sound( const std::string& fileName, const double& volume )
   : _Volume( volume )
{
   _FileBuffer = 0;
   _status = false;

   if( !LoadFromFile( fileName ) )
      return;

   if( !ParseFileBuffer() )
      return;

   _Data = (short*)( _FileBuffer + 44 );
   _status = true;
}

Sound::~Sound()
{
   PlaySound( NULL, 0, 0 );
   if( _FileBuffer )
   {
      delete[] _FileBuffer;
   }
}

void Sound::Play( bool asynchro )
{
   if( !_status )
      return;

   asynchro ? PlaySound( (LPCWSTR)_FileBuffer, 0, SND_MEMORY | SND_ASYNC ) : PlaySound( (LPCWSTR)_FileBuffer, 0, SND_MEMORY );

   ApplyVolume();

   asynchro ? PlaySound( (LPCWSTR)_FileBuffer, 0, SND_MEMORY | SND_ASYNC ) : PlaySound( (LPCWSTR)_FileBuffer, 0, SND_MEMORY );

}

bool Sound::LoadFromFile( const std::string& fileName )
{
   std::ifstream infile( fileName.c_str(), std::ios::binary );

   if( !infile )
      return false;

   infile.seekg( 0, std::ios::end );
   _FileLength = (size_t) infile.tellg();

   _FileBuffer = new char[_FileLength];
   memset( _FileBuffer, 0, _FileLength );

   infile.seekg( 0, std::ios::beg );
   infile.read( _FileBuffer, _FileLength );

   infile.close();
   return true;
}

bool Sound::ParseFileBuffer()
{
   std::string tempStr( _FileBuffer, 4 );
   if( tempStr.compare( "RIFF" ) != 0 )
      return false;

   // Validate the size of the data (Chunk size) specified is not bigger than the actual size of the file.
   uint32_t tempFourByte = *(uint32_t*)( _FileBuffer + 4 );
   if( tempFourByte + 8 > _FileLength )
      return false;

   tempStr = std::string( _FileBuffer + 8, 4 );
   if( tempStr.compare( "WAVE" ) != 0 )
      return false;

   tempStr = std::string( _FileBuffer + 12, 4 );
   if( tempStr.compare( "fmt " ) != 0 )
      return false;

   // Validate the subChucnk is 16 for fun
   tempFourByte = *(uint32_t*)( _FileBuffer + 16 );
   if( tempFourByte != 16 )
      return false;

   // Validate if Audio Format is PCM
   uint16_t tempTwoBytes = *(uint16_t*)( _FileBuffer + 20 );
   if( tempTwoBytes != 1 )
      return false;

   // Validate if bits per sample is 16 bit
   tempTwoBytes = *(uint16_t*)( _FileBuffer + 34 );
   if( tempTwoBytes != 16 )
      return false;

   // Validate the size of the data section specified plus 44 bytes (all the headers prior to data section) 
   // add up to the actual file size.
   tempFourByte = *(uint32_t*)( _FileBuffer + 40 );
   if( tempFourByte + 44 > _FileLength )
      return false;

   //Store number of samples of 2 byte from the value (data size in word)  
   _NumShortSamples = tempFourByte / 2;
   return true;
}

void Sound::ApplyVolume()
{
   short* pSample = _Data;
   for( long dwIndex = 0; dwIndex < _NumShortSamples; dwIndex++ )
   {
      short shSample = *pSample;
      shSample = (short)( shSample * _Volume );
      *pSample = shSample;
      pSample++;
   }
}