#ifndef FONCGESJEU_H
#define FONCGESJEU_H


#include <semaphore.h>  
#include <stdbool.h>

//================STRUCTURE DES CARTES ==================
#define NBMAX 104


typedef struct
{
    int points; 
    int numero;
} carte;

typedef struct
{
    int capa;
    int nbr; // nombre de  carte
    carte *tab;
} paquet; 



//=================STRUCTURE SOCKET========================//
typedef  struct {
    int i; 
    int socket; 
    carte c ; //carte joué (la plus petite )
    paquet   p  ; 

}threadArgs ; 

//=================STRUCTURE JOUEUR========================//

typedef struct {
int id  ; 
int socket  ;
paquet   p  ; 
int points ; 
int score  ;  
 
}joueur  ; 


typedef struct {
    int nbr; // nombre de  joueurs 
    joueur tab[10];
}joueurs ; 

//=================STRUCTURE ROBOT========================//

typedef struct {
int id  ; 
int socket  ;
paquet   p  ; 
int points ; 
int score  ;

}robot  ; 


typedef struct {
    int nbr; // nombre de  robots 
    robot tab[10];
}robots ; 

// ============+END+=================//

paquet * basePaquet  ; 
threadArgs newsocketR ;  
threadArgs newsocketH  ;
joueurs listeJoueurs ; 
robots listeRobots ;
paquet *tabPaquet  ;  
sem_t *partage;
carte cartesRecues[20] ; 
paquet  cartesEnJeu[4] ;
pthread_t threadsH[10];
pthread_t threadsR[10];
int manche  ; 
 FILE *fichier  ;


// Déclaration des fonctions GesDuJeu
void initPaquet(paquet *p, int capacite) ; 
void liberePaquet(paquet *p);
void videPaquet(paquet* p) ;
void afficheCarte(carte c);
void affichePaquet(paquet *p);
carte creeCarte(int num) ;
void melangePaquet(paquet* p) ; 
void produitJeu(paquet* p) ;
paquet PrendrePaquet(paquet * p  , int nbr ) ; 
carte PrendreCarte(paquet * p  , int i  ) ; 
void distributionDesCartes(joueurs jos  , robots ros  , paquet *p) ; 
void afficheCarteEnJeu() ;


void echangerCartes(carte *cartes, int i, int j) ; 
void triABulles(paquet * p , int taille)   ; 
int  trouverMinimum(int tab[] , int taille)  ;
int getIDwhithNum(paquet *p , carte c  )  ;  

void serialiser_paquet(const paquet *p, char *buffer, size_t buffer_size) ; 
paquet deserialiser_paquet(const char *buffer, size_t buffer_size, paquet *p) ;

void serialiser_carte(const carte *c, char *buffer, size_t buffer_size);
carte  deserialiser_carte(const char *buffer, size_t buffer_size, carte *c)  ;

void sendPaquet(int socket, paquet p  ) ;
paquet  receivePaquet(int socket ,  paquet p );
carte  receiveCarte(int socket  , carte c  ) ;
void sendCarte ( int socket , carte c ) ; 
 
void actu(carte c  , int index ) ; 
void actualisation( carte c   , int index  ,int idR   , int idJ   ,  int choix )  ;
void initialisation()   ; 
void detruireRessourcePartagee() ;  
bool verifierPoints() ; 



void *connexionHumain(void  *args) ;
void *connexionRobot(void *args) ; 
void *FaireJouerR(void *args) ; 
void *FaireJouerH(void *args) ; 
void *casserLigneRobot(void *args) ; 
void *casserLigneJoueur(void *args) ; 
void *statistique( void *args)  ; 
void error(char *msg) ; 




void *Jeu() ; 


#endif
