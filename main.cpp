#include <cstdlib>
#include <iostream>

using namespace std;
#include "Wave.hpp"
#include <math.h>
#include "string.h"

#define PI (3.141592653589793)

char fileName[200]; // Fichier wave

//==============================================================================
void printUsage(char* name) {

  cout << "\nUsage : "
       <<name
       <<" fichier.wav\n";
  exit(-1);
}

//==============================================================================
void processOptionsInLine(int argc, char** argv){
  if (argc != 2) {
    printUsage(argv[0]);
  }
  //RECOPIE DU NOM DE L'IMAGE DANS LA VARIABLE seqName
  strcpy(fileName, argv[1]);
}

void DFT(unsigned char *signal, double *partie_reelle, double *partie_imaginaire, int N)
{
	double reel;
	double imaginaire;
	for(int i = 0; i < N-1; i++)
	{
		reel = 0;
		imaginaire = 0;
		for(int n = 0; n < N-1; n++)
		{
			reel += (double)((int)signal[n]) * cos( 2 * PI * ( n * i ) / N );
			imaginaire += (double)((int)signal[n]) * sin( 2 * PI * ( n * i ) / N );
		}
		imaginaire *= -1;
		partie_reelle[i] = reel;
		partie_imaginaire[i] = imaginaire;
	}
}

void IDFT(unsigned char *signal, double *partie_reelle, double *partie_imaginaire, int N)
{
	for(int i = 0; i < N-1; i++)
	{
		signal[i] = 0;
		for(int n = 0; n < N-1; n++)
		{
			signal[i] += (unsigned char)((int)((partie_reelle[n] * cos( 2 * PI * (i * n) / N ) - partie_imaginaire[n] * sin( 2 * PI * (i * n) / N ))/N));
		}
	}
}

int main(int argc, char *argv[]) {

  // processOptionsInLine(argc, argv);
  strcpy(fileName,"La.wav");
  long int echSec = 10000;
  unsigned char data[echSec];
  
  double teta = 0;
  double dteta = 2 * PI/(echSec/440);

  for(int i = 0 ; i < echSec ; i++)
    {
      data[i] = (unsigned char)((sin(teta)+1)*127.0);
      teta += dteta;
	  if(teta >= 2*PI)
		teta = 0;
    }
	for(int i = 0; i < 10; i++)
	{
		cout << (double)((int)data[i]) << endl;
	}
  double data_reel[echSec];
  double data_imaginaire[echSec];
  cout << "Début DTF" << endl;
  DFT(data,data_reel,data_imaginaire,echSec);
  cout << "Fin DTF et Début IDTF" << endl;
  IDFT(data,data_reel,data_imaginaire,echSec);
  cout << "Fin IDTF" << endl;
  	for(int i = 0; i < 10; i++)
	{
		cout << (double)((int)data[i]) << endl;
	}
   Wave * w = new Wave(data,echSec,(short)1,echSec);
   w->write(fileName);
  //delete data;
   delete w;
}