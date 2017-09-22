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

  FILE * pArquivo1;
  FILE * pArquivo2;
  FILE * pArquivo3;
  FILE * pArquivo4;
  FILE * pArquivo5;
  FILE * pDestino;

  pArquivo1 = fopen ("static.txt", "r");
  pArquivo2 = fopen ("aodv.txt", "r");
  pArquivo3 = fopen ("olsr.txt", "r");
  pArquivo4 = fopen ("dsdv.txt", "r");
  // pArquivo5 = fopen ("vegasLog.txt", "r");
  pDestino = fopen ("todosatraso.txt", "w+");

  if(pArquivo1,pArquivo2,pArquivo4 == NULL)
      printf("Erro, nao foi possivel abrir o arquivo\n");
  else
    while( (fscanf(pArquivo1,"%i %lf\n", &tempo, &dado))!=EOF && (fscanf(pArquivo2,"%i %lf\n", &tempo2, &dado2))!=EOF && (fscanf(pArquivo3,"%i %lf\n", &tempo3, &dado3))!=EOF && (fscanf(pArquivo4,"%i %lf\n", &tempo4, &dado4))!=EOF)
      fprintf(pDestino,"%i %lf %lf %lf %lf\n", tempo, dado, dado2, dado3, dado4);


	// if(pArquivo1,pArquivo2,pArquivo3 == NULL)
	// 		printf("Erro, nao foi possivel abrir o arquivo\n");
	// else
	// 	while( (fscanf(pArquivo1,"%i %lf\n", &tempo, &dado))!=EOF && (fscanf(pArquivo2,"%i %lf\n", &tempo2, &dado2))!=EOF && (fscanf(pArquivo3,"%i %lf\n", &tempo3, &dado3))!=EOF)
	// 		fprintf(pDestino,"%i %lf %lf %lf\n", tempo, dado, dado2, dado3);

  printf ("Finished!\n");
  return 0;
}