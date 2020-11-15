/* ========================================================================= *
 * File parsing and Main Function

 * ------------------------------------------------------------------------- *
 * NOM
 *      file_intersect
 * SYNOPSIS
 *      file_intersect fileA fileB
 * DESCIRPTION
 *      prints the lines that can be found in both fileA and fileB
 * USAGE
 *      ./file_intersect French.txt English.txt
 *          will print the lines that are both French.txt and English.txt
 * ------------------------------------------------------------------------- *
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "StringArray.h"



/* Constant */
static const size_t MAX_LINE_SIZE = 128;

/* ------------------------------------------------------------------------- *
 * Read a line of characters from the file stream $file$ and returns the data
 * as an array of char. A line is defined as a sequence of characters followed
 * by a line feed \n. Lines containing more than $MAX_LINE_SIZE$ characters
 * are splitted into smaller lines of size of at most $MAX_LINE_SIZE$. Empty
 * lines are skipped.
 *
 * The array must later be deleted by calling free().
 *
 * PARAMETER
 * file         File stream
 *
 * RETURN
 * string       A new pointer to an array of characters that represents the
 *              line read from the file stream $file$.
 * NULL         The function returns NULL when it has reached the end of the
 *              file. On error, the function also returns NULL.
 * ------------------------------------------------------------------------- */
static char* readLine(FILE* file);

/* ------------------------------------------------------------------------- *
 * Create a StringArray object from the content of the file $fileName$.
 * Each line of $fileName$ is stored as an element of the StringArray object.
 *
 * The Array object must later be deleted by calling freeArray().
 *
 * PARAMETER
 * fileName         Name of a file
 *
 * RETURN
 * array            A new pointer to an StringArray object
 * NULL             If an error has occured. The function produces an error
 *                  message on the standard error output.
 * ------------------------------------------------------------------------- */
static StringArray* createArrayFromFile(const char* fileName);

/* ------------------------------------------------------------------------- *
 * Print the content of the StringArray object $array$ on the standard output.
 *
 * PARAMETER
 * array            An StringArray object
 * ------------------------------------------------------------------------- */
static void printArray(const StringArray* array);



static char* readLine(FILE* file)
{
  // Read a line
  char* line = (char*)calloc(MAX_LINE_SIZE, sizeof(char));
  if (!line)
    return NULL;

  if (!fgets(line, MAX_LINE_SIZE - 1, file))
  {
    free(line);
    return NULL;
  }

  // Remove trailing newline
  size_t lastCharIndex = strlen(line) - 1;
  if (line[lastCharIndex] == '\n')
    line[lastCharIndex--] = '\0';

  // Skip empty line
  if (lastCharIndex == (size_t)-1)
  {
    free(line);
    return readLine(file);
  }

  // Adjust memory size
  line = realloc(line, (lastCharIndex + 2) * sizeof(char));
  return line;
}


static StringArray* createArrayFromFile(const char* fileName)
{
  // Open file
  FILE* file = fopen(fileName, "r");
  if (!file)
  {
    fprintf(stderr, "Error while loading file '%s': %s\n",
            fileName, strerror(errno));
    return NULL;
  }

  // Create the Array
  StringArray* result = createEmptyArray();
  if (!result)
  {
    fprintf(stderr, "Memory allocation error while reading file %s\n", fileName);
    return NULL;
  }


  // Read file line by line
  char* line = readLine(file);
  while (line != NULL)
  {
    if (!insertInArray(result, line))
    {
      fprintf(stderr, "Memory allocation error while reading file %s\n", fileName);
      freeArray(result, true);
      return NULL;
    }

    line = readLine(file);
  }

  fclose(file);
  return result;
}


void printArray(const StringArray* array)
{
  for (size_t i = 0; i < arrayLength(array); i++)
    printf("%s\n", getElementInArray(array, i));
}





int main(int argc, char** argv)
{
  // Check arguments
  if (argc != 3)
  {
    printf("Usage: %s <File> <File>\n", argv[0]);
    return -1;
  }

  // Load files
  StringArray* firstArray = createArrayFromFile(argv[1]);
  if (!firstArray)
    return -1;

  StringArray* secondArray = createArrayFromFile(argv[2]);
  if (!secondArray)
  {
    freeArray(firstArray, true);
    return -1;
  }

  // Compute intersection
  StringArray* intersection = getIntersection(firstArray, secondArray);
  if (!intersection)
  {
    printf("An error has occured while computing the files' intersection: \n");
    printf("- %s\n", argv[1]);
    printf("- %s\n", argv[2]);
    freeArray(firstArray, true);
    freeArray(secondArray, true);
    return 3;
  }

  printArray(intersection);

  // Free memory
  freeArray(firstArray, true);
  freeArray(secondArray, true);
  freeArray(intersection, false);

  return 0;
}
