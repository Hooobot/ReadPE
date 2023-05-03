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


#include <stdio.h>      // For printf, perror, FILE, fopen, fread, fclose
#include <time.h>       // For time_t, ctime
#include <string.h>     // For strtok, memset
#include <stdint.h>     // For uint16_t, uint32_t
#include <assert.h>     // For assert
#include "readpe.h"     // For DOS, COFF, Optional, and Section structs


// Define DLL characteristics for the Optional/Image Header

#define IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE 0x0040
#define IMAGE_DLLCHARACTERISTICS_NX_COMPAT 0x0100
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE 0x8000


///////////////////////////////////////////////////////////////////////////////
/* Define a function to read and parse the DOS header data */
int readdos(char *filename)
{
   //Stream IO for taking and parsing data
   FILE *fp;
   DOS_Header doshdr;

   // Zero out structs for consistency
   memset(&doshdr,      0, sizeof(doshdr));

   // Open the file for binary reading
   fp = fopen(filename, "rb");
   // Error if file cannot be opened
   if (fp == NULL) {
      perror("Error opening file");
      return 1;
   }

   // Read the DOS header
   if (fread(&doshdr, sizeof(doshdr), 1, fp) != 1) {
      // Error if file cannot be read
      perror("Error reading DOS header");
      return 1;
   }

   // Print the DOS header information
   // @todo perhaps better formatting with printf, right align, etc.
   printf("DOS Header\n");
   // This... is wrong. @todo to check the MZ and confirm its accruacy
   printf("    Magic number:                    0x%04x (MZ)\n", doshdr.Magic);
   printf("    Bytes in last page:              %d\n", doshdr.BytesLastPage);
   printf("    Pages in file:                   %d\n", doshdr.Pages);
   printf("    Relocations:                     %d\n", doshdr.Relocations);
   printf("    Size of header in paragraphs:    %d\n", doshdr.HeaderSize);
   printf("    Minimum extra paragraphs:        %d\n", doshdr.MinParagraph);
   printf("    Maximum extra paragraphs:        %d\n", doshdr.MaxParagraph);
   printf("    Initial (relative) SS value:     0x%0x\n", doshdr.SS);
   printf("    Initial SP value:                0x%0x\n", doshdr.SP);
   printf("    Initial IP value:                0x%0x\n", doshdr.IP);
   printf("    Initial (relative) CS value:     0x%0x\n", doshdr.CS);
   printf("    Address of relocation table:     0x%0x\n", doshdr.RelocationTable);
   printf("    Overlay number:                  %d\n", doshdr.OverlayNum);
   printf("    OEM identifier:                  %d\n", doshdr.OEM_ID);
   printf("    OEM information:                 %d\n", doshdr.OEM_Info);
   printf("    PE header offset:                0x%0x\n", doshdr.PEOffset);

   // Move fp to PE header offset
   fseek(fp, doshdr.PEOffset, SEEK_SET);
   // Closed file for cleanliness
   fclose(fp);
   // Return 0 if read properly
   return 0;
}  /// readdos




///////////////////////////////////////////////////////////////////////////////
/* Define a function to read the COFF header */
int readcoff (char* filename) {

   FILE* fp;
   COFF_Header coffhdr;

   // Zero out structs for consistency
   memset(&coffhdr,     0, sizeof(coffhdr));

   // Opens a new file pointer for the COFF header
   fp = fopen(filename, "rb");
   // Error if file cannot be opened
   if (fp == NULL) {
      perror("Error opening file");
      return 1;
   }

   // Finds PE Offset from DOS header for proper start location
   fseek(fp, 0x3C, SEEK_SET);
   uint32_t PEOffset;
   if (fread(&PEOffset, sizeof(PEOffset), 1, fp) != 1) {
      perror("Error reading e_lfanew");
      return 1;
   }
   // Continues off PE Offset for start of COFF header
   fseek(fp, PEOffset + 4, SEEK_SET);
   if (fread(&coffhdr, sizeof(coffhdr), 1, fp) != 1) {
      perror("Error reading COFF header");
      return 1;
   }


   // Prints the COFF header information 
   // @todo perhaps better formatting with printf, right align, etc.
   printf("COFF/File header\n");
   printf("    Machine:                         0x%04x ", coffhdr.Machine);

   // Uses a switch to accompany future development of known machines
   switch (coffhdr.Machine) {
      // 0x14c has value of IMAGE_FILE_MACHINE_I386
      case 0x14c:
         printf("IMAGE_FILE_MACHINE_I386\n");
         break;
      case 0x8664:
         printf("IMAGE_FILE_MACHINE_AMD64\n");
         break;
      default:
         printf("Unknown machine\n");
         break;
   }
   printf("    Number of sections:              %d\n", coffhdr.NumberOfSections);

   // Determines timestamp in form of a long as 'readpe' does
   // Also portrays time, but in terms of local time rather than UTC
   // There are many ways to do this. I used local since it seemed simplest
   time_t timestamp = coffhdr.TimeDateStamp;
   char *time = strtok(ctime(&timestamp), "\n");
   printf("    Date/time stamp:                 %ld (%s)\n", (long) timestamp, time);

   // Other COFF info...
   printf("    Symbol Table offset:             0x%x\n", coffhdr.PointerToSymbolTable);
   printf("    Number of symbols:               %d\n", coffhdr.NumberOfSymbols);
   printf("    Size of optional header:         0x%x\n", coffhdr.SizeOfOptionalHeader);
   printf("    Characteristics:                 0x%x\n", coffhdr.Characteristics);
   printf("    Characteristics names\n");

   // Determines if the following two Characteristics flag is set
   // @todo More to/can be added 
   // @todo Can be formatted cleaner - switch, flag system, etc.
   // @todo might be split to a function similar to the section header below
   if (coffhdr.Characteristics & 0x0002) {
      printf("                                         IMAGE_FILE_EXECUTABLE_IMAGE\n");
   }
   if (coffhdr.Characteristics & 0x0020) {
      printf("                                         IMAGE_FILE_LARGE_ADDRESS_AWARE\n");
   }
   if (coffhdr.Characteristics & 0x0100) {
      printf("                                         IMAGE_FILE_32BIT_MACHINE\n");
   }

   fclose(fp);
   return 0;
}  /// readcoff




