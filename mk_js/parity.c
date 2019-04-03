#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int lo,  hi;
int val(), err(), parity();
int main(int argc, char **argv)
{
  int i, n, len; 
  char s[100];
  if (argc != 2) { 
    printf("Usage: %s {Permutation String}\n", argv[0]);
    exit(0);
  }
  len = strlen(argv[1]);
  for (i=0; i < len; i++) 
    s[i] = argv[1][i];
  lo = 256; 
  hi = 0;
  for (i=0; i < len; i++) {
   if (s[i] < lo) lo = s[i];
   if (s[i] > hi) hi = s[i];
  }
  for (i=0; i < len; i++)
    s[i] -= lo;
  if (val(s, len)) 
    err(1);
  n = parity(s, len);
  printf("%d\n", n);
}

int err(int n) {
  printf("ERROR: Invalid Permutation String\n");
  exit(0);
}

int val(char *s, int n) {
  int i;
  char vv[256];
  for (i=0; i < n; i++) vv[i] = 0;
  for (i=0; i < n; i++) vv[s[i]] = 1;
  for (i=0; i < n; i++)
    if (vv[i] != 1)
      return(1);
  return(0);
}

int parity(char *s, int len) {
  int i, j, n;
  char x, tmp[100];
  for (i = 0; i < len; i++)
    tmp[i] = s[i];
  for (i=n=0; i < len; i++) {
    for (j=i+1; j < len; j++)
      if (tmp[j] == i) {
        tmp[j] = tmp[i];
        tmp[i] = i;
        n++;
      }
  }
  return(n%2);
}
