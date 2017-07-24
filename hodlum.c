#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

main(){

double z=29;
z=z/10+0.05;

/*Comptage lignes fichier*/

FILE *fp1;
fp1=fopen("170130_slice500_29","r");

int c;
int N = 0;          //Nb. lignes fichier
while ((c = getc(fp1)) != EOF){
    if (c == '\n'){++N;}
}

printf("N=%i\n",N);

fclose(fp1);

/*****************************************************/
/*Ouverture fichier masse, écriture fichier log masse*/
/*****************************************************/

int i;
int n;
int l;
int t;
int L;

double magmin=-22;      //Magnitude minimale
double magmax=-21.5;    //Magnitude maximale

/*Données fichier*/

double* idh=NULL;
idh=malloc(N*sizeof(double));
int* idb=NULL;
idb=malloc(N*sizeof(double));
double* M=NULL;
M=malloc(N*sizeof(double));
//int* g=NULL;
//g=malloc(N*sizeof(double));
double* mag=NULL;
mag=malloc(N*sizeof(double));

double x;

FILE *fp2;
fp2=fopen("170130_slice500_29","r");

for (n=0;n<N;n++){
    if (n%10000 == 0){printf("1 - n=%i/%i\n",n,N);}

    fscanf(fp2,"%lf %i %lf %lf %lf\n",&idh[n],&idb[n],&M[n],&x,&mag[n]);
    M[n]=log10(M[n]);
}

/*Données triées*/

double* idht=NULL;
idht=malloc(N*sizeof(double));
int* idbt=NULL;
idbt=malloc(N*sizeof(double));
double* Mt=NULL;
Mt=malloc(N*sizeof(double));
int* gt=NULL;
gt=malloc(N*sizeof(double));
double* magt=NULL;
magt=malloc(N*sizeof(double));

L=0;
t=0;

for (n=0;n<N;n++){
    if (n%1000 == 0){printf("2 - n=%i/%i\n",n,N);}
    l=0;                                                    //Condition pour savoir si le halo est répété
    if (mag[n]<=magmax && mag[n]>=magmin){                  //Si le halo est dans l'intervalle de magnitude désiré
    for (i=0;i<t;i++){                                      //Comparaison avec les éléments du tableau trié
        if (idh[n]==idht[i] && idb[n]==idbt[i]){            //Si le halo est répété
            gt[i]=gt[i]+1;                                  //Incrément du nombre de galaxies dans le halo i
            //if (g[i]>gmax){gmax=g[i];}
            l=l+1;
        }
    }
    if (l==0){          //Si le halo n'est pas répété
        gt[t]=1;        //Incrément du nombre de galaxies dans le halo t
        idht[t]=idh[n];
        idbt[t]=idb[n];
        Mt[t]=M[n];
        t=t+1;
    }
    }
}

fclose(fp2);

/****************************************************/
/*Moyenne nombre de galaxies par intervalle de masse*/
/****************************************************/

/*Fichier avec les données triées: identifiant halo, masse halo, nombre de galaxies/halo*/

FILE *fp3;
fp3=fopen("slice5003_29.dat","w");

for (i=0;i<t;i++){fprintf(fp3,"%lf %i %lf %i\n",idht[i],idbt[i],Mt[i],gt[i]);}

fclose(fp3);

free(idh);
free(idb);
free(M);
//free(g);
free(mag);

free(idht);
free(idbt);
free(Mt);
free(gt);
free(magt);

printf("z=%lf\n",z);
}