///////////////////////////////////////////////////////////////////////////////
/* Define a function to read the Optional/Image Header */

int read_optional_header(char *filename) {
   FILE *fp;
   Optional_Header optionalhdr;
   COFF_Header coffhdr;
   DOS_Header doshdr;

   // Zero out structs for consistency
   memset(&optionalhdr, 0, sizeof(optionalhdr));
   memset(&doshdr,      0, sizeof(doshdr));
   memset(&coffhdr,     0, sizeof(coffhdr));


   fp = fopen(filename, "rb");
   if (fp == NULL) {
      perror("Error opening file");
      return 1;
   }

   // Read the DOS header
   if (fread(&doshdr, sizeof(doshdr), 1, fp) != 1) {
      perror("Error reading DOS header");
      return 1;
   }

   // Read the COFF header
   fseek(fp, doshdr.PEOffset + 4, SEEK_SET);
   if (fread(&coffhdr, sizeof(coffhdr), 1, fp) != 1) {
      perror("Error reading COFF header");
      return 1;
   }

   // Read the optional header
   if (fread(&optionalhdr, sizeof(optionalhdr), 1, fp) != 1) {
      perror("Error reading optional header");
      return 1;
   }

   // Print the optional header information
   // Print the optional header information
   printf("Optional/Image header\n");
   printf("    Magic number:                    0x%x (PE32)\n", optionalhdr.Magic);
   printf("    Linker major version:            %d\n", optionalhdr.MajorLinkerVersion);
   printf("    Linker minor version:            %d\n", optionalhdr.MinorLinkerVersion);
   printf("    Size of .text section:           0x%x\n", optionalhdr.SizeOfCode);
   printf("    Size of .data section:           0x%x\n", optionalhdr.SizeOfInitializedData);
   printf("    Size of .bss section:            0x%x\n", optionalhdr.SizeOfUninitializedData);
   printf("    Entrypoint:                      0x%x\n", optionalhdr.AddressOfEntryPoint);
   printf("    Address of .text section:        0x%x\n", optionalhdr.BaseOfCode);
   printf("    Address of .data section:        0x%x\n", optionalhdr.BaseOfData);
   printf("    ImageBase:                       0x%x\n", optionalhdr.ImageBase);
   printf("    Alignment of sections:           0x%x\n", optionalhdr.SectionAlignment);
   printf("    Alignment factor:                0x%x\n", optionalhdr.FileAlignment);
   printf("    Major version of required OS:    %d\n", optionalhdr.MajorOperatingSystemVersion);
   printf("    Minor version of required OS:    %d\n", optionalhdr.MinorOperatingSystemVersion);
   printf("    Major version of image:          %d\n", optionalhdr.MajorImageVersion);
   printf("    Minor version of image:          %d\n", optionalhdr.MinorImageVersion);
   printf("    Major version of subsystem:      %d\n", optionalhdr.MajorSubsystemVersion);
   printf("    Minor version of subsystem:      %d\n", optionalhdr.MinorSubsystemVersion);
   printf("    Size of image:                   0x%x\n", optionalhdr.SizeOfImage);
   printf("    Size of headers:                 0x%x\n", optionalhdr.SizeOfHeaders);
   printf("    Checksum:                        0x%x\n", optionalhdr.CheckSum);
   printf("    Subsystem required:              0x%x", optionalhdr.Subsystem);

   if (optionalhdr.Subsystem == 0x3) {
      printf(" (IMAGE_SUBSYSTEM_WINDOWS_CUI)\n");
   }

   printf("    DLL characteristics:             0x%x\n", optionalhdr.DllCharacteristics);
   printf("    DLL characteristics names\n");

   // Print the DLL characteristics names
   uint16_t dll_characteristics = optionalhdr.DllCharacteristics;
   if (dll_characteristics & IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE) {
      printf("                                         IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE\n");
   }
   if (dll_characteristics & IMAGE_DLLCHARACTERISTICS_NX_COMPAT) {
      printf("                                         IMAGE_DLLCHARACTERISTICS_NX_COMPAT\n");
   }
   if (dll_characteristics & IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE) {
      printf("                                         IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE\n");
   }

   printf("    Size of stack to reserve:        0x%x\n", optionalhdr.SizeOfStackReserve);
   printf("    Size of stack to commit:         0x%x\n", optionalhdr.SizeOfStackCommit);
   printf("    Size of heap space to reserve:   0x%x\n", optionalhdr.SizeOfHeapReserve);
   printf("    Size of heap space to commit:    0x%x\n", optionalhdr.SizeOfHeapCommit);

   // Close the file
   fclose(fp);
   return 0;
}



