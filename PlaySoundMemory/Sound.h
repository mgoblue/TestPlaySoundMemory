#include <string>

class Sound
{
public:
   Sound( const std::string& fileName, const double& volume );
   ~Sound();
   void Play( bool asynchro = true );

private:
   void ApplyVolume();
   bool LoadFromFile( const std::string& fileName );
   bool ParseFileBuffer();

   char* _FileBuffer;
   short* _Data;
   size_t _FileLength;

   long _NumShortSamples;
   double _Volume;
   bool _status;
};
