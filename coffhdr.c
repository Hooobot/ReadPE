///////////////////////////////////////////////////////////////////////////////
//          University of Hawaii, College of Engineering
//          Lab 6 - Readpe - SRE - Spring 2023
//
// readcoff  Gets the COFF_Header information in a Windows PE file
///
/// @see     https://linuxcommandlibrary.com/man/readpe
///
/// @file    coffhdr.c
/// @author  Hubert Liang <hubertl@hawaii.edu>
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>      // For printf, perror, FILE, fopen, fread, fclose
#include <stdint.h>     // For uint16_t, uint32_t
#include <time.h>       // For time_t, ctime
#include <string.h>     // For memset
#include "coffhdr.h"    // For COFF_Header struct


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


