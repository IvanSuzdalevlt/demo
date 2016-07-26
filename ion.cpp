#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <graphics.h>
#include "winbgim.h"
#include "ionfn.h"
#include "datafn.h"
using namespace std;

int main(){
int iki=15;
double K=0.2;
int aprx=2;
int max=400;
int plotx=40;
int ploty=40;
int B=45;
//modelio parametru ivedimas
cout<<"Jonosferinio modeliavimo programa. Autorius Ivan Suzdalev AEm-6\n\n";

cout<<"Iveskite paskutine modeliavimo minute (maks 15min)";
cin>>iki;
cout<<"Iveskite maskimalia TEC koncentracija nuo 10 iki 10000 (PVZ 400)";
cin>>max;
cout<<"Iveskite aproksimacijos algoritmo veikimo lygi nuo 0 iki 6 (pvz 2)";
cin>>aprx;
cout<<"Iveskite modeliavimo algoritmo greicio koeficienta nuo 0 iki 10... (pvz 0.9)";
cin>>K;
cout<<"Ilgumos koordinates yra ribose tarp -150 ir -50 laipsn vakaru ilgumos\n";
cout<<"Platumos koordinates yra ribose tarp 60 ir 10 laipsn siaures platumos\n";
cout<<"Iveskite grafiko atvaizdavimo koordinates taska(ilguma) X  \n";
cin>>plotx;
cout<<"Iveskite grafiko atvaizdavimo koordinates taska(platuma) Y \n";
cin>>ploty;
cout<<"Iveskite tariama palydovinio signalo kritimo kampa [laipsn]\n venkite 90laipsn reiksmes  ";
cin>>B;
cout<<"\n-------------------------------------\n";
cout<<"Modeliavimas vyks minutes laiko tarpais iki: "<<iki<<"-os minutes\n";
cout<<"Maskimalii TEC koncentracija  "<<max<<"*10^16 electronu/m^2"<<"\n";
cout<<"Aproksimacijos algoritmo veikimo lygis yra: "<<aprx<<"\n";
cout<<"Modeliavimo algoritmo greicio koeficientas: "<<K<<"\n";
cout<<"Tariamas palydovinio signalo kritimo kampas [laipsn]  "<<B<<"\n\n";
cout<<"Grafiko atvaizdavimo koordinates\n taskas(ilguma/platuma) X ir Y  "<<plotx<<"   "<<ploty<<" \n";
cout<<"Pazymekite grafini langa ir paspauskite  bet kuri migtuka 1 karta\n";
cout<<"Noredami nutraukti modeliavima, grafiniame lange  paspauskite bet kuri migtuka\n";

//Platumos ir ilgumos konvertavimas programai prieinamu formatu
plotx=150+plotx;
ploty=60-ploty;
if (plotx<=1 || plotx>100) plotx=1;
if (ploty<=1 || ploty>50) ploty=1;

//failo su TEC duomenimis atidarymas ir nuskaitimas
FILE *file;
if ((file = fopen("TEC.txt", "r")) ==NULL){
  printf("failas nerastas\n");
  exit(1); 
}

if (iki>0 && iki <16) initwindow(1024,720); //atidaromas grafinis langas
if (iki<=0 || iki >=16) cin>>iki;
getch();


fseek(file,0,SEEK_END);
int bites = ftell(file);
char list[bites];
fseek(file,0,SEEK_SET);
fread( list,sizeof(char), sizeof(list), file );


    int zoom=6;
    int min=-1;
    int time=0;
    int Q11=0,Q12=0,Q21=0,Q22=0;
    double greitis[15];
    double pagreitis;
    
//tuscio duomenu bloko sukurimas
int block[blockcount(list,sizeof(list),time)]; 
int duom[16][102][52];
int origduom[16][102][52];
int faze=0;



//Modeliavimo ir rekonstrukcijos algoritmo pradzia
for (faze=0;faze<iki;faze++)
{     
    *block=blockcreate(list,block,bites,faze);
    //char bloko konvertavimas i trimati int bloka-masyva
    int flag=1;
    int kiek=0;
    int p=0;
    int x=0,y=0;
    int sizeblock=sizeof(block);
    for (int i=0;i<200;i++)
            {

}
        for (int i=0;i<sizeblock;i++)
            {
            if (block[i]==32 && flag==0) {kiek++; x++;}    
            if (block[i]==32) flag=1;
            if (block[i]!=32 && block[i]!=10) flag=0;
            if (block[i]==10 && i>0) {x=0;y++;}
            if (flag==0)
                {
                if (p<0) p=p*10-block[i];
                if (p>=0) p=p*10+block[i];
                if (block[i]==-1) p=(-1)*block[i];
    
                duom[faze][x][y]=p;
                origduom[faze][x][y]=p;
                }
                if (flag==1) p=0;
            }
            
//kadangi platuma prasideda nuo 10 ir baigiasi 60, apverciam duomenys, kad 10 butu apacioje
int buferis[102][53];

//verciam modeliuojamus duomenis veidrodiniu atvaizdu
for (int y=52;y>0;y--)
             {
             for (int x=0;x<102;x++)          
             {
             buferis[x][y]=duom[faze][x][51-y];
             }
}

for (int y=52;y>0;y--)
             {
             for (int x=0;x<102;x++)          
             {
             duom[faze][x][y]=buferis[x][y];
             }
}


//verciam originalius duomenis veidrodiniu atvaizdu
for (int y=52;y>0;y--)
             {
             for (int x=0;x<102;x++)          
             {
             buferis[x][y]=origduom[faze][x][51-y];
             }
}

for (int y=52;y>0;y--)
             {
             for (int x=0;x<102;x++)          
             {
             origduom[faze][x][y]=buferis[x][y];
             }
}




  
  
//Jonosferos modeliavimo algoritmas
//duomenu suliginimas, jei TEC=0, tai TEC apkaiciuojamas modeliavimo ar suapvalinimo metu;
        if (faze>1)
        {
         for (int y=1;y<52;y++)
             {
             for (int x=1;x<102;x++)
             {
             greitis[0]=0;
             greitis[1]=(duom[1][x][y]-duom[0][x][y]);    
            if (duom[faze][x][y]==0)  duom[faze][x][y]=duom[faze-1][x][y];   
                greitis[faze]=(duom[faze][x][y]-duom[faze-1][x][y])*K;
                pagreitis=greitis[faze]-greitis[faze-1]; 
              //  cout<<"k="<<K<<" pagreitis="<<pagreitis<<"\n";
                duom[faze][x][y]=(double)duom[faze-1][x][y]+(double)greitis[faze]+(double)pagreitis/2;
                if (duom[faze][x][y]<0) duom[faze][x][y]=duom[faze-1][x][y]; //klaidu tikrinimas         
        }}
        }


        //duomenu aproksimacijos float algoritmas                   
        if (faze==1)
        {
        for(int i=1;i<=aprx;i++)
        {        
               for (int y=1;y<52;y++)
              {
               for (int x=1;x<=102;x++)
               {  
               duom[faze][x][y]=(duom[faze][x][y]+duom[faze-1][x][y])/2;                  
                }}  
        }
        }
        
        if (faze>=2){
        for(int i=1;i<=aprx;i++)
        {
               for (int y=1;y<52;y++)
              {
               for (int x=1;x<102;x++)
               {   
               duom[faze][x][y]=(duom[faze][x][y]+duom[faze-1][x][y]+duom[faze-2][x][y])/3;                  
                }}  
        }
        } 
        //aproksimacijos algoritmo pabaiga         

    //duomenu isvedimas
//apatines rekonstrukcinio atvaizdo linijos braizimas
bardraw(0,zoom*107,max,faze);

//grafiko braizimo funkcija
plotdraw(zoom*9,zoom*47,zoom,duom[faze-1][plotx][ploty],duom[faze][plotx][ploty],origduom[faze-1][plotx][ploty],origduom[faze][plotx][ploty],faze,12,14,max);    
//pranesimu lango braizimas
message(zoom*9,zoom*105,zoom,faze,plotx,ploty,duom[faze-1][plotx][ploty],duom[faze][plotx][ploty],origduom[faze-1][plotx][ploty],origduom[faze][plotx][ploty],B);

//interpoliavimas pagal originalius duomenys
      drawborders(0,0,zoom,0,100);
     int x1=0,x2=zoom,y1=0+5*zoom,y2=zoom+5*zoom;
    for (int y=2;y<50;y++)
    {
        for (int x=2;x<100;x++)
            {
            Q11=origduom[faze][x][y];
            Q12=origduom[faze][x][y+1];
            Q21=origduom[faze][x+1][y];
            Q22=origduom[faze][x+1][y+1];
            x1=x1+zoom; 
            x2=x2+zoom;
            interpoldraw(Q11,Q12,Q21,Q22,x1,x2,y1,y2,min,max);//interpoliacijos grafinis isvedimas
            }
            x1=0;
            x2=zoom;
            y1=y1+zoom;
            y2=y2+zoom;
            if (kbhit()) break;

    } 

    
     
    //interpoliavimas pagal sumodeliutus ir apvalintus duomenys
    drawborders(0,zoom*53,zoom,150,50);
    x1=0,x2=zoom,y1=0+58*zoom,y2=zoom+58*zoom;
    for (int y=2;y<50;y++)
    {
        for (int x=2;x<100;x++)
            {
            Q11=duom[faze][x][y];
            Q12=duom[faze][x][y+1];
            Q21=duom[faze][x+1][y];
            Q22=duom[faze][x+1][y+1];
            x1=x1+zoom; 
            x2=x2+zoom;
            interpoldraw(Q11,Q12,Q21,Q22,x1,x2,y1,y2,min,max);//interpoliacijos grafinis isvedimas
            }
            x1=0;
            x2=zoom;
            y1=y1+zoom;
            y2=y2+zoom;
            if (kbhit()) break;
    } 
    


    

if (kbhit()) break;
}   
 
//grafinio isvedimo pabaiga
while(!kbhit());     //laukiam, kol bus paspaustas bet koks migtukas
closegraph();        //close graphics window
fclose(file);
return (0);}
