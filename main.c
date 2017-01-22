/******************************************************************************
//
// NAME:          Blake Larson
//
// HOMEWORK:      HW7a
//
// CLASS:         ICS 212
//
// INSTRUCTOR:    Ravi Narayan
//
// DATE:          November 2, 2016
//
// FILE:          main.c
//
// DESCRIPTION:   This file holds the main function only.
//
*****************************************************************************/

#include "header.h"

unsigned char tcpContainer[20];

/******************************************************************************
// Function name: main
//
// DESCRIPTION:   This is the main function that reads in a binary file and
//                print its corresponding header request. The program then
//                writes edits to existing header and then writes a new
//                response header to a different binary file. Finally the
//                new binary file is then read back as a response header in
//                the console.
//
//                Comments:
//                    If the binary file specified in the first call to
//                    readfile doesn't exist in the directory, then a print
//                    error will occurr and the Request and Response tcp
//                    headers will both print normally but with all zeros
//                    instead. Thus the program will still run.
//
// Parameters:    None
//
// Return values: int (for now 0)
//
*****************************************************************************/

int main()
{
  /* Reading in original .bin file called (test2.bin) */
  readfile("test.bin");

  printf("\n");
  printf("--------------Request--------------\n");

  /* Printing original request header*/
  printheader();

  /* Editing, then writing the responce file */
  writefile("test1.bin");

  /* Reading in new response .bin file called (test1.bin) */
  readfile("test1.bin");

  printf("\n");
  printf("--------------Response-------------\n");

  /* Printing new response header */
  printheader();

  printf("\n");
  printf("-----End of TCP Header Program-----\n");

  return 0;
}
