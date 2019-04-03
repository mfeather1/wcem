#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

char moves[1000][55];
char cubes[1000][54];

void do_move(), show(), show_cube(), populate_moves();
int cvt_mv();

int main(argc, argv)
  int argc;
  char **argv;
{
  int n;
  char *p;

  if (argc != 3)
  {
    printf("Usage: %s {cube string} {move list in quotes}\n", argv[0]);
    exit(0);
  }

  populate_moves();
  strncpy(cubes[0], argv[1], 54);
  // show_cube(cubes[0]);

  p = strtok (argv[2], " ");
  for (n=0; p != NULL; n++)
    {
      //printf("%s %d\n", p, cvt_mv(p));
      do_move(cvt_mv(p), cubes[n], cubes[n+1]);
      p = strtok (NULL, " ");
      if (n > 1000)
      { 
         printf("move list limited to 1000 moves\n");
         exit(0);
      } 
    }

  // show_cube(cubes[n]);
  show(cubes[n]);
  exit(0);
}

void show_cube(s)
     char *s;
{	
  int i, j;

  for (i=0; i < 9; i+=3)
    printf("    %c%c%c\n",   s[i], s[i+1], s[i+2]);

  for (; i < 45; i+=12)
    {
      for (j=0;  j < 12; j+=3)
	printf("%c%c%c ", s[i+j], s[i+j+1], s[i+j+2]);
      printf("\n");
    }

  for (; i < 54; i+=3)
    printf("    %c%c%c\n",   s[i], s[i+1], s[i+2]);

  printf("\n");
}

/*****************************************************************************/

// Right Twist CW
char R[54] = { 
   0, 1,14, 3, 4,26, 6, 7,38, 9,10,11,12,13,47,39,27,15,
   8,19,20,21,22,23,24,25,50,40,28,16, 5,31,32,33,34,35,
  36,37,53,41,29,17, 2,43,44,45,46,42,48,49,30,51,52,18
};

// Up Twist CW
char U[54] = { 
   6, 3, 0, 7, 4, 1, 8, 5, 2,12,13,14,15,16,17,18,19,20,
   9,10,11,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,
  36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53
};
 
// Front Twist CW
char F[54] = { 
   0, 1, 2, 3, 4, 5,35,23,11, 9,10,45,36,24,12, 6,16,17,
  18,19,20,21,22,46,37,25,13, 7,28,29,30,31,32,33,34,47,
  38,26,14, 8,40,41,42,43,44,39,27,15,48,49,50,51,52,53
};
  
// UD Slice Twist CW
char Um[54] = { 
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,
  18,19,20,24,25,26,27,28,29,30,31,32,21,22,23,33,34,35,
  36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53
};
  
// FB Slice Twist CW
char Fm[54] = { 
   0, 1, 2,34,22,10, 6, 7, 8, 9,48,11,12,13,14,15, 3,17,
  18,19,20,21,49,23,24,25,26,27, 4,29,30,31,32,33,50,35,
  36,37,38,39, 5,41,42,43,44,45,46,47,40,28,16,51,52,53
};

// LR Slice Twist CW
char Rm[54] = { 
   0,13, 2, 3,25, 5, 6,37, 8, 9,10,11,12,46,14,15,16,17,
  18, 7,20,21,22,23,24,49,26,27,28,29,30, 4,32,33,34,35,
  36,52,38,39,40,41,42, 1,44,45,43,47,48,31,50,51,19,53
};

// Mirror Right-Left
char RL[54] = {
   2, 1, 0, 5, 4, 3, 8, 7, 6,17,16,15,14,13,12,11,10, 9,
  20,19,18,29,28,27,26,25,24,23,22,21,32,31,30,41,40,39,
  38,37,36,35,34,33,44,43,42,47,46,45,50,49,48,53,52,51
};  

// Mirror Up-Down
char UD[54] = {
  51,52,53,48,49,50,45,46,47,33,34,35,36,37,38,39,40,41,
  42,43,44,21,22,23,24,25,26,27,28,29,30,31,32, 9,10,11,
  12,13,14,15,16,17,18,19,20, 6, 7, 8, 3, 4, 5, 0, 1, 2
};

