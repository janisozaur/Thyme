////////////////////////////////////////////////////////////////////////////////
//                               --  THYME  --                                //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: THYME
//
//          File:: HOOKER.CPP
//
//        Author:: OmniBlade
//
//  Contributors:: CCHyper, Saberhawk, jonwil, LRFLEW
//
//   Description:: Originally based on work by the Tiberian Technologies team
//                 to hook Renegade, rewritten based on work by LRFLEW for
//                 OpenMC2. Provides methods for accessing data and functions in
//                 an existing binary and replacing functions with new 
//                 implementations from an injected DLL.
//
//       License:: Thyme is free software: you can redistribute it and/or 
//                 modify it under the terms of the GNU General Public License 
//                 as published by the Free Software Foundation, either version 
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//                 
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "hooker.h"

static const int nBinarySize = 0x0062F000;  //Size of game binary

//HANDLE hProcess = GetCurrentProcess();
DWORD OldProtect;

void StartHooking()
{
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)0x00401000, nBinarySize, PAGE_EXECUTE_READWRITE, &OldProtect);
}

void StopHooking()
{
	DWORD OldProtect2;
    VirtualProtectEx(GetCurrentProcess(), (LPVOID)0x00401000, nBinarySize, OldProtect, &OldProtect2);
}
