#include <stdio.h>

int main ()
{
  int tempo;
  double dado;
  int tempo2;
  double dado2;
  int tempo3;
  double dado3;
  int tempo4;
  double dado4;
  int tempo5;
  double dado5;
  int tempo6;
  double dado6;

  FILE * pArquivo1;
  FILE * pArquivo2;
  FILE * pArquivo3;
  FILE * pArquivo4;
  FILE * pArquivo5;
  FILE * pArquivo6;
  FILE * pDestino;

  pArquivo1 = fopen ("aodv2.txt", "r");
  pArquivo2 = fopen ("aodv5.txt", "r");
  pArquivo3 = fopen ("aodv11.txt", "r");
  pArquivo4 = fopen ("aodv6.txt", "r");
  pArquivo5 = fopen ("aodv9.txt", "r");
  pArquivo6 = fopen ("aodv12.txt", "r");
  pDestino = fopen ("todosjitter.txt", "w+");

  if(pArquivo1,pArquivo2,pArquivo4,pArquivo5,pArquivo6 == NULL)
      printf("Erro, nao foi possivel abrir o arquivo\n");
  else
    while( (fscanf(pArquivo1,"%i %lf\n", &tempo, &dado))!=EOF && (fscanf(pArquivo2,"%i %lf\n", &tempo2, &dado2))!=EOF && (fscanf(pArquivo3,"%i %lf\n", &tempo3, &dado3))!=EOF && (fscanf(pArquivo4,"%i %lf\n", &tempo4, &dado4))!=EOF && (fscanf(pArquivo5,"%i %lf\n", &tempo5, &dado5))!=EOF && (fscanf(pArquivo6,"%i %lf\n", &tempo6, &dado6))!=EOF)
      fprintf(pDestino,"%i %lf %lf %lf %lf %lf %lf\n", tempo, dado, dado2, dado3, dado4, dado5, dado6);


	// if(pArquivo1,pArquivo2,pArquivo3 == NULL)
	// 		printf("Erro, nao foi possivel abrir o arquivo\n");
	// else
	// 	while( (fscanf(pArquivo1,"%i %lf\n", &tempo, &dado))!=EOF && (fscanf(pArquivo2,"%i %lf\n", &tempo2, &dado2))!=EOF && (fscanf(pArquivo3,"%i %lf\n", &tempo3, &dado3))!=EOF)
	// 		fprintf(pDestino,"%i %lf %lf %lf\n", tempo, dado, dado2, dado3);

  printf ("Finished!\n");
  return 0;
}