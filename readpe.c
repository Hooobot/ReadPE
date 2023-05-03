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


#include <stdio.h>         // For printf, perror, FILE, fopen, fread, fclose
#include <stdint.h>        // For uint16_t, uint32_t
#include <assert.h>        // For assert
#include "doshdr.h"        // For DOS_Header 
#include "coffhdr.h"       // For COFF_Header
#include "optionhdr.h"     // For Optional/Image Header
#include "sectionhdr.h"    // For Sections


int main(int argc, char *argv[]) {        // argv for PE file designation
   
   // Check for correct number of arguments
   if (argc != 2) {
      // Otherwise specifies instructions
      printf("Usage: %s <filename>\n", argv[0]);
      return 1;
   }
   // Ensure that argv[1] is not a NULL pointer
   assert(argv[1] != NULL);


   // Calls readdos and readcoff with file given via argument
   int dos_result = readdos(argv[1]);
   int coff_result = readcoff(argv[1]);
   int optionhdr_result = readoptionhdr(argv[1]);
   int sections_result = read_sections(argv[1]);

   // Check if the functions returned success (assuming 0 is success)
   assert(dos_result == 0);
   assert(coff_result == 0);
   assert(optionhdr_result == 0);
   assert(sections_result == 0);

   // Returns 0 if properly executed, 1 if not
   return 0;
}  
/// main



