///////////////////////////////////////////////////////////////////////////////
//          University of Hawaii, College of Engineering
//          Lab 6 - Readpe - SRE - Spring 2023
//
/// Creates the struct necessary for the readcoff function in 'optionhdr.c'
/// Also establishes function prototype for 'readpe.c'
///
/// @see     https://learn.microsoft.com/en-us/windows/win32/debug/pe-format 
///
/// @file    optionhdr.h
/// @author  Hubert Liang <hubertl@hawaii.edu>
///////////////////////////////////////////////////////////////////////////////

#ifndef optionhdr_h
#define optionhdr_h

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


// Contain function prototype for 'readpe.c' to utilize
int readoptionhdr(char* filename);

#endif
// End of optionhdr header file

