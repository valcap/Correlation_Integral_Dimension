
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Programma per il calcolo del correlation integral secondo Grassberger and Procaccia
 Characterization of Strange Attractors, Physical Review Letters, (1983), 50, 5
 La correlation dimension e' ottenuta dal log-log plot del correlation integral
 rispetto al parametro r ed e' sempre minore o uguale alla dimensione frattale
 (A. Vulpiani, 'Determinismo e Caos', Carocci Ed.)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double edistance(double,double,double,double);
double heaviside(double);
double numcoup(double);
double r,corrint,temp;
double xx1,yy1,xx2,yy2;
double alfa;
double MAXR,MINR,INCR;
int npoint;
int i,j,k;
FILE *fcorrint;
FILE *FX1;FILE *FY1;

main( int argc, char *argv[] ) {
        if ( argc != 3 ) {
          perror("Usage: .exe fileX fileY\n\n");
          exit(1);
        }
        /*ASKS GENERAL SETTINGS ABOUT r*/
	printf("MAXR\n");
	scanf("%lf", &MAXR);
	printf("MINR\n");
	scanf("%lf", &MINR);
	printf("INCR\n");
	scanf("%lf", &INCR);

        /*GETS NUMBER OF POINTS AND PROPORTIONAL COEFFICIENT alfa*/
        FX1=fopen(argv[1],"r");
        if( FX1==NULL ) {
                perror("Errore in apertura del file di input FX1");
                exit(1);
        }
        printf("File con coordinata X %s\n",argv[1]);
        npoint = 0;
        while (fscanf(FX1, "%lf", &xx1) != EOF) {
                npoint++;
        }
        rewind(FX1);
	alfa = numcoup(npoint);

	/*OPENS (the other) INPUT FILE*/
	FY1=fopen(argv[2],"r");
        if( FY1==NULL ) {
                perror("Errore in apertura del file di input FY1");
                exit(1);
        }
        printf("File con coordinata Y %s\n",argv[2]);

	/*CALCULATES r-NEIGHBOURS*/
	r = MINR;
	while (r<=MAXR) {
		corrint = 0.;
		for ( i = 1; i <= npoint; i++ ) {
			k = 1;
			do {
				fscanf(FX1, "%lf", &xx1);
				fscanf(FY1, "%lf", &yy1);
				k++;
			} while (k<=i);
			rewind(FX1);
			rewind(FY1);
			for ( j = i + 1; j <= npoint; j++ ) {
                        	k = 1;
	                        do {
        	                        fscanf(FX1, "%lf", &xx2);
                	                fscanf(FY1, "%lf", &yy2);
					k++;
                        	} while (k<=j);
                        	rewind(FX1);
                        	rewind(FY1);
				temp = heaviside((r-edistance(xx1,yy1,xx2,yy2)));
/*
                                printf("%lf\t%lf\t%lf\t%lf\n",xx1,yy1,xx2,yy2);
                                printf("%lf\n",r-edistance(xx1,yy1,xx2,yy2));
*/
				corrint = corrint + temp;
			}
		}
		printf("%lf\t%lf\n",r,corrint/alfa);
		r = r*INCR;
	}
	
	/*CLOSES FILES*/
	fclose(FX1);fclose(FY1);
}

/*SUBROUTINES*/
double edistance(double ics1, double ipsilon1, double ics2, double ipsilon2) {
	double edist;
	edist = sqrt((ics1-ics2)*(ics1-ics2) + (ipsilon1-ipsilon2)*(ipsilon1-ipsilon2));
	return (edist);
}
double heaviside(double arg) {
	double heav;
	if (arg<0) {
		heav = 0;
	} else {
		heav = 1;
	}
	return(heav);
}
double numcoup(double m) {
	double nc;
	nc = (m*(m-1.0))/2.0;
	return (nc);
}
