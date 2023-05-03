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
#include <string.h>     // For strtok, memset
#include <stdint.h>     // For uint16_t, uint32_t
#include <assert.h>     // For assert
#include "readpe.h"     // For DOS, COFF, Optional, and Section structs
#include "doshdr.h"
#include "coffhdr.h"
#include "optionhdr.h"



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
   readoptionhdr(argv[1]);
   read_sections(argv[1]);

   // Returns 0 if properly executed, 1 if not
   return 0;
}  /// main



