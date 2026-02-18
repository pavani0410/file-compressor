// compressor_rle.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_THREADS 4

// Structure to hold data for each thread
typedef struct
{
    char *input_data;
    char *output_data;
    int start;
    int end;
    int *output_size;
} ThreadData;

// Run-Length Encoding compression function
void *compress_chunk(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    int input_pos = data->start;
    int output_pos = data->start;

    while (input_pos < data->end)
    {
        char current = data->input_data[input_pos];
        int count = 1;

        while (input_pos + count < data->end && data->input_data[input_pos + count] == current && count < 255)
        {
            count++;
        }

        if (count > 3)
        {                                          // Only compress if the sequence is longer than 3
            data->output_data[output_pos++] = '#'; // Compression marker
            data->output_data[output_pos++] = count;
            data->output_data[output_pos++] = current;
            input_pos += count;
        }
        else
        {
            data->output_data[output_pos++] = current;
            input_pos++;
        }
    }

    *data->output_size = output_pos - data->start;
    pthread_exit(NULL);
}

// Function to compress a file using RLE with threading
void compress_file(const char *input_file, const char *output_file)
{
    FILE *in_fp, *out_fp;
    struct stat st;

    // Get file size
    stat(input_file, &st);
    long file_size = st.st_size;

    // Allocate memory
    char *input_buffer = malloc(file_size);
    char *output_buffer = malloc(file_size * 2); // Allocate more space in case compressed data needs it

    // Read input file
    in_fp = fopen(input_file, "rb");
    fread(input_buffer, 1, file_size, in_fp);
    fclose(in_fp);

    // Initialize threads
    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[MAX_THREADS];
    int output_sizes[MAX_THREADS];
    int chunk_size = file_size / MAX_THREADS;

    // Launch compression threads
    for (int i = 0; i < MAX_THREADS; i++)
    {
        thread_data[i].input_data = input_buffer;
        thread_data[i].output_data = output_buffer;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == MAX_THREADS - 1) ? file_size : (i + 1) * chunk_size;
        thread_data[i].output_size = &output_sizes[i];
        pthread_create(&threads[i], NULL, compress_chunk, &thread_data[i]);
    }

    // Wait for threads to complete
    int total_size = 0;
    for (int i = 0; i < MAX_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
        total_size += output_sizes[i];
    }

    // Check if compression is effective
    if (total_size >= file_size)
    {
        printf("RLE compression not possible. Compressed file is larger than the original.\n");
    }
    else
    {
        // Write compressed data to output file
        out_fp = fopen(output_file, "wb");
        fwrite(output_buffer, 1, total_size, out_fp);
        fclose(out_fp);
        printf("File compressed successfully. Original size: %ld bytes, Compressed size: %d bytes\n", file_size, total_size);
    }

    free(input_buffer);
    free(output_buffer);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s -c input_file output_file\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0)
    {
        compress_file(argv[2], argv[3]);
    }
    else
    {
        printf("Invalid option. Use -c for compress\n");
        return 1;
    }

    return 0;
}