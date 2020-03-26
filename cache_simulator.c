//Name: Talha Jamal
//Student ID: 10138573

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    //to print the csv format file for outputs
    //FILE *f = fopen("cache_simulator.csv", "w");

    //Adding both files so they can be looped through
    char *file_name[2] = {"cross_correlation_trace_003.trc", "bubble_sort_trace_003.trc"};
    //looping through one file at a time
    for (int i = 0; i < 2; i++)
    {
        //initialising cache size to 512 so every time the code runs the first value is 256 (after the division)
        int cache_size = 512;

        //To go through each mode of block size [256, 128, 64, 32, 16, 8, 4, 2]
        for (int j = 0; j < 8; j++)
        {
            int address; //variable to store the memory
            int block_size = 256;

            unsigned int CRH = 0; // Cache Read Hits
            unsigned int CRM = 0; //Cache Read Misses
            unsigned int CWH = 0; // Cache Write Misses
            unsigned int CWM = 0; // Cache Write Misses
            unsigned int NRA = 0; // Number of Read Accesses
            unsigned int NWA = 0; // Number of Write Accesses

            int cache_position = 0; //variable to store position of cache

            char R_or_W; //variable to store the value

            //Masking list to get the required bits.
            int mask_list[8] = {0xFFFF, 0xFFFE, 0xFFFC, 0xFFF8, 0xFFF0, 0xFFE0, 0xFFC0, 0xFF80};
            int tag_bits; //variable to store the tag bits in

            FILE *fp = fopen(file_name[i], "r"); //To read each file

            cache_size = cache_size / 2; //to go into a different mode in every loop

            //initialising the cache array
            int cache_array[cache_size];
            for (int f = 0; f < cache_size; f++)
            {
                cache_array[f] = 0;
            }

            //Initialising the valid bits
            int valid_bit[cache_size];
            for (int q = 0; q < cache_size; q++)
            {
                valid_bit[q] = 0;
            }

            //Assigning the right size according to the cache_size to the Block Size
            block_size = block_size / cache_size;

            // initialising dirty bits to the same size as cache_array
            int dirty_bit[cache_size];
            for (int y = 0; y < cache_size; y++)
            {
                dirty_bit[y] = 0;
            }

            while (fscanf(fp, "%c %x\n\r", &R_or_W, &address) != EOF)
            {
                //Masking the tag bits
                tag_bits = address & mask_list[j];
                //Shifting the tag bits by the jth iteration of mode ID
                tag_bits = tag_bits >> (j);

                //Checking if the tag_bit is already present inside the Cache Memory
                //If it is, mark valid bit at the corresponding array position to 1
                for (int x = 0; x < cache_size; x++)
                {
                    if (tag_bits == cache_array[x])
                    {
                        valid_bit[cache_position] = 1;
                    }
                }

                //If it is a Cache Hit, checking which value to increment according to
                //read or write
                if (valid_bit[cache_position] == 1)
                {
                    if (R_or_W == 'R')
                    {
                        CRH++;
                    }
                    if (R_or_W == 'W')
                    {
                        CWH++;
                        //Moving Dirty Bit to 1 when it is a Cache Write Hit
                        dirty_bit[cache_position] = 1;
                    }
                }

                //Code to enter when it is a Cache Miss
                if (valid_bit[cache_position] == 0)
                {
                    if (R_or_W == 'R')
                    {
                        // check dirty bits before you put anything in the cache
                        if (dirty_bit[cache_position] == 1)
                        {
                            NWA = NWA + block_size;
                            NRA = NRA + block_size;
                        }
                        if (dirty_bit[cache_position] == 0)
                        {
                            NRA = NRA + block_size;
                        }
                        //Assigning Dirty Bit to 0
                        dirty_bit[cache_position] = 0;

                        cache_array[cache_position] = tag_bits;
                        CRM++;
                        cache_position++;
                        if (cache_position > (cache_size - 1))
                        {
                            cache_position = 0;
                        }
                    }
                    else if (R_or_W == 'W')
                    {
                        // check dirty bits before you enter something in the cache
                        if (dirty_bit[cache_position] == 1)
                        {
                            NWA = NWA + block_size;
                            NRA = NRA + block_size;
                        }
                        if (dirty_bit[cache_position] == 0)
                        {
                            NRA = NRA + block_size;
                        }
                        //Assigning Dirty Bit to 1
                        dirty_bit[cache_position] = 1;

                        cache_array[cache_position] = tag_bits;
                        CWM++;
                        cache_position++;
                        if (cache_position > (cache_size - 1))
                        {
                            cache_position = 0;
                        }
                    }
                }
                // Initialising valid bit back to 0 for all array values before the next mode
                for (int a = 0; a < cache_size; a++)
                {
                    valid_bit[a] = 0;
                }
            }
            fclose(fp);
            //Printing the results
            printf("%s, ", file_name[i]);
            printf("%d, ", (j + 1));
            printf("%d, ", NRA);
            printf("%d, ", NWA);
            printf("%d, ", CRH);
            printf("%d, ", CRM);
            printf("%d, ", CWH);
            printf("%d\n", CWM);
        }
    }
    return 0;
}