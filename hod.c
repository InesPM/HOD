#include <stdio.h>
#include <math.h>
#include <stdlib.h>

main(){

/************************************/
/*Création array à partir du fichier*/
/************************************/

double z=0.0;       //Redshift compris entre z et z+0.1

/*Comptage lignes fichier*/

FILE *fp1;
fp1=fopen("170130_slice_0","r");

int c;
int N = 0;          //Nb. lignes fichier
while ((c = getc(fp1)) != EOF){
    if (c == '\n'){++N;}
}

printf("N=%i\n",N);

fclose(fp1);

/*Paramètres fichier*/

int n;              //Position dans l'array
int i;              //Numéro ligne<n
int L=N;            //Nb. de halos non répétés

int a=0;

double* idh=NULL;
idh=malloc(N*sizeof(double));
int* idb=NULL;
idb=malloc(N*sizeof(double));
double* M=NULL;
M=malloc(N*sizeof(double));
int* g=NULL;
g=malloc(N*sizeof(double));


/*double idh[N];
int idb[N];
double M[N];
int g[N];*/             //Nb. de galaxies dans un halo donné

int gmax=1;             //Nb. max galaxies/halo
int gsat=2;
double Mmin=1e16;       //Masse min halo pour 1 galaxie
double Mmax=0;          //Masse max halo
double Msat=1e16;       //Masse min halo pour galaxie satellite

/*Ouverture fichier*/

FILE *fp2;
fp2=fopen("170130_slice_0","r");

for (n=0;n<N;n++){
    if (n%1000 == 0){printf("n=%i/%i\n",n,N);}

    fscanf(fp2,"%lf %i %lf",&idh[n],&idb[n],&M[n]);
    g[n]=1; //1 galaxie par ligne du fichier
    for (i=0;i<n;i++){
        if (idh[n]==idh[i] && idb[n]==idb[i] && g[i]!=0){     //Si le halo est répété
            g[i]=g[i]+1;        //Incrément du nombre de galaxies dans le halo i
            idh[n]=0;
            idb[n]=0;
            g[n]=0;             //Id halo=0, supprimée postérieurement
            L=L-1;
            if (g[i]>gmax){gmax=g[i];}
            if (M[i]<Msat && g[i]>1){Msat=M[i];}
        }
    }

    if (M[n]<Mmin){Mmin=M[n];}
    if (M[n]>Mmax){Mmax=M[n];}
}

printf("Nb max de galaxies/halo=%i \n",gmax);
printf("Masse max halo=%lf \n",Mmax);
printf("Masse min halo=%lf \n",Mmin);
printf("Masse min halo galaxie satellite=%lf \n",Msat);

fclose(fp2);

/****************************************************/
/*Moyenne nombre de galaxies par intervalle de masse*/
/****************************************************/

/*Fichier avec les données triées: identifiant halo, masse halo, nombre de galaxies/halo*/

FILE *fp3;
fp3=fopen("slice_0.dat","a");

for (i=0;i<N;i++){if (g[i]!=0){fprintf(fp3,"%lf %i %lf %i\n",idh[i],idb[i],M[i],g[i]);}}

fclose(fp3);

free(idh);
free(idb);
free(M);
free(g);


/*Création vecteurs données à partir du fichier*/

double* idht=NULL;
idht=malloc(L*sizeof(double));
int* idbt=NULL;
idbt=malloc(L*sizeof(double));
double* Mt=NULL;
Mt=malloc(L*sizeof(double));
int* gt=NULL;
gt=malloc(L*sizeof(double));

/*double idht[L];
int idbt[L];
double Mt[L];
int gt[L];*/

int l;

FILE *fp4;
fp4=fopen("slice_0.dat","r");

for (l=0;l<L;l++){fscanf(fp4,"%lf %i %lf %i",&idht[l],&idbt[l],&Mt[l],&gt[l]);}

fclose(fp4);

/**/

int nint=20;                    //Nb. d'intervalles de masse
double dm=(Mmax-Mmin)/nint;     //Intervalle de masse
int d;                          //Position intervalle
double gm[nint];                //Nb. moyen de galaxies par intervalle de masse
int h[nint];                    //Nb. de halos par intervalle de masse

/*Fichier contenant la moyenne du nombre de galaxies/halo pour un intervalle de masse*/

FILE *fp5;
fp5=fopen("slice_0_moy.dat","a");

for (d=0;d<nint;d++){               //Intervalle de masse
    printf("d=%i\n",d);
    h[d]=0;                         //Initialisation nb. de halos dans intervalle de masse
    gm[d]=0;                        //Initialisation nb. moyen galaxies
    for (l=0;l<L;l++){
        if (Mt[l]>=Mmin+d*dm && Mt[l]<=Mmin+(d+1)*dm){
            gm[d]=gm[d]+gt[l];      //Incrément nb. galaxies
            h[d]=h[d]+1;            //Incrément nb. halos
        }
    }
    if (h[d]!=0){gm[d]=gm[d]/h[d];}  //Moyenne nb. galaxies/halo

    fprintf(fp5,"%lf %lf %i\n",Mmin+d*dm,gm[d],h[d]);
}

free(idht);
free(idbt);
free(Mt);
free(gt);

printf("d=%i dm=%lf",d,dm);

fclose(fp5);

/******************************************************/
/*Fichier contenant les masses en fonction du redshift*/
/******************************************************/

/*FILE *fp6;
fp6=fopen("mass.dat","a");
fprintf(fp6,"%lf %lf %lf %lf\n",Mmin,Msat,Msat/Mmin,z);
fclose(fp6);*/

}

