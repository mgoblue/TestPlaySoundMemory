// PlaySoundMemory.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Sound.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
   cout << "So far so good" << endl;

   Sound laptopClick( "laptop_click_single.wav", 0.25 );

   laptopClick.Play( false );

   Sound mouseClick( "mouse_click_single.wav", 0.2);

   mouseClick.Play( false );

   return 0;
}