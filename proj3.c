#include <fcntl.h>  
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/stat.h> 
#include <sys/time.h>
#include <sys/types.h> 
#include <unistd.h> 

/**
 * This function prototype declares which will compare files byte to byte.
 * It takes two file paths as constant character pointers for an comparison.
 */
void firstStep(const char *, const char *);

/**
 * This function prototype declares the secondStep which compares files using memory buffers.
 * It takes two file paths as constant character pointers for array-based comparison.
 */
void secondStep(const char *, const char *);

/**
 * The main function serves as the program entry point for file comparison.
 * It accepts command line arguments and coordinates the comparison process.
 */
int main(int argc, char * argv[]) {
    // This conditional checks if exactly two filenames were provided as arguments.
    if (argc != 3) {
        // This prints the correct usage format when arguments are missing.
        printf("Usage: proj3.out <file1> <file2>\n");
        // This terminates the program with failure status for incorrect usage.
        exit(EXIT_FAILURE);
    }

    // This attempts to open the first input file in read only mode.
    int file1 = open(argv[1], O_RDONLY);
    // This attempts to open the second input file in read only mode.
    int file2 = open(argv[2], O_RDONLY);

    // This checks if either file failed to open properly.
    if (file1 == -1 || file2 == -1) {
        // This displays an error message when the file reading fails.
        perror("There was an error reading a file");
        // This exits the program if the files cannot be accessed.
        exit(EXIT_FAILURE);
    }

    // This closes the first input file after the verification.
    close(file1);
    // This closes the second input file after the verification.
    close(file2);

    // This calls the firstStep to perform byte to byte file comparison.
    firstStep(argv[1], argv[2]);
    // This calls secondStep to perform memory buffer file comparison.
    secondStep(argv[1], argv[2]);

    // This returns 0, which indicates a successful program execution.
    return 0;
}

void firstStep(const char * argv1, const char * argv2) {
    // This declares start and end timeval structures for timing measurement.
    struct timeval start, end;

    // This records the starting time before comparison begins.
    gettimeofday(&start, NULL);

    // These declare file descriptors for both input and output files.
    int file1;
    int file2;
    int file3;

    // This opens the first input file for reading.
    file1 = open(argv1, O_RDONLY);
    // This opens the second input file for reading.
    file2 = open(argv2, O_RDONLY);
    // This opens the output file for writing differences from file1.
    file3 = open("differencesFoundInFile1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // This checks if any file failed to open.
    if (file1 == -1 || file2 == -1 || file3 == -1) {
        perror("Error opening files in firstStep");
        exit(EXIT_FAILURE);
    }

    // This reads the first byte from each input file for comparison.
    char characterFile1, characterFile2;
    ssize_t read1, read2;

    // This loop continues until reaching the end of file1.
    while ((read1 = read(file1, &characterFile1, 1)) > 0) {
        read2 = read(file2, &characterFile2, 1);
        // This checks if the current bytes from both files differ.
        if (read2 <= 0 || characterFile1 != characterFile2) {
            // This writes differing bytes from file1 to the output file.
            write(file3, &characterFile1, 1);
        }
    }

    // This closes the first input file after processing.
    close(file1);
    // This closes the second input file after processing.
    close(file2);
    // This closes the output file after writing all differences.
    close(file3);

    // This records the ending time after comparison completes.
    gettimeofday(&end, NULL);
    // This calculates and prints the execution time in milliseconds.
    printf("Step 1 took %f milliseconds\n", 
          ((end.tv_sec - start.tv_sec) * 1000.0) + 
          ((end.tv_usec - start.tv_usec) / 1000.0));
}

void secondStep(const char * argv1, const char * argv2) {
    // This declares start and end timeval structures for timing measurement.
    struct timeval start, end;

    // This records the starting time before comparison begins.
    gettimeofday(&start, NULL);

    // These declare file descriptors for both input and output files.
    int file1;
    int file2;
    int file3;

    // This opens the first input file for reading.
    file1 = open(argv1, O_RDONLY);
    // This opens the second input file for reading.
    file2 = open(argv2, O_RDONLY);
    // This opens the output file for writing differences from file2.
    file3 = open("differencesFoundInFile2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // This checks if any file failed to open.
    if (file1 == -1 || file2 == -1 || file3 == -1) {
        perror("Error opening files in secondStep");
        exit(EXIT_FAILURE);
    }

    // These declare stat structures to store file metadata.
    struct stat statFile1, statFile2;

    // This retrieves metadata about the first input file.
    stat(argv1, &statFile1);
    // This retrieves metadata about the second input file.
    stat(argv2, &statFile2);

    // This stores the size in bytes of the first input file.
    int sizeFile1 = statFile1.st_size;
    // This stores the size in bytes of the second input file.
    int sizeFile2 = statFile2.st_size;

    // This allocates memory for storing file1's contents.
    char * arrayFile1 = malloc(sizeFile1);
    // This allocates memory for storing file2's contents.
    char * arrayFile2 = malloc(sizeFile2);

    // This checks if memory allocation succeeded.
    if (arrayFile1 == NULL || arrayFile2 == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // This reads file1's entire content into the allocated array.
    if (read(file1, arrayFile1, sizeFile1) != sizeFile1) {
        perror("Error reading file1");
        exit(EXIT_FAILURE);
    }

    // This reads file2's entire content into the allocated array.
    if (read(file2, arrayFile2, sizeFile2) != sizeFile2) {
        perror("Error reading file2");
        exit(EXIT_FAILURE);
    }

    // This determines the smaller file size for safe comparison.
    int minSize = (sizeFile1 < sizeFile2) ? sizeFile1 : sizeFile2;

    // This loop compares each corresponding byte in both arrays.
    for (int i = 0; i < minSize; i++) {
        // This checks if the current bytes from both arrays differ.
        if (arrayFile1[i] != arrayFile2[i]) {
            // This writes differing bytes from file2 to the output file.
            write(file3, &arrayFile2[i], 1);
        }
    }

    // This releases memory allocated for file1's content array.
    free(arrayFile1);
    // This releases memory allocated for file2's content array.
    free(arrayFile2);
    // This closes the first input file after processing.
    close(file1);
    // This closes the second input file after processing.
    close(file2);
    // This closes the output file after writing all differences.
    close(file3);

    // This records the ending time after the comparison completes.
    gettimeofday(&end, NULL);
    // This calculates and prints the execution time in milliseconds.
    printf("Step 2 took %f milliseconds\n", 
          ((end.tv_sec - start.tv_sec) * 1000.0) + 
          ((end.tv_usec - start.tv_usec) / 1000.0));
}