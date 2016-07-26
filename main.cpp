#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <math.h>
#include <conio.h>
using namespace std;


///////////////////////////////
class Neur {
public: 
float w[16];
float x[16];
float b;
float beta;
 //funkcija aktivacii 
float NET()
    {
    int count=sizeof(x)/sizeof(x[0]);
    float NET=0;
    for (int i=0;i<count;i++) NET=NET+x[i]*w[i];
    return(NET+b);
    }
  

float OUT()
    {
    float OUT=0;
    OUT=1/(1+exp(-1*NET()));
    return (OUT);
    }
    
float dOUT()
      {         
      return(OUT()*(1-OUT()));  
      }
//////////////////////////

//vvod dannyx
float in(float vect[])
{
int count=sizeof(x)/sizeof(x[0]);
    for (int i=0;i<count;i++) x[i]=vect[i];
}


};

Neur neuron[2][16];
//////////////////////////////////////



// Klass seti
class Net {
public:   

  float q;
 //funkcija aktivacii dlia vsej seti  
float OUT()
    {
  float OUT;
  int neurons=0;
  int layers=0;
  int sinaps=0;
  neurons=(sizeof(neuron)/sizeof(neuron[0][0]))/(sizeof(neuron)/sizeof(neuron[0]));
  layers=sizeof(neuron)/sizeof(neuron[0]);  
  sinaps=sizeof(neuron[0][0].x)/sizeof(neuron[0][0].x[0]);

if (layers==2)
{
        for (int z=0;z<neurons;z++)
            {             
            neuron[1][0].x[z]=neuron[0][z].OUT();     
            }
}

if (layers==3)
{
        for (int y=0;y<neurons;y++)
            {         
            for (int z=0;z<neurons;z++)
                {             
                neuron[1][y].x[z]=neuron[0][z].OUT();     
                }
            }
        for (int z=0;z<neurons;z++)
            {             
            neuron[2][0].x[z]=neuron[1][z].OUT();     
            }
        
            
}

if (layers==4)
{
        for (int y=0;y<neurons;y++)
            {         
            for (int z=0;z<neurons;z++)
                {             
                neuron[1][y].x[z]=neuron[0][z].OUT();     
                }
            }
        for (int y=0;y<neurons;y++)
            {         
            for (int z=0;z<neurons;z++)
                {             
                neuron[2][y].x[z]=neuron[1][z].OUT();     
                }
            }
        for (int z=0;z<neurons;z++)
            {             
            neuron[3][0].x[z]=neuron[2][z].OUT();     
            }
        
            
}
            

    OUT=neuron[layers-1][0].OUT();
    return OUT;
    }
//////////////////////////
       
//funkcija zadaci randomnyx vesov
float randx()
{
  int neurons=0;
  int layers=0;
  int sinaps=0;
  neurons=(sizeof(neuron)/sizeof(neuron[0][0]))/(sizeof(neuron)/sizeof(neuron[0]));
  layers=sizeof(neuron)/sizeof(neuron[0]);  
  sinaps=sizeof(neuron[0][0].x)/sizeof(neuron[0][0].x[0]);

for (int i=0;i<layers;i++)
    {
    for (int y=0;y<neurons;y++)
        {
        for (int z=0;z<sinaps;z++)
            {             
            neuron[i][y].x[z]=(float)(rand() % 10)/1000;  
            neuron[i][y].w[z]=(float)(rand() % 10)/1000;
            }
        }
    }
}
/////////////////////////


//vvod dannyx
float in(float vect[])
{
      
  int neurons=0;
  int layers=0;
  int sinaps=0;

  neurons=(sizeof(neuron)/sizeof(neuron[0][0]))/(sizeof(neuron)/sizeof(neuron[0]));
  layers=sizeof(neuron)/sizeof(neuron[0]);  
  sinaps=sizeof(neuron[0][0].x)/sizeof(neuron[0][0].x[0]); 
    cout<<"layers "<< layers<<"\n";
    cout<<"neurons "<< neurons<<"\n";
    cout<<"sinaps "<< sinaps<<"\n";
   
    for (int y=0;y<neurons;y++)
        {
        for (int z=0;z<neurons;z++)
            {             
            neuron[0][y].x[z]=vect[z];  
            }
        }
}
////////////////////      

float uchenie(float vect[], float rez)
{
      
  int neurons=0;
  int layers=0;
  int sinaps=0;
  float nu=0.05;
   float nub=0.08;
  
 neurons=(sizeof(neuron)/sizeof(neuron[0][0]))/(sizeof(neuron)/sizeof(neuron[0]));
 layers=sizeof(neuron)/sizeof(neuron[0]);  
 sinaps=sizeof(neuron[0][0].x)/sizeof(neuron[0][0].x[0]); 

  
in(vect);

// obnulenie bet*************************
for (int y=layers-2;y>=0;y--)
{ 
  for (int i=0;i<neurons;i++)
  {
 neuron[y][i].beta=0;
  }
}

 neuron[layers-1][0].beta=(rez-OUT()); 


 //Pogreshnosti
for (int y=layers-2;y>=0;y--)
{ 
  for (int i=0;i<neurons;i++)
  {
   for (int z=0;z<sinaps;z++)
   {
   neuron[y][i].beta= neuron[y][i].beta + neuron[y+1][i].beta*neuron[y+1][i].w[z];
   //cout<<"sinaps="<<z<<" neuron= "<<i<<" Layers= "<<y<<"\n";
  // getch();
   }
  }    
}

 cout <<"rez= "<<rez<<" Out="<<OUT()<<" Beta="<<neuron[layers-1][0].beta;
  cout << "\n";
  
  //popravki


 for (int y=0;y<layers;y++)
{ 
  for (int z=0;z<neurons;z++)
  {
  for (int i=0;i<sinaps;i++)
      {
      neuron[y][z].w[i]= neuron[y][z].w[i] + nu*neuron[y][z].beta*neuron[y][z].dOUT()*neuron[y][z].x[i];
      }
  }
}

 for (int y=0;y<layers;y++)
{ 
  for (int z=0;z<neurons;z++)
  {
      neuron[y][z].b= neuron[y][z].b + nub*neuron[y][z].beta*neuron[y][z].dOUT();
  }
}

 cout << "\n"; 

}

                  

/////////////////
};
Net net;


