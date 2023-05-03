///////////////////////////////////////////////////////////////////////////////
//          University of Hawaii, College of Engineering
//          Lab 6 - Readpe - SRE - Spring 2023
//
///
/// readpe - Header for the readpe.h file, defining the structs for
///          DOS Header, COFF Header, Section Header, and Optional Header
///          
///
/// @see     https://linuxcommandlibrary.com/man/readpe
///
/// @file    readpe.h
/// @author  Hubert Liang <hubertl@hawaii.edu>
///////////////////////////////////////////////////////////////////////////////

#ifndef readpe_h
#define readpe_h


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


#endif 
// End of header
