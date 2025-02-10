#include "clipboard.h"

char *clipboard() {
  char *rawData = NULL;

  FILE *pipe = popen("pbpaste", "r");
  if (!pipe)
    return NULL;

  rawData = (char *)malloc(BUFFER_SIZE);

  fread(rawData, 1, BUFFER_SIZE, pipe);
  pclose(pipe);

  return strdup(rawData);
}
