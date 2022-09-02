#include <stdio.h>
#include <stdlib.h>

typedef struct Buffer
{
  long int len;
  char *array;
} Buffer;

Buffer *newBuffer (void)
{
  Buffer *ret = malloc (sizeof(Buffer));
  ret->len = 0;
  ret->array = malloc (0);
  return ret;
}

void bufferAppend (Buffer *b, const char c)
{
  const long int newlen = b->len+1;
  b->array = realloc (b->array, newlen);
  b->array[b->len] = c;
  b->len = newlen;
}

int main (int argc, char **argv)
{
  if (argc<2)
  {
    fprintf (stderr, "Which file I should convert?\nPut the name of the file to be converted as my argument.\n");
    exit (EXIT_FAILURE);
  }
  char *filename = argv[1];
  FILE *file = fopen (filename, "rb+");
  Buffer *buffer = newBuffer();
  while (!(feof(file)||ferror(file)))
  {
    const int C = fgetc (file);
    if (C==EOF)
    {
      if (ferror(file))
      {
        perror ("fgetc");
        exit (EXIT_FAILURE);
      }
      if (feof(file))
      {
        break;
      }
    }
    const char c = C;
    if (c=='\r')
    {
      const int D = fgetc (file);
      if (D==EOF)
      {
        if (feof(file))
        {
          bufferAppend (buffer, c);
          break;
        }
        if (ferror(file))
        {
          perror ("fgetc");
          exit (EXIT_FAILURE);
        }
      }
      const char d = D;
      if (d=='\n')
      {
        bufferAppend (buffer, d);
        continue;
      }
      else
      {
        ungetc (d, file);
        bufferAppend (buffer, c);
        continue;
      }
    }
    else
    {
      bufferAppend (buffer, c);
    }
  }
  rewind (file);
  fwrite (buffer->array, 1, buffer->len, file);
  fclose (file);
  return EXIT_SUCCESS;
}