///////////////////////////////////////////////////////////////////////////////
/* Section Header Implementation */

// Switch statement to include the characteristic flags of a section
const char* sectionCharacteristics(uint32_t flag) {
   switch (flag) {
      case 0x20:
         return "IMAGE_SCN_CNT_CODE";
      case 0x40:
         return "IMAGE_SCN_CNT_INITIALIZED_DATA";
      case 0x80:
         return "IMAGE_SCN_CNT_UNINITIALIZED_DATA";
      case 0x20000000:
         return "IMAGE_SCN_MEM_EXECUTE";
      case 0x40000000:
         return "IMAGE_SCN_MEM_READ";
      case 0x80000000:
         return "IMAGE_SCN_MEM_WRITE";
      case 0x2000000:
         return "IMAGE_SCN_MEM_DISCARDABLE";
         // Add other characteristic names as needed
      default:
         return "UNKNOWN";
   }
}



// ReadPE Sections
int read_sections(char *filename) {
   // Initialize file, DOS Header, COFF Header to read
   FILE *fp;
   DOS_Header doshdr;
   COFF_Header coffhdr;
   Section_Header sectionhdr;

   // Zero out structs for consistency
   memset(&doshdr,      0, sizeof(doshdr));
   memset(&coffhdr,     0, sizeof(coffhdr));
   memset(&sectionhdr,  0, sizeof(sectionhdr));

   fp = fopen(filename, "rb");
   if (fp == NULL) {
      perror("Error opening file");
      return 1;
   }

   // Read the DOS header
   fread(&doshdr, sizeof(doshdr), 1, fp);

   // Read the COFF header
   fseek(fp, doshdr.PEOffset + 4, SEEK_SET);
   fread(&coffhdr, sizeof(coffhdr), 1, fp);

   // Read the sections
   // We are moving the file pointer past the PEOffset to find start of section
   fseek(fp, doshdr.PEOffset + 4 + sizeof(coffhdr) + coffhdr.SizeOfOptionalHeader, SEEK_SET);

   printf("Sections\n");
   // We want to loop through all possible sections for this program...
   for (int i = 0; i < coffhdr.NumberOfSections; ++i) {
      // ...and read each one
      fread(&sectionhdr, sizeof(sectionhdr), 1, fp);

      // Here is how 'readpe' printed it out:
      printf("    Section\n");
      printf("\tName:                            %s\n", 
            sectionhdr.Name);
      printf("\tVirtual Size:                    0x%x (%d bytes)\n", 
            sectionhdr.VirtualSize, sectionhdr.VirtualSize);
      printf("\tVirtual Address:                 0x%x\n", 
            sectionhdr.VirtualAddress);
      printf("\tSize Of Raw Data:                0x%x (%d bytes)\n", 
            sectionhdr.SizeOfRawData, sectionhdr.SizeOfRawData);
      printf("\tPointer To Raw Data:             0x%x\n", 
            sectionhdr.PointerToRawData);
      printf("\tNumber Of Relocations:           %d\n", 
            sectionhdr.NumberOfRelocations);
      printf("\tCharacteristics:                 0x%x\n", 
            sectionhdr.Characteristics);
      printf("\tCharacteristic Names\n");

      // We use a loop with a left shift operation '<<=' to move bit by bit
      // We want to use bits so we can check it easier with its characteristics 
      for (uint32_t flag = 1; flag; flag <<= 1) {
         // Bitwise '&' is used because of bitwise operation instead of logical
         if (sectionhdr.Characteristics & flag) {
            // Leave the sorting to the sectionCharacteristics function
            printf("%-47s%s\n", "", sectionCharacteristics(flag));
         }
      }
   }

   fclose(fp);
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/* argv used to determine PE file to read */
int main(int argc, char *argv[]) {
   // Check for correct number of arguments
   if (argc != 2) {
      // Otherwise specifies instructions
      printf("Usage: %s <filename>\n", argv[0]);
      return 1;
   }

   // Calls readdos and readcoff with file given via argument
   readdos(argv[1]);
   readcoff(argv[1]);
   read_optional_header(argv[1]);
   read_sections(argv[1]);

   // Returns 0 if properly executed, 1 if not
   return 0;
}  /// main



