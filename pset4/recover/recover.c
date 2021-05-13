#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover <name of forensic image>\n");
        return 1;
    }
    
    FILE *fptr = fopen(argv[1], "r");
    if (fptr == NULL)
    {
        printf("File cannot be opened for reading.\n");
        return 1;
    }
    
    BYTE buffer[512];
    int count = 0, firstJPG = 0;
    FILE *f;
    
    while (fread(buffer, 512, 1, fptr) == 1)
    {
        //check if start of new JPG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //check if first JPG
            if (firstJPG == 0)
            {
                firstJPG = 1;
            }
            else
            {
                //if not, add 1 to count of JPG number then close current file
                count++;
                fclose(f);
            }
            
            //make a new filename for this ###th JPG, assign a file pointer to the new file and start writing into it
            char filename[8];
            sprintf(filename, "%03i.jpg", count);
            f = fopen(filename, "w");
            fwrite(buffer, 512, 1, f);
        }
        //if start of first JPG has not been found yet, or continuation of current JPG
        else
        {
            //only write into file if it is a continuation of current JPG
            if (firstJPG == 1)
            {
                fwrite(buffer, 512, 1, f);
            }
        }
    }
    
    //close files
    fclose(f);
    fclose(fptr);
    
    return 0;
}
