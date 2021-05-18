#include    <stdlib.h>
#include    <stdio.h>
#include    <fcntl.h>
//
//  This utility is designed to display checksum in byte
//
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
#define SIZE_BUFFER	  0x100

void ShowUsage(void)
{
  printf("This tool show file checksum by byte.\n\n"
    "Usage:  Checksum [Dest File] \n"
   "\n");
}

int main(int argc, char *argv[])
{
  FILE *fhSrc;
  int i;
  BYTE bBuffer[SIZE_BUFFER];
  DWORD Checksum;
  DWORD lCurrentOffset;
  DWORD lROMSize = 0;
  char option_char;

  if (argc<2) {
    ShowUsage();
    exit(-1);
  }
  //
  // processing parameters
  //
  if ( argc>1)
  {
    for (i=1;i <argc; i++)
    {
      if (*argv[i] == '-')
      {
        option_char = (char)(*(argv[i]+1));
        switch (option_char) {
            case 'H':
            case 'h':
            case '?':
            ShowUsage();
            exit(0);
            break ;
            default :
            break ;
        }
      }
    }
  }
  //
  // _open Target file
  //
  if ((fhSrc = fopen (argv[1], "rb")) == NULL )
  {
    printf("%s open error!\n",argv[1]);
    exit(-2);
  }
  //
  // Checksum by byte thru the file
  //
  // _lseek( fhSrc, 0L, SEEK_SET);           /* Set to position 0 */
  fseek( fhSrc, 0L, SEEK_SET);           /* Set to position 0 */
  Checksum=0;
  lCurrentOffset=0;
  while ( fread( bBuffer, 1, SIZE_BUFFER, fhSrc))
  {
    //
    // Check if exceed size requested?
    //
    if( lROMSize && lCurrentOffset >= lROMSize)
      break;
    //
    // Accumulate _read buffer
    //    
    for (i=0; i<SIZE_BUFFER; i++)
    {
      //
      // Skip BIOSInfo block if requested & exist
      //
      Checksum += (DWORD)bBuffer[i];
    //   printf("Checksum = %08lX\n", Checksum);
    }
    lCurrentOffset += SIZE_BUFFER;
  }
  //
  // Set file size
  //
  lROMSize = lCurrentOffset;
  //
  // Show result by requested format
  //
  printf("0x%08lX 0x%lx %s", Checksum, lROMSize, argv[1]);

  // printf("Checksum Size= %dMB", (int)(lROMSize>>20));
  // if (lROMSize>>10 == (lROMSize>>20)<<10) {
  //   printf("\n");
  // } else { 
  //   printf("+%dKB\n", (int)(lROMSize>>10 & ((1<<10)-1)));
  // }
  // printf("%s\n",argv[1]);
  
  fclose(fhSrc);
  return 0;
}