// Mirror Front-Back
char FB[54] = {
   6, 7, 8, 3, 4, 5, 0, 1, 2,11,10, 9,20,19,18,17,16,15,
  14,13,12,23,22,21,32,31,30,29,28,27,26,25,24,35,34,33,
  44,43,42,41,40,39,38,37,36,51,52,53,48,49,50,45,46,47
};

// Sym X
char X[54] = {
  12,13,14,24,25,26,36,37,38,11,23,35,45,46,47,39,27,15,
   8, 7, 6,10,22,34,48,49,50,40,28,16, 5, 4, 3, 9,21,33,
  51,52,53,41,29,17, 2, 1, 0,44,43,42,32,31,30,20,19,18
};

// Sym Y
char Y[54] = {
  33,21, 9,34,22,10,35,23,11,51,48,45,36,24,12, 6, 3, 0,
  20,32,44,52,49,46,37,25,13, 7, 4, 1,19,31,43,53,50,47,
  38,26,14, 8, 5, 2,18,30,42,39,27,15,40,28,16,41,29,17
};

// Sym Z
char Z[54] = {
   6, 3, 0, 7, 4, 1, 8, 5, 2,12,13,14,15,16,17,18,19,20, 
   9,10,11,24,25,26,27,28,29,30,31,32,21,22,23,36,37,38,
  39,40,41,42,43,44,33,34,35,47,50,53,46,49,52,45,48,51
};

int cvt_mv(s)
char *s;
{
  // make CCW half-twists CW
  if (strlen(s) == 3 && s[1] == '2' && s[2] == '\'') s[2] = '\0';

  if (strcmp(s, "R") == 0)  return 0;
  if (strcmp(s, "R2") == 0) return 1;
  if (strcmp(s, "R'") == 0) return 2;
  if (strcmp(s, "L") == 0)  return 3;
  if (strcmp(s, "L2") == 0) return 4;
  if (strcmp(s, "L'") == 0) return 5;

  if (strcmp(s, "U") == 0)  return 6;
  if (strcmp(s, "U2") == 0) return 7;
  if (strcmp(s, "U'") == 0) return 8;
  if (strcmp(s, "D") == 0)  return 9;
  if (strcmp(s, "D2") == 0) return 10;
  if (strcmp(s, "D'") == 0) return 11;

  if (strcmp(s, "F") == 0)  return 12;
  if (strcmp(s, "F2") == 0) return 13;
  if (strcmp(s, "F'") == 0) return 14;
  if (strcmp(s, "B") == 0)  return 15;
  if (strcmp(s, "B2") == 0) return 16;
  if (strcmp(s, "B'") == 0) return 17;

  if (strcmp(s, "Um") == 0)  return 18;
  if (strncmp(s, "Um2", 3) == 0) return 19;
  if (strcmp(s, "Um'") == 0) return 20;

  if (strcmp(s, "Fm") == 0)  return 21;
  if (strncmp(s, "Fm2", 3) == 0) return 22;
  if (strcmp(s, "Fm'") == 0) return 23;
  
  if (strcmp(s, "Rm") == 0)  return 24;
  if (strncmp(s, "Rm2", 3) == 0) return 25;
  if (strcmp(s, "Rm'") == 0) return 26;

  if (strcmp(s, "X") == 0)  return 27;
  if (strcmp(s, "X2") == 0) return 28;
  if (strcmp(s, "X'") == 0) return 29;
  
  if (strcmp(s, "Y") == 0)  return 30;
  if (strcmp(s, "Y2") == 0) return 31;
  if (strcmp(s, "Y'") == 0) return 32;
  
  if (strcmp(s, "Z") == 0)  return 33;
  if (strcmp(s, "Z2") == 0) return 34;
  if (strcmp(s, "Z'") == 0) return 35;

  if (strcmp(s, "E") == 0)  return 18;
  if (strncmp(s, "E2", 3) == 0) return 19;
  if (strcmp(s, "E'") == 0) return 20;

  if (strcmp(s, "S") == 0)  return 21;
  if (strncmp(s, "S2", 3) == 0) return 22;
  if (strcmp(s, "S'") == 0) return 23;
  
  if (strcmp(s, "M") == 0)  return 24;
  if (strncmp(s, "M2", 3) == 0) return 25;
  if (strcmp(s, "M'") == 0) return 26;

  printf("Invalid Move: %s\n", s);
  exit(0);
}

