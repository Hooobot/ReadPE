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
#include <stdint.h>     // For uint16_t, uint32_t
#include <time.h>       // For time_t, ctime

// Define the DOS Header

typedef struct {
   uint16_t dos_magic_num;
   uint16_t dos_bytes_last_page;
   uint16_t dos_pages;
   uint16_t dos_relocations;
   uint16_t dos_header_size;
   uint16_t dos_min_paragraphs;
   uint16_t dos_max_paragraphs;
   uint16_t dos_ss;
   uint16_t dos_sp;
   uint16_t dos_csum;
   uint16_t dos_cs;
   uint16_t dos_ip;
   uint16_t dos_relocation_table;
   uint16_t dos_overlay_num;
   uint16_t dos_res[4];
   uint16_t dos_oem_id;
   uint16_t dos_oem_info;
   uint16_t dos_res2[10];
   uint32_t dos_pe_offset;
} IMAGE_DOS_HEADER;

// Define a function to read and parse the DOS header data

int readdos(char *filename)
{
   FILE *fp;
   IMAGE_DOS_HEADER doshdr;

   // Open the file for binary reading
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

   // Print the DOS header information
   printf("DOS Header\n");
   printf("    Magic number:                    0x%04X (MZ)\n", doshdr.dos_magic_num);
   printf("    Bytes in last page:              %d\n", doshdr.dos_bytes_last_page);
   printf("    Pages in file:                   %d\n", doshdr.dos_pages);
   printf("    Relocations:                     %d\n", doshdr.dos_relocations);
   printf("    Size of header in paragraphs:    %d\n", doshdr.dos_header_size);
   printf("    Minimum extra paragraphs:        %d\n", doshdr.dos_min_paragraphs);
   printf("    Maximum extra paragraphs:        %d\n", doshdr.dos_max_paragraphs);
   printf("    Initial (relative) SS value:     0x%0x\n", doshdr.dos_ss);
   printf("    Initial SP value:                0x%0x\n", doshdr.dos_sp);
   printf("    Initial IP value:                0x%0x\n", doshdr.dos_ip);
   printf("    Initial (relative) CS value:     0x%0x\n", doshdr.dos_cs);
   printf("    Address of relocation table:     0x%0x\n", doshdr.dos_relocation_table);
   printf("    Overlay number:                  %d\n", doshdr.dos_overlay_num);
   printf("    OEM identifier:                  %d\n", doshdr.dos_oem_id);
   printf("    OEM information:                 %d\n", doshdr.dos_oem_info);
   printf("    PE header offset:                0x%0x\n", doshdr.dos_pe_offset);

   fseek(fp, doshdr.dos_pe_offset, SEEK_SET);

   fclose(fp);
   return 0;
}


int main(int argc, char *argv[])
{
   if (argc != 2) {
      printf("Usage: %s <filename>\n", argv[0]);
      return 1;
   }

   readdos(argv[1]);

   return 0;
}

