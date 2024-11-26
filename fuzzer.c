#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MUTATION 13

const char char_pool[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#$%^&*()_+[]{}|;:',.<>?/`~";
const size_t char_pool_size = sizeof(char_pool) - 1;

unsigned char randByte()
{
    return char_pool[rand() % char_pool_size]; // Make the char rand by calling rand then mod it to 256 for range of a byte
}

void add_input(char **input, size_t *size) // Increase the size of the input by 10
{
    *input = realloc(*input, *size + 10);
    for (int32_t i = 0; i < 10; i++)
    {
        (*input)[*size + i] = randByte(); // Append rand bytes to the end of the input
    }
    *size += 10;
}

void seedPRNG(int32_t seed) // Initial seed of the
{
    srand(seed);
}

int main(int argc, char *argv[])
{
    if (argc != 3) // Ensure there are two arguments.
    {
        fprintf(stderr, "Please use the format: <png_seed>  <num_iter>");
        return -1;
    }

    FILE *seed_file = fopen("_seed_", "rb"); // Check whether the _seed_ file is available and present
    if (!seed_file)
    {
        perror("Could not open seed file");
        return 1;
    }

    uint32_t prng_seed = strtoul(argv[1], NULL, 10);      // Set the prng_seed from parameter
    uint32_t num_iterations = strtoul(argv[2], NULL, 10); // Set the num_interations from parameter

    seedPRNG(prng_seed); // Call the function to set the prng_seed for all rand's

    // Take contents of seed file
    fseek(seed_file, 0, SEEK_END);
    size_t seed_size = ftell(seed_file);
    rewind(seed_file);

    // Create a char array to hold the input
    char *input = malloc(seed_size);
    fread(input, 1, seed_size, seed_file);
    fclose(seed_file);
    size_t input_size = seed_size;

    // loop through iterations and the input to randomize the bytes
    for (uint32_t i = 0; i < num_iterations; i++)
    {
        for (uint32_t j = 0; j < input_size; j++)
        {
            if ((rand() % 100) < 13) // Percentage of mutation
            {
                input[j] = randByte(); // If in 13 percent, then randomize the byte
            }
        }

        if ((i + 1) % 500 == 0)
        {
            add_input(&input, &input_size); // After 500 interations, add 10 more bytes to the end of the input
        }
    }

    fwrite(input, 1, input_size, stdout); // Write it out to std for debugging
    return 0;
}
