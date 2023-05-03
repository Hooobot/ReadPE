///////////////////////////////////////////////////////////////////////////////
//          University of Hawaii, College of Engineering
//          Lab 6 - Readpe - SRE - Spring 2023
//
// readdos - Gets the DOS_Header information in a Windows PE file
///
/// @see     https://linuxcommandlibrary.com/man/readpe
///
/// @file    doshdr.c
/// @author  Hubert Liang <hubertl@hawaii.edu>
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>      // For printf, perror, FILE, fopen, fread, fclose
#include <stdint.h>     // For uint16_t, uint32_t
#include <string.h>     // For memset
#include <assert.h>     // For assert
#include "doshdr.h"     // For DOS_Header struct

// Validation for the DOS_Header data
void validate_dos_header(DOS_Header *doshdr) {
    // Verify the magic number (MZ)
    assert(doshdr->Magic == 0x5A4D);

    // Additional checks for values in the DOS header
    assert(doshdr->Pages >= 0);
    assert(doshdr->Relocations >= 0);
    assert(doshdr->HeaderSize > 0);
    assert(doshdr->MinParagraph >= 0);
    assert(doshdr->MaxParagraph >= 0);
    assert(doshdr->SS >= 0);
    assert(doshdr->SP >= 0);
    assert(doshdr->IP >= 0);
    assert(doshdr->CS >= 0);
    assert(doshdr->RelocationTable >= 0);
    assert(doshdr->OverlayNum >= 0);
    assert(doshdr->OEM_ID >= 0);
    assert(doshdr->OEM_Info >= 0);
    assert(doshdr->PEOffset > 0);
}


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
   
   // Validate the DOS header using the prior function
   validate_dos_header(&doshdr);

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
}  
/// readdos


