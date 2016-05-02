#include "types.h"

int Bool_from_string( char * string)
{
  if ((strcmp(string,"TRUE") == 0) ||
    (strcmp(string,"true") == 0) ||
    (strcmp(string,"True") == 0) ||
    (strcmp(string,"1") == 0))
    return Bool_True;
  if ((strcmp(string,"FALSE") == 0) ||
    (strcmp(string,"false") == 0) ||
    (strcmp(string,"False") == 0) ||
    (strcmp(string,"0") == 0))
    return Bool_False;
  return -1;
}

char *string_from_bool(Bool b)
{
  if (b == Bool_True)
  {
    return "true";
  }
  else if (b == Bool_False)
  {
    return "false";
  }
  else return "ERROR";
}