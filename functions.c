/******************************************************************************
//
// FILE:          functions.c
//
// DESCRIPTION:   Three functions that are used to read in a binary file
//                that is specified in the parameter of readfile function,
//                then the contents are printed. After, the contents are
//                then modified to specification, and then written to a new
//                binary file. This new binary file is then read in and its
//                contents are printed showing the updates have been made.
//
*****************************************************************************/

#include "header.h"
extern unsigned char tcpContainer[20];

/******************************************************************************
// Function name: printheader
//
// DESCRIPTION:   A function that takes no parameters. This function prints
//                the entire contents of the global char array called
//                tcpContainer. This function is used to print first the
//                request and then the response tcp headers.
//
//                Comments: The reserved is 6 bits of the existing 8, but
//                the first two bits of the 8 are 00. In this case 00111111.
//                This was bit shifted to the right twice to show 6 bits.
//
// Parameters:    None
//
// Return values: int (for now 0)
//
*****************************************************************************/

int printheader()
{
  unsigned short tempShort;
  unsigned long tempLong;
  unsigned int tempInt;
  unsigned char tempChar;
  unsigned char tempChar2;
  int cbEmpty;

  cbEmpty = 0;

  printf("----Printing current TCP header----\n");
  printf("------------TCP Header-------------\n");

  /* 8bits */
  tempShort = tcpContainer[0];
  tempShort = tempShort << 8;
  tempShort = tempShort | tcpContainer[1];
  printf("Source Port: ");
  printf("%u\n", tempShort);

  /* 8bits */
  tempShort = tcpContainer[2];
  tempShort = tempShort << 8;
  tempShort = tempShort | tcpContainer[3];
  printf("Destination Port: ");
  printf("%u\n", tempShort);

  /* 32 bits */
  tempLong = tcpContainer[4];
  tempLong = tempLong << 8;
  tempLong = tempLong | tcpContainer[5];
  tempLong = tempLong << 8;
  tempLong = tempLong | tcpContainer[6];
  tempLong = tempLong << 8;
  tempLong = tempLong | tcpContainer[7];
  printf("Sequence Number: ");
  printf("%u\n", tempLong);

  /* 32 bits */
  tempLong = tcpContainer[8];
  tempLong = tempLong << 8;
  tempLong = tempLong | tcpContainer[9];
  tempLong = tempLong << 8;
  tempLong = tempLong | tcpContainer[10];
  tempLong = tempLong << 8;
  tempLong = tempLong | tcpContainer[11];
  printf("Acknowledgement Number: ");
  printf("%u\n", tempLong);

  /* 4 bits */
  tempInt = tcpContainer[12];
  tempInt = tempInt >> 4;
  printf("Data Offset: ");
  printf("%u\n", tempInt);

  /* 6 bits: There will be an initial 00 ahead of 6 bits for Reserved. */
  tempChar = tcpContainer[12];
  tempChar = tempChar << 4;
  tempChar2 = tcpContainer[13];
  tempChar2 = tempChar2 >> 6;
  tempChar2 = tempChar2 << 2;
  tempChar = tempChar | tempChar2;
  tempChar = tempChar >> 2;
  printf("Reserved: ");
  printf("%u\n", tempChar);

  /* 6 bits */
  tempShort = tcpContainer[13];
  printf("Control Bits: \n");
  if(tempShort & 0x01)
  {
    printf("\tFIN\n");
    cbEmpty++;
  }
  if(tempShort & 0x02)
  {
    printf("\tSYN\n");
    cbEmpty++;
  }
  if(tempShort & 0x04)
  {
    printf("\tRST\n");
    cbEmpty++;
  }
  if(tempShort & 0x08)
  {
    printf("\tPSH\n");
    cbEmpty++;
  }
  if(tempShort & 0x10)
  {
    printf("\tACK\n");
    cbEmpty++;
  }
  if(tempShort & 0x20)
  {
    printf("\tURG\n");
    cbEmpty++;
  }
  if(cbEmpty == 0)
  {
    printf("There are no flags set!");
  }

  /* 8 bits */
  tempShort = tcpContainer[14];
  tempShort = tempShort << 8;
  tempShort = tempShort | tcpContainer[15];
  printf("Window: ");
  printf("%04u\n", tempShort);

  /* 8 bits */
  tempShort = tcpContainer[16];
  tempShort = tempShort << 8;
  tempShort = tempShort | tcpContainer[17];
  printf("Checksum: ");
  printf("%04x\n", tempShort);

  /* 8 bits */
  tempShort = tcpContainer[18];
  tempShort = tempShort << 8;
  tempShort = tempShort | tcpContainer[19];
  printf("Urgent Pointer: ");
  printf("%04u\n", tempShort);

  printf("-----------------------------------\n");

  return 0;
}