///////////////////////////////


//Glavnaja funkcija
int main(int argc, char *argv[])
{ 

  
//inicializacija
srand(time(0));
net.randx();
////////////////////////////////////////////////////////
getch();
float vect[64];
float temp;
float Wt=0;
float Wmax=2;
float Z=0;
float Zi=200;
int u=0;
//Nachalo cikla obuchenija
//****************************************************************

for (long i=0;i<50000;i++)
{

float vect01[]= 
                   {
                    1, 1, 1, 1,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                   };
net.uchenie(vect01,0);


float vect02[]= 
                   {
                    0, 0, 0, 0,
                    1, 1, 1, 1,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                   };
net.uchenie(vect02,0);

float vect03[]= 
                   {
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    1, 1, 1, 1,
                    0, 0, 0, 0,
                   };
net.uchenie(vect03,0);

float vect11[]= 
                   {
                    1, 0, 0, 0,
                    1, 0, 0, 0,
                    1, 0, 0, 0,
                    1, 0, 0, 0,
                   };
net.uchenie(vect11,1);

float vect12[]= 
                   {
                    0, 0, 0, 1,
                    0, 0, 0, 1,
                    0, 0, 0, 1,
                    0, 0, 0, 1,
                   };
net.uchenie(vect12,1);

float vect13[]= 
                   {
                    0, 1, 0, 0,
                    0, 1, 0, 0,
                    0, 1, 0, 0,
                    0, 1, 0, 0,
                   };
net.uchenie(vect13,1);
}
//**************************************************************
//Konec cikla obucenia
//**************************************************************
//Konec cikla obucenia
float vectin[]= 
                   {
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    1, 1, 1, 1,
                    0, 0, 0, 0,
                   };
net.in(vectin);
  ofstream myfile;
  myfile.open ("Out.txt");
  myfile <<  net.OUT()<<";"<<Z/Zi<<";\n";        
  myfile.close();   

//Proverka

cout <<"NET= "<<neuron[0][0].NET()<<"\n\n";
cout <<"OUT= "<<net.OUT()<<"\n\n";

   system("PAUSE");
   return EXIT_SUCCESS;
}

