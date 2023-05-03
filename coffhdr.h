///////////////////////////////////////////////////////////////////////////////
//          University of Hawaii, College of Engineering
//          Lab 6 - Readpe - SRE - Spring 2023
//
/// Creates the struct necessary for the readcoff function in 'coffhdr.c'
/// Also establishes function prototype for 'readpe.c'
///
/// @see     https://learn.microsoft.com/en-us/windows/win32/debug/pe-format 
///
/// @file    coffhdr.h
/// @author  Hubert Liang <hubertl@hawaii.edu>
///////////////////////////////////////////////////////////////////////////////

#ifndef coffhdr_h
#define coffhdr_h

// Define the COFF Header
typedef struct {
   uint16_t Machine;
   uint16_t NumberOfSections;
   uint32_t TimeDateStamp;
   uint32_t PointerToSymbolTable;
   uint32_t NumberOfSymbols;
   uint16_t SizeOfOptionalHeader;
   uint16_t Characteristics;
} COFF_Header;

// Contain function prototype for 'readpe.c' to utilize
int readcoff(char* filename);

#endif
// End of coffhdr header file