/******************************************************************************
// Function name: readfile
//
// DESCRIPTION:   A function that takes a char array as input. This function
//                will store the contents of the binary file into a global
//                char array called tcpContainer that is of size 20. Each
//                8 bit sequence in the binary file, corresponds to an
//                individual index of the global char array. If the specified
//                binary file doesn't exist then the function prints an error
//                and doesn't run the function, otherwise the contents are
//                stored.
//
// Parameters:    input - a char array that takes the name of the binary file
//                        as input to be read and stored.
//
// Return values: int (for now 0)
//
*****************************************************************************/

int readfile(char input[])
{

  FILE *file;
  file = fopen(input, "r");

  if(file == NULL)
  {
    printf("\nThere was an error reading in the file!\n");
  }
  else
  {
    fread(tcpContainer,sizeof(tcpContainer), 1, file);
    printf("\nFile successfully read and contents stored in container.\n");
  }

	fclose(file);

	return 0;

}

/******************************************************************************
// Function name: writefile
//
// DESCRIPTION:   A function that takes a char array as input. This function
//                makes the appropriate changes to the current tcp header
//                information that is stored in the global char array called
//                tcpContainer. Afterwards, the function will then write the
//                current contents into a new binary file, specified by the
//                argument passed.
//
//                Specific constraints/edits to response tcp header:
//                    1) Switch the source and destination ports.
//                    2) Sequence number must be incremented by 1.
//                    3) Acknowledgment number of the response must be the
//                        sequence number of the request.
//                    4) Only flags that are "set" should be printed in header.
//                    5) If the SYN flag is set in the request, both the SYN
//                        and ACK flags must be set in the response.
//                    6) Reserved, Window, and Urgent Point must be zero.
//                    7) Checksum must be ffff.
//
// Parameters:    output - a char array that takes a name of a binary file
//                         and stores current content into it.
//
// Return values: int (for now 0)
//
*****************************************************************************/

int writefile(char output[])
{
  unsigned short tempShort;
  unsigned long tempLong;
  unsigned long tempLong2;
  FILE *file;
  file = fopen(output, "w");

  if(file == NULL)
  {
    printf("There was an error creating the file!\n");
  }
  else
  {
    /* Switch Source and destination ports */
    tempShort = tcpContainer[0];
    tcpContainer[0] = tcpContainer[2];
    tcpContainer[2] = tempShort;
    tempShort = tcpContainer[1];
    tcpContainer[1] = tcpContainer[3];
    tcpContainer[3] = tempShort;

    /* Acknowledgement number reflects Sequence number */
    tcpContainer[8] = tcpContainer [4];
    tcpContainer[9] = tcpContainer [5];
    tcpContainer[10] = tcpContainer [6];
    tcpContainer[11] = tcpContainer [7];

    /* Increment the sequence number by 1 */
    tempLong = tcpContainer[4];
    tempLong = tempLong << 8;
    tempLong = tempLong + tcpContainer[5];
    tempLong = tempLong << 8;
    tempLong = tempLong + tcpContainer[6];
    tempLong = tempLong << 8;
    tempLong = tempLong + tcpContainer[7];

    tempLong = tempLong + 1;
    tempLong2 = tempLong;
    tempLong = tempLong >> 24;
    tempLong = tempLong & 0x00ff;
    tcpContainer[4] = tempLong;

    tempLong = tempLong2;
    tempLong = tempLong << 8;
    tempLong = tempLong >> 24;
    tempLong = tempLong & 0x00ff;
    tcpContainer[5] = tempLong;

    tempLong = tempLong2;
    tempLong = tempLong << 16;
    tempLong = tempLong >> 24;
    tempLong = tempLong & 0x00ff;
    tcpContainer[6] = tempLong;

    tempLong = tempLong2;
    tempLong = tempLong << 24;
    tempLong = tempLong >> 24;
    tempLong = tempLong & 0x00ff;
    tcpContainer[7] = tempLong;

    /* Make Reserved only zero */
    tcpContainer[12] = tcpContainer[12] & 0xf0;
    tcpContainer[13] = tcpContainer[13] & 0x3f;

    /* If the SYN flag is set, then set the ACK flag as well */
    if(tcpContainer[13] & 0x02)
    {
      tcpContainer[13] = tcpContainer[13] | 0x10;
    }

    /* Make Window only zero */
    tcpContainer[14] = tcpContainer[14] & 0x00;
    tcpContainer[15] = tcpContainer[15] & 0x00;

    /* Make Checksum only ffff */
    tcpContainer[16] = tcpContainer[16] | 0xff;
    tcpContainer[17] = tcpContainer[17] | 0xff;

    /* Make Urgent Pointer only zero */
    tcpContainer[18] = tcpContainer[18] & 0x00;
    tcpContainer[19] = tcpContainer[19] & 0x00;

    printf("Header successfully editted to proper conditions.\n");

    fwrite(tcpContainer, sizeof(tcpContainer), 1, file);

    printf("New .bin file successfully written with editted header.\n");

  }

  fclose(file);

  return 0;
}
