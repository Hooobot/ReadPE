///////////////////////////////////////////////////////////////////////////////
//          University of Hawaii, College of Engineering
//          Lab 6 - Readpe - SRE - Spring 2023
//
///
/// readpe - Reads Window System portable executable files 
///          then parses its content to portray separate 
///          sections of a Window's executable file.
///          Imitates the Linux Command 'readpe'
///
/// @see     https://linuxcommandlibrary.com/man/readpe
///
/// @file    readpe.c
/// @author  Hubert Liang <hubertl@hawaii.edu>
///////////////////////////////////////////////////////////////////////////////

#ifndef readpe_h
#define readpe_h

#include <string.h>     // For strtok
#include <stdio.h>      // For printf, perror, FILE, fopen, fread, fclose
#include <stdint.h>     // For uint16_t, uint32_t
#include <time.h>       // For time_t, ctime

// Define DLL characteristics for the Optional/Image Header

#define IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE 0x0040
#define IMAGE_DLLCHARACTERISTICS_NX_COMPAT 0x0100
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE 0x8000


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
} IMAGE_SECTION_HEADER;


// Define the Optional / Image Header

typedef struct {
   uint16_t Magic;
   uint8_t MajorLinkerVersion;
   uint8_t MinorLinkerVersion;
   uint32_t SizeOfCode;
   uint32_t SizeOfInitializedData;
   uint32_t SizeOfUninitializedData;
   uint32_t AddressOfEntryPoint;
   uint32_t BaseOfCode;
   uint32_t BaseOfData;
   uint32_t ImageBase;
   uint32_t SectionAlignment;
   uint32_t FileAlignment;
   uint16_t MajorOperatingSystemVersion;
   uint16_t MinorOperatingSystemVersion;
   uint16_t MajorImageVersion;
   uint16_t MinorImageVersion;
   uint16_t MajorSubsystemVersion;
   uint16_t MinorSubsystemVersion;
   uint32_t Win32VersionValue;
   uint32_t SizeOfImage;
   uint32_t SizeOfHeaders;
   uint32_t CheckSum;
   uint16_t Subsystem;
   uint16_t DllCharacteristics;
   uint32_t SizeOfStackReserve;
   uint32_t SizeOfStackCommit;
   uint32_t SizeOfHeapReserve;
   uint32_t SizeOfHeapCommit;
} Optional_Header;

#endif 
// End of header
