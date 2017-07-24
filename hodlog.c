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
    for (i=0;i<t;i++){                                      //Comparaison avec les éléments du tableau trié
        if (idh[n]==idht[i] && idb[n]==idbt[i]){            //Si le halo est répété
            gt[i]=gt[i]+1;                                  //Incrément du nombre de galaxies dans le halo i
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

fclose(fp2);

/****************************************************/
/*Moyenne nombre de galaxies par intervalle de masse*/
/****************************************************/

/*Fichier avec les données triées: identifiant halo et box, masse halo, nombre de galaxies/halo*/

FILE *fp3;
fp3=fopen("slice500_29.dat","w");

for (i=0;i<t;i++){fprintf(fp3,"%lf %i %lf %i\n",idht[i],idbt[i],Mt[i],gt[i]);}

fclose(fp3);

free(idh);
free(idb);
free(M);
free(mag);

int gmax=1;             //Nb. max galaxies/halo
double Mmin=1e16;       //Masse min halo pour 1 galaxie
double Mmax=0;          //Masse max halo

for(i=0;i<t;i++){
    if (gt[i]>gmax){gmax=gt[i];}
    if (Mt[i]<Mmin){Mmin=Mt[i];}
    if (Mt[i]>Mmax){Mmax=Mt[i];}
}

printf("Nb max de galaxies/halo=%i \n",gmax);
printf("log Masse max halo=%lf \n",Mmax);
printf("log Masse min halo=%lf \n",Mmin);

/********************************************************************/
/*Calcul de la moyenne du nombre de galaxies par intervalle de log M*/
/********************************************************************/

printf("\nparamètres\n");

int nint=40;                    //Nb. d'intervalles de masse
double dm=(Mmax-Mmin)/nint;     //Intervalle de masse
int d;                          //Position intervalle
double gm[nint];                //Nb. moyen de galaxies par intervalle de masse
int h[nint];                    //Nb. de halos par intervalle de masse

/*Fichier contenant la moyenne du nombre de galaxies/halo pour un intervalle de masse*/

printf("moyenne\n");

FILE *fp5;
fp5=fopen("500logM40_N_z_h.dat","a");

for (d=0;d<nint;d++){               //Intervalle de masse
    printf("d=%i\n",d);
    h[d]=0;                         //Initialisation nb. de halos dans intervalle de masse
    gm[d]=0;                        //Initialisation nb. moyen galaxies
    for (i=0;i<N;i++){
        if (Mt[i]>=Mmin+d*dm && Mt[i]<=Mmin+(d+1)*dm){
            gm[d]=gm[d]+gt[i];       //Incrément nb. galaxies
            h[d]=h[d]+1;            //Incrément nb. halos
        }
    }
    if (h[d]!=0){gm[d]=gm[d]/h[d];}  //Moyenne nb. galaxies/halo

    fprintf(fp5,"%lf %lf %lf %i\n",Mmin+(d+0.5)*dm,gm[d],z,h[d]);
}

fprintf(fp5,"\n\n");

free(idht);
free(idbt);
free(Mt);
free(gt);
free(magt);


printf("\nNb max de galaxies/halo=%i \n",gmax);
printf("log Masse max halo=%lf \n",Mmax);
printf("log Masse min halo=%lf \n",Mmin);
printf("d=%i dm=%lf",d,dm);

fclose(fp5);

printf("z=%lf\n",z);
}