void populate_moves()
{
  int i;

  for (i=0; i < 54; i++) moves[0][i] = R[i];
  for (i=0; i < 54; i++) moves[1][i] = R[moves[0][i]];
  for (i=0; i < 54; i++) moves[2][i] = R[moves[1][i]];

  for (i=0; i < 54; i++) moves[3][i] = RL[moves[2][RL[i]]];
  for (i=0; i < 54; i++) moves[4][i] = RL[moves[1][RL[i]]];
  for (i=0; i < 54; i++) moves[5][i] = RL[moves[0][RL[i]]];
  
  for (i=0; i < 54; i++) moves[6][i] = U[i];
  for (i=0; i < 54; i++) moves[7][i] = U[moves[6][i]];
  for (i=0; i < 54; i++) moves[8][i] = U[moves[7][i]];

  for (i=0; i < 54; i++) moves[9][i]  = UD[moves[8][UD[i]]];
  for (i=0; i < 54; i++) moves[10][i] = UD[moves[7][UD[i]]];
  for (i=0; i < 54; i++) moves[11][i] = UD[moves[6][UD[i]]];

  for (i=0; i < 54; i++) moves[12][i] = F[i];
  for (i=0; i < 54; i++) moves[13][i] = F[moves[12][i]];
  for (i=0; i < 54; i++) moves[14][i] = F[moves[13][i]];

  for (i=0; i < 54; i++) moves[15][i] = FB[moves[14][FB[i]]];
  for (i=0; i < 54; i++) moves[16][i] = FB[moves[13][FB[i]]];
  for (i=0; i < 54; i++) moves[17][i] = FB[moves[12][FB[i]]];

  for (i=0; i < 54; i++) moves[18][i] = Um[i]; 
  for (i=0; i < 54; i++) moves[19][i] = Um[moves[18][i]];  
  for (i=0; i < 54; i++) moves[20][i] = Um[moves[19][i]];

  for (i=0; i < 54; i++) moves[21][i] = Fm[i]; 
  for (i=0; i < 54; i++) moves[22][i] = Fm[moves[21][i]];  
  for (i=0; i < 54; i++) moves[23][i] = Fm[moves[22][i]];

  for (i=0; i < 54; i++) moves[24][i] = Rm[i]; 
  for (i=0; i < 54; i++) moves[25][i] = Rm[moves[24][i]];  
  for (i=0; i < 54; i++) moves[26][i] = Rm[moves[25][i]];

  for (i=0; i < 54; i++) moves[27][i] = X[i]; 
  for (i=0; i < 54; i++) moves[28][i] = X[moves[27][i]];
  for (i=0; i < 54; i++) moves[29][i] = X[moves[28][i]];

  for (i=0; i < 54; i++) moves[30][i] = Y[i]; 
  for (i=0; i < 54; i++) moves[31][i] = Y[moves[30][i]];
  for (i=0; i < 54; i++) moves[32][i] = Y[moves[31][i]];

  for (i=0; i < 54; i++) moves[33][i] = Z[i]; 
  for (i=0; i < 54; i++) moves[34][i] = Z[moves[33][i]];
  for (i=0; i < 54; i++) moves[35][i] = Z[moves[34][i]];
}

void do_move(mv, s1, s2)
char mv, *s1, *s2;
{
  int i;
  for (i=0; i < 54; i++)
    s2[i] = s1[moves[mv][i]];
}

void show(s)
char *s;
{
  int i;
  for (i=0; i < 54; i++)
    printf("%c", s[i]);
  // printf("  ");
  printf("\n");
}
