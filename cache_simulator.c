#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

    char *file_name[2] = {"bubble_sort_trace_003.trc", "cross_correlation_trace_003.trc"};

    //looping through both files
    for (int i = 0; i < 2; i++)
    {
        int cache_size = 512;
        //printf("Current File is : %s \n", file_name[i]);
        //To go through each mode of block size [256, 128, 64, 32, 16, 8, 4, 2, 1]
        for (int j = 0; j < 8; j++)
        {
            int address; //variable to store the memory

            unsigned int CRH = 0; // Cache Read Hits
            unsigned int CRM = 0; //Cache Read Misses
            unsigned int CWH = 0; // Cache Write Misses
            unsigned int CWM = 0; // Cache Write Misses
            unsigned int NRA = 0; // Number of Read Accesses
            unsigned int NWA = 0; // Number of Write Accesses

            int cache_position = 0; //variable to store position of cache

            char R_or_W; //variable to store the value
            int valid_bit = 0;
            int mask_list[8] = {0xFFFF, 0xFFFE, 0xFFFC, 0xFFF8, 0xFFF0, 0xFFE0, 0xFFC0, 0xFF80};
            int tag_bits; //variable to store the tag bits in

            FILE *fp = fopen(file_name[i], "r"); //reading mode

            cache_size = cache_size / 2;

            int cache_array[cache_size];

            /*for (int z = 0; z < cache_size; z++)
            {
                cache_array[z] = 0;
            }*/

            while (fscanf(fp, "%c %x\n\r", &R_or_W, &address) != EOF)
            {
                //Masking the tag bits with 0xFF80 because we need 16 bits
                tag_bits = address & mask_list[j];
                //Shifting the tag bits by the jth iteration of mode
                tag_bits = tag_bits >> (j);

                for (int x = 0; x < cache_size; x++)
                {
                    if (tag_bits == cache_array[x])
                    {
                        valid_bit = 1;
                    }
                }

                if (valid_bit == 1)
                {
                    if (R_or_W == 'R')
                    {
                        NRA++;
                        CRH++;
                    }
                    if (R_or_W == 'W')
                    {
                        NWA++;
                        CWH++;
                    }
                }

                if (valid_bit == 0)
                {
                    if (R_or_W == 'R')
                    {
                        cache_array[cache_position] = tag_bits;
                        NRA++;
                        CRM++;
                        cache_position++;
                        if (cache_position > (cache_size - 1))
                        {
                            cache_position = 0;
                        }
                    }
                    else if (R_or_W == 'W')
                    {
                        cache_array[cache_position] = tag_bits;
                        NWA++;
                        CWM++;
                        cache_position++;
                        if (cache_position > (cache_size - 1))
                        {
                            cache_position = 0;
                        }
                    }
                }
                valid_bit = 0;
            }
            fclose(fp);

            printf("\nIn file %s, in mode ID %d\n", file_name[i], (j + 1));
            printf("Total number of Read Accesses: %d\n", NRA);
            printf("Total number of Write Accesses: %d\n", NWA);
            printf("Total number of Cache Read Hits: %d\n", CRH);
            printf("Total number of of Cache Read Misses: %d\n", CRM);
            printf("Total number of Cache Write Hits: %d\n", CWH);
            printf("Total number of Cache Write Misses: %d\n", CWM);
            printf("Percentage Read Hits: %f\n", (((float)CRH / (float)NRA) * 100));
            printf("Percentage Write Hits: %f\n", (((float)CWH / (float)NWA) * 100));
        }
    }
    return 0;
}