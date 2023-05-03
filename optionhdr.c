///////////////////////////////////////////////////////////////////////////////
//          University of Hawaii, College of Engineering
//          Lab 6 - Readpe - SRE - Spring 2023
//
// readcoff  Gets the Optional_Header information in a Windows PE file
///
/// @see     https://linuxcommandlibrary.com/man/readpe
///
/// @file    optionhdr.c
/// @author  Hubert Liang <hubertl@hawaii.edu>
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>         // For printf, perror, FILE, fopen, fread, fclose
#include <stdint.h>        // For uint16_t, uint32_t
#include <string.h>        // For memset
#include "coffhdr.h"       // For COFF_Header struct and function
#include "doshdr.h"        // For DOS_Header struct and function
#include "optionhdr.h"     // For Optional_Header struct

// Define DLL characteristics for the Optional/Image Header

#define IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE 0x0040
#define IMAGE_DLLCHARACTERISTICS_NX_COMPAT 0x0100
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE 0x8000

/* Define a function to read the Optional/Image Header */

int readoptionhdr(char *filename) {
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
/// readoptionhdr
