///////////////////////////////////////////////////////////////////////////////
//          University of Hawaii, College of Engineering
//          Lab 6 - Readpe - SRE - Spring 2023
//
/// Creates the struct necessary for the readcoff function in 'doshdr.c'
/// Also establishes function prototype for 'readpe.c'
///
/// @see     https://learn.microsoft.com/en-us/windows/win32/debug/pe-format 
///
/// @file    doshdr.h
/// @author  Hubert Liang <hubertl@hawaii.edu>
///////////////////////////////////////////////////////////////////////////////

#ifndef doshdr_h
#define doshdr_h


// Define the DOS Header
typedef struct {
   uint16_t Magic;
   uint16_t BytesLastPage;
   uint16_t Pages;
   uint16_t Relocations;
   uint16_t HeaderSize;
   uint16_t MinParagraph;
   uint16_t MaxParagraph;
   uint16_t SS;
   uint16_t SP;
   uint16_t CSUM;
   uint16_t CS;
   uint16_t IP;
   uint16_t RelocationTable;
   uint16_t OverlayNum;
   uint16_t Reserved4[4];          // Reserved space...
   uint16_t OEM_ID;
   uint16_t OEM_Info;
   uint16_t Reserved10[10];        // More reserved space
   uint32_t PEOffset;
} DOS_Header;

// Contain function prototype for 'readpe.c' to utilize
int readdos(char* filename);

#endif
// End of doshdr header file
