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
#include <string.h>        // For strtok, memset
#include <stdint.h>        // For uint16_t, uint32_t
#include <assert.h>        // For assert
#include "readpe.h"        // For DOS, COFF, Optional, and Section structs
#include "doshdr.h"
#include "coffhdr.h"
#include "optionhdr.h"
#include "sectionhdr.h"

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



