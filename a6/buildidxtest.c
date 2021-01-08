#include <stdio.h>
#include <stdlib.h>

int main( int argc, char **argv )
{
  int i;

  FILE *fp = fopen( argv[1], "rb" );
  if (!fp)
  {
    fprintf( stderr, "fopen error %s\n", argv[1] );
    exit(-1);
  }

  fseek( fp, atol(argv[2])*sizeof(int), SEEK_SET );
  fread( &i, sizeof(int), 1, fp );

  printf( "%d\n", i );
  return 0;
}
