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


void filtreTeste(double* data,double* partie_reelle, double *partie_imaginaire, int N)
{
	cout<<"hé ho";
	for(int i = 0 ; i < N ; i++)
	{
		for(int j = 0 ; j < N ; i++)
		{
			data[i] = data[i] + (cos((i * 2 * j * PI / N)  + atan(partie_imaginaire[i] / partie_reelle[i])) * sqrt((partie_reelle[i]*partie_reelle[i]) + (partie_imaginaire[i] * partie_imaginaire[i])));
		}
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

int main(int argc, char *argv[])
{
  // processOptionsInLine(argc, argv);
  strcpy(fileName,"BrokenGlass.wav");
   Wave * f = new Wave();
   f->read(fileName);
  
  long int ech = f->sampling_freq;
  int echSec = ech * 1;
  unsigned char * data = (unsigned char *)malloc(echSec * sizeof(unsigned char));
  
   f->getData8(&data,&echSec);
  
  // double teta = 0;
  // double dteta = 2 * PI * 440 / ech;
	
  // for(int i = 0 ; i < echSec ; i++)
    // {
      // data[i] = (unsigned char)((sin(teta)+1)*127.0);
      // teta += dteta;
	  // if(teta >= 2*PI)
		// teta = 0;
    // } 
	
  double data_reel[echSec];
  double data_imaginaire[echSec];
  
//filtreTeste(data_reel,data_imaginaire,echSec);
  cout << "Début DTF" << endl;
	DFT(data,data_reel,data_imaginaire,echSec);	
	for(int i =0; i < echSec; i++)
	{
		data[i] = (unsigned char)((int)(data_reel[i] - data_imaginaire[i])/echSec);
	}
	
	for(int i=1; i < echSec; i++)
	{
		if((i > 1000/(ech/echSec) && i < echSec/2) || (i > echSec/2 && i < (echSec-1000)/(ech/echSec)))
		{
			data_reel[i] = 0;
			data_imaginaire[i] = 0;
		}
	}
	for(int i =0; i < echSec; i++)
	{
		data[i] = (unsigned char)((int)(data_reel[i] - data_imaginaire[i])/echSec);
	}
	
  cout << "Fin DTF et Début IDTF" << endl;
	IDFT(data,data_reel,data_imaginaire,echSec);
  cout << "Fin IDTF" << endl;

   Wave * w = new Wave(data,echSec,(short)1,ech);
   strcpy(fileName,"BrokenGlass2.wav");
   w->write(fileName);
  //delete data;
   delete w;
}