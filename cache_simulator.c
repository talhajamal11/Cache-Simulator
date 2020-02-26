#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char read_or_write;
    int address;

    int cache_line_ID;
    int tag_bits;
    int cache_size = 1;

    unsigned int read_count;
    unsigned int write_count;
    unsigned int cache_read_hits;
    unsigned int cache_read_misses;
    unsigned int cache_write_hits;
    unsigned int cache_write_misses;

    char *file_name[2] = {"bubble_sort_trace_003.trc", "cross_correlation_trace_003.trc"};

    FILE *fp;

    //looping through both files
    for (int i = 0; i < 2; i++)
    {
        fp = fopen(file_name[i], "r"); //reading mode
        printf("Current File is : %s \n", file_name[i]);

        //To go through each mode of block size [128, 64, 32, 16, 8, 4, 2, 1]

        for (int j = 0; j < 8; j++)
        {

            cache_size = cache_size * 2;
            int cache_array[cache_size];
            int valid_bits[cache_size];

            /*for (int x = 0; x < cache_size; x++)
            {
                valid_bits[x] = 0;
                cache_array[x] = 0;
            }*/

            while (fscanf(fp, "%c %x\n\r", &read_or_write, &address) != EOF)
            {
                if (read_or_write == 'R')
                {
                    read_count++;
                }
                if (read_or_write == 'W')
                {
                    write_count++;
                }

                //Masking the tag bits with 0xFF80 because we need 16 bits
                tag_bits = address & 0xFF80;
                //Shifting the tag bits by 1 every iteration of mode
                tag_bits = address >> (cache_size - 1);
                int f = 0;
            }
            printf("Total Reads for %s = %d\n, Total Writes for %s = %d\n", file_name[i], read_count, file_name[i], write_count);
        }
        printf("These business students are dicks\n");
    }
    return 0;
}