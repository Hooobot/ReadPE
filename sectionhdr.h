///////////////////////////////////////////////////////////////////////////////
//          University of Hawaii, College of Engineering
//          Lab 6 - Readpe - SRE - Spring 2023
//
/// Creates the struct necessary for the readcoff function in 'sectionhdr.c'
/// Also establishes function prototype for 'readpe.c'
///
/// @see     https://learn.microsoft.com/en-us/windows/win32/debug/pe-format 
///
/// @file    sectionhdr.h
/// @author  Hubert Liang <hubertl@hawaii.edu>
///////////////////////////////////////////////////////////////////////////////

#ifndef sectionhdr_h
#define sectionhdr_h

// Define the Section Header
typedef struct {
   uint8_t  Name[8];
   uint32_t VirtualSize;
   uint32_t VirtualAddress;
   uint32_t SizeOfRawData;
   uint32_t PointerToRawData;
   uint32_t PointerToRelocations;
   uint32_t PointerToLinenumbers;
   uint16_t NumberOfRelocations;
   uint16_t NumberOfLinenumbers;
   uint32_t Characteristics;
} Section_Header;

// Contain function prototype for 'readpe.c' to utilize
int read_sections(char* filename);
const char* sectionCharacteristics(uint32_t flag);

#endif
// End of sectionhdr header file

