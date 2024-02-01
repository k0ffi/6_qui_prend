#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "foncGesJeu.h"
#include <arpa/inet.h>
#include <stdbool.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;











bool verifierPoints() {
    for (int i = 0; i < listeJoueurs.nbr; i++) {
        int points = listeJoueurs.tab[i].points;
        if (points >= 66) {
            return true; // Au moins un joueur a atteint 66 points
        }
    }

    for (int i = 0; i < listeRobots.nbr; i++) {
        int points = listeRobots.tab[i].points;
        if (points >= 66) {
            return true; // Au moins un robot a atteint 66 points
        }
    }

    return false; // Aucun joueur ni robot n'a atteint 66 points
}









void afficheCarteEnJeu()

{
    printf("\n\n\n\n") ; 

    for ( int k =0  ; k<4  ; k ++ )
        {

            printf("±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±\n ");
            printf("\033[0m");
           
            printf("\033[5mLIGNE %d" , k ) ; 
            printf("\033[0m");
            
        for( int i =0   ; i < cartesEnJeu[k].nbr  ; i ++ )
            {
                
                afficheCarte(cartesEnJeu[k].tab[i]) ; 
                
            }
          
            printf("\n±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±\n"); 
           
            printf("\033[0m");
           
        }



}



void actualisation( carte c   , int index  ,int idR   , int idJ   ,  int choix )

{   
    

    if ( choix ==1 )
    { //joueur
        if( cartesEnJeu[index].nbr ==5) 
            {
                paquet p  ; 
                initPaquet(&p,cartesEnJeu[index].nbr) ;  
                cartesEnJeu[index].tab[cartesEnJeu[index].nbr] =  c  ; 
                cartesEnJeu[index].nbr ++  ; 
                p  = PrendrePaquet(&cartesEnJeu[index] ,cartesEnJeu[index].nbr-1 ) ; 


                  int som =0 ; 
                for ( int i =0 ; i< p.nbr ;  i++)
                        som += p.tab[i].points ;  // ils prend tous les points de la ligne choisis
                    
                listeJoueurs.tab[idJ].points += som ; 
                            
            

             }
        else{
                cartesEnJeu[index].tab[cartesEnJeu[index].nbr] =  c  ; 
                cartesEnJeu[index].nbr ++  ; 

            }


    }else 
    {// robots
    if( cartesEnJeu[index].nbr ==5) 
            {
                paquet p  ; 
                initPaquet(&p,cartesEnJeu[index].nbr) ;  
                cartesEnJeu[index].tab[cartesEnJeu[index].nbr] =  c  ; 
                cartesEnJeu[index].nbr ++  ; 
                p  = PrendrePaquet(&cartesEnJeu[index] ,cartesEnJeu[index].nbr-1 ) ; 


                int som =0 ; 
                for ( int i =0 ; i< p.nbr ;  i++)
                        som += p.tab[i].points ;  // ils prend tous les points de la ligne choisis
                    
                listeRobots.tab[idR].points += som ; 
                            
            

             }
        else{
                cartesEnJeu[index].tab[cartesEnJeu[index].nbr] =  c  ; 
                cartesEnJeu[index].nbr ++  ; 

            }



    }

    
}




void initialisation()
{
        
    //initialisation du jeu 
basePaquet = ( paquet *)malloc(sizeof(paquet)) ;
paquet  p  ;
initPaquet(&p ,6)  ;   
initPaquet(basePaquet,104) ; 
produitJeu(basePaquet) ; 
melangePaquet(basePaquet) ;



distributionDesCartes(listeJoueurs , listeRobots  , basePaquet) ;
p =  (PrendrePaquet(basePaquet , 4)) ;  
for( int i =0 ; i<4  ;  i++ ){
    initPaquet(&cartesEnJeu[i],6) ; 
    cartesEnJeu[i].tab[0] = p.tab[i] ;  
    cartesEnJeu[i].nbr = 1 ; 
}

}


void initPaquet(paquet *p, int capacite) {
    // Vérifier si le pointeur p est différent de NULL
    if (p == NULL) {
        fprintf(stderr, "Pointeur de paquet invalide.\n");
        exit(EXIT_FAILURE);
    }

    // Initialiser la capacité
    p->capa = capacite;
    p->nbr = capacite ; 

    // Allouer dynamiquement de la mémoire pour p->tab
    p->tab = (carte *)malloc(capacite * sizeof(carte));

    // Vérifier si l'allocation de mémoire a réussi
    if (p->tab == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour p->tab.\n");
        exit(EXIT_FAILURE);
    }
}

void liberePaquet(paquet *p)
{
    free(p->tab);
    p->nbr=0 ; 

}

void afficheCarte(carte c)
{
    printf("\n___________________________\n");
    printf("\033[3m Carte n° [%d] \n",c.numero);
    printf("\033[0m");
  for ( int i = 0  ; i<  c.points ; i ++ )
            printf("🐮 ") ; 

    printf("\n---------------------------");
}

void affichePaquet(paquet *p)
{

    for (int i = 0; i < p->nbr; i++)
    {
        printf("\033[1;35m\t\t\t   <%d>",i);
        printf("\033[0m");
        afficheCarte(p->tab[i]);
        printf("\n\n");
    }
}

void videPaquet(paquet* p)
{
    p->nbr =0 ; 
}


carte creeCarte(int num)
{
    carte carte;
    carte.numero = num;


    if(((11*(num%10)) == num))
    {
        carte.points = 5;  
    } 
    else if(num % 5 == 0)
    {
        carte.points = 2;  // Exemple : points de 1 à 10 en fonction du dernier chiffre
    }
    else
    {
        carte.points = 1;
    }
    if(num % 10 == 0)
    {
        carte.points = 3;
    }
    else if(num == 55)
    {
        carte.points = 7;
    }

    return carte;


}

void produitJeu(paquet* p) 
{
for(int  i =1 ; i<= 104  ; i++)
         p->tab[i-1]  = creeCarte(i) ; 
p->nbr = p->capa ; 
}






void melangePaquet(paquet* p)

{  srand(time(NULL)) ;
    for(int i =1  ;  i <=1000 ; i++)
    {
      
        int a  =  rand()%p->nbr ; 
        int  b = rand()%p->nbr ; 
        carte ctemp   ; 
        ctemp  = p->tab[a]   ; 
        p->tab[a] = p->tab[b] ; 
        p->tab[b]  = ctemp ; 
    }

}




paquet PrendrePaquet(paquet *p, int nbr)
{
    paquet pr;

    // Vérifier si le paquet initial a suffisamment de cartes
    if (p->nbr < nbr) {
        fprintf(stderr, "Erreur : pas assez de cartes dans le paquet initial\n");
        initPaquet(&pr, 10);
        return pr;
    }

    initPaquet(&pr, nbr);

    // Copier les nbr premières cartes dans le nouveau paquet
    for (int i = 0; i < nbr; i++) {
        pr.tab[i] = p->tab[i];
    }

    // Déplacer le pointeur tab du paquet initial de nbr positions
    p->tab = p->tab + nbr;
    p->nbr = p->nbr - nbr;

    return pr;
}





carte PrendreCarte(paquet * p  , int i  ) 
{
    carte c ; 
  do{
        c  = p->tab[i] ; 

        // Remplacer la carte tirée par la dernière carte du paquet
        p->tab[i] = p->tab[p->nbr - 1];
       

        
  
    }while((p->nbr> i && i<0))  ; 
     p->nbr--;
    
 return c;
}

//=============================================================

// Fonction pour échanger deux cartes dans le tableau
void echangerCartes(carte *cartes, int i, int j) {
    carte temp = cartes[i];
    cartes[i] = cartes[j];
    cartes[j] = temp;
}

// Fonction de tri à bulles pour trier le tableau de cartes par ordre croissant
void triABulles(paquet * p, int taille) {
    int aEchange;
    do {
        aEchange = 0;
        for (int i = 0; i < taille - 1; ++i) {
            if (p->tab[i].numero > p->tab[i + 1].numero) {
                echangerCartes(p->tab, i, i + 1);
                aEchange = 1;
            }
        }
    } while (aEchange);
}

int trouverMinimum(int tab[] , int  taille) {
    // Vérifier si le tableau est vide
    if (taille == 0) {
        fprintf(stderr, "Erreur : tableau vide\n");
    }

    // Initialiser la variable pour stocker le minimum avec la première valeur du tableau
    int minimum = tab[0] ; 
    int  k  =0 ; 

    // Parcourir le reste du tableau pour trouver le minimum
    for (int i = 1 ; i < taille; ++i) {
        if (tab[i] < minimum) {
            minimum = tab[i];
            k=i ; 
        }

    }

    return k ;
}


int getIDwhithNum(paquet *p , carte c  ) 
{

    int k =0 ; 
    for ( int i  = 0 ;  i < p->nbr ; i++)
    {
        if ( p->tab[i].numero == c.numero)
                    k = i ; 
    }
return  k  ;  

}

//=============================================================




void serialiser_carte(const carte *c, char *buffer, size_t buffer_size)
 {
    if (buffer_size < sizeof(carte)) {
        fprintf(stderr, "Taille du tampon insuffisante pour la sérialisation\n");
        return;
    }

    // Copier les membres de la structure dans le tampon
    memcpy(buffer, c, sizeof(carte));
}



carte  deserialiser_carte(const char *buffer, size_t buffer_size, carte *c) 
{
    if (buffer_size < sizeof(carte)) {
        fprintf(stderr, "Taille du tampon insuffisante pour la désérialisation\n");
        exit(-1) ; 
    }

    // Copier les octets du tampon dans la structure
    memcpy(c, buffer, sizeof(carte));
    return *c  ; 
}

void sendPaquet(int socket, paquet p ) 
{
        for (int i=0  ; i< p.nbr ; i++)
        {
            sendCarte(socket,p.tab[i]) ; 

        }

}

paquet  receivePaquet(int socket ,  paquet p  )
{
    paquet  pr ; 
    initPaquet(&pr,p.nbr) ; 
     for (int i=0  ; i<p.nbr ; i++)
        {
           pr.tab[i] = receiveCarte(socket,p.tab[i]) ; 
             
        }

  return pr  ; 

}


carte receiveCarte(int socket  , carte c )
{
    
    char buffer[sizeof(carte)];
    bzero(buffer,sizeof(buffer)) ; 
    carte cr ; 
    memset(buffer, 0, sizeof(buffer));
    int err = recv(socket,buffer ,  sizeof(buffer), 0);
    cr  = deserialiser_carte(buffer, sizeof(buffer), &c);
    if (err < 0)
    {
        perror("Erreur de réception de la carte  : ");
       
    }
    else if (err < sizeof(buffer))
    {
        printf("Erreur : nombre d'octets reçus incorrect (%d)\n", err);
    }
  return cr  ; 
}

void sendCarte ( int socket , carte c   )
{
   
        
         char buffer[sizeof(carte)];
         bzero(buffer,sizeof(buffer)) ; 
        
        serialiser_carte(&c, buffer, sizeof(buffer));
        
        // Envois de la carte 
        int err = send(socket, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        if (err < 0)
        {
            perror("Erreur d'envoi de la carte : ");
        }
        else if (err < sizeof(buffer))
        {
            printf("Erreur : nombre d'octets envoyés incorrect (%d)\n", err);
        }
      

}


void distributionDesCartes(joueurs jos  , robots ros  , paquet *p)
{ sem_unlink("/partage");


    partage = sem_open("/partage", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);

    if (partage == SEM_FAILED) {
        perror("Erreur lors de l'initialisation du sémaphore");
        return ;
    }

    sem_wait(partage) ; 


    for (int i = 0; i < (ros.nbr + jos.nbr) ; i++)
    {
      tabPaquet[i] = PrendrePaquet(p ,10) ; 
     
    }
    sem_post(partage);
    



     
}








// traitement consernant le ROBOT ==========================


void *connexionRobot(void *args) {
    

    threadArgs *ns = (threadArgs *)args;
   


    if (ns->socket < 0) {
        fprintf(stderr, "\n Connexion [Robot] non acceptée !\n");
    } else {
        printf("\n=====>   [Robot °%d]  en Jeu  +++++\n", ns->i);
         // envois de l 'identiter du  robot 
         int err  =  send(ns->socket, &ns->i , sizeof(int), 0) ; 
        if (err < 0)
             {
            fprintf(stderr, "\n  ID non envoyé !\n");
            }

        listeRobots.tab[ns->i].p = *(tabPaquet+ (listeJoueurs.nbr+  ns->i )  )  ; 
        sendPaquet(ns->socket, *(tabPaquet+ (listeJoueurs.nbr+  ns->i )  ) ) ;  
    


    
}




   
   
    pthread_exit(EXIT_SUCCESS);

}




// traitement consernant l'Humain   ==========================

void *connexionHumain(void *args) {
   
    threadArgs *ns = (threadArgs *)args;
   
    if (ns->socket < 0) {
        fprintf(stderr, "\n Connexion [Joueur] non acceptée !\n");
    } else {
     
        printf("\n=====>   [Joueur °%d]  en JEU  +++++\n", ns->i);

        int err  =  send(ns->socket, &ns->i , sizeof(int), 0) ; 
        if (err < 0)
            {
            fprintf(stderr, "\n ID non envoyé !\n");
            }
        listeJoueurs.tab[ns->i].p = *(tabPaquet+ (ns->i )  )  ; 
        sendPaquet(ns->socket, *(tabPaquet+ (ns->i )  ) ) ;  
    



    }

   
   
    pthread_exit(EXIT_SUCCESS);
    
}


void *FaireJouerR(void *args) 

{
    pthread_mutex_lock(&mutex);
    threadArgs *ns = (threadArgs *)args;
    printf(" \n Le  Robot [%d]  a joué   "  ,ns->i ) ;
    int nbr  = (listeJoueurs.nbr + listeRobots.nbr) ; 
    int err  = send(ns->socket ,&nbr, sizeof(int) , 0) ;
    if (err < 0){
        fprintf(stderr ," erreur d 'envois du signal ") ; 
    }



    //  recevoir une carte  

    carte c  ={0,0} ; 
    cartesRecues[listeJoueurs.nbr + ns->i]   = receiveCarte(ns->socket ,c) ; 
    pthread_mutex_unlock(&mutex);
    pthread_exit(EXIT_SUCCESS);
}



void *FaireJouerH(void *args)  
{
    pthread_mutex_lock(&mutex);
    threadArgs *ns = (threadArgs *)args;
   

    printf("\n C'est autour du Joueur [%d]  de jouer   "  ,ns->i  ) ;
    int nbr  = (listeJoueurs.nbr + listeRobots.nbr) ; 
    int err  = send(ns->socket ,&nbr, sizeof(int) , 0) ;
    if (err < 0){
        fprintf(stderr ," erreur d 'envois du signal ") ; 
    }

    //  recevoir une carte  

    carte c = {0,0}  ; 
    cartesRecues[ns->i] = receiveCarte(ns->socket ,c) ; 
    pthread_mutex_unlock(&mutex);
    pthread_exit(EXIT_SUCCESS);




}





void *casserLigneRobot(void *args) 
{

    pthread_mutex_lock(&mutex);
     threadArgs *ns = (threadArgs *)args;


    int err  = send(ns->socket , &ns->i  , sizeof(int) , 0) ;
    if (err < 0){
        fprintf(stderr ," erreur d 'envois du signal ") ; 
    }

    //  reception dela ligne
    int ligne =9 ;  
    int err1  = recv(ns->socket ,&ligne  , sizeof(int) , 0 ) ;  
    if (err1<0)
    {   
        fprintf(stderr , " Ligne non recus  de la part du robots ") ; 

    }else{

        paquet p  ; 
        initPaquet(&p,cartesEnJeu[ligne].nbr-1) ;  
        actualisation(ns->c,ligne,ns->i,0,0) ;  //mettre la plus petite carte sur la ligne choisi
        p  = PrendrePaquet(&cartesEnJeu[ligne] ,cartesEnJeu[ligne].nbr-1 ) ;  
        int som =0 ; 
        for ( int i =0 ; i< p.nbr ;  i++)
        
            {
               som += p.tab[i].points ;  // ils prend tous les points de la ligne choisis 
            }
              listeRobots.tab[ns->i].points += som ; 
            


    }

    afficheCarteEnJeu()  ;
    pthread_mutex_unlock(&mutex);
    pthread_exit(EXIT_SUCCESS);
}




void *casserLigneJoueur(void *args) 
{  
    pthread_mutex_lock(&mutex);
    threadArgs *ns = (threadArgs *)args;


    int err  = send(ns->socket , &ns->i  , sizeof(int) , 0) ;
    if (err < 0){
        fprintf(stderr ," erreur d 'envois du signal ") ; 
    }

    //  reception dela ligne
    int ligne =9 ;  
    int err1  = recv(ns->socket ,&ligne  , sizeof(int) , 0 ) ;  
    if (err1<0)
    {   
        fprintf(stderr , " Ligne non recus  de la part du robots ") ; 

    }else{


        paquet p  ; 
        initPaquet(&p,cartesEnJeu[ligne].nbr-1) ;  
        actualisation(ns->c,ligne,0,ns->i,1) ; //mettre la plus petite carte sur la ligne choisi ; 
        p  = PrendrePaquet(&cartesEnJeu[ligne] ,cartesEnJeu[ligne].nbr-1 ) ;  
        int som =0 ; 
        for ( int i =0 ; i< p.nbr ;  i++)
        
            {
               som += p.tab[i].points ;  // ils prend tous les points de la ligne choisis 
            }
            listeJoueurs.tab[ns->i].points += som ; 
             

    }
    afficheCarteEnJeu()  ;
    pthread_mutex_unlock(&mutex);
    pthread_exit(EXIT_SUCCESS);
}

























// le deroulement du jeu






void *Jeu()
{
  fprintf(fichier,"________ Manche %d"  ,manche)  ; 
  bool rejoue = true ; 
  int nbrParticipant  = listeRobots.nbr +listeJoueurs.nbr ; 
 
   paquet pu, carteEnJeuNonTrier;
   paquet p , copieNonTrier  ;
   
          
 
   do
   {
              

            for (int u = 0; u < 10; u++)
            {                
                initPaquet(&pu, 5);
                initPaquet(&carteEnJeuNonTrier, 4);
                initPaquet(&p ,nbrParticipant) ; 
                initPaquet(&copieNonTrier,nbrParticipant) ;

                                    
                                int cpt = 0 ;  
                                // faire jouer chaque joueurs
                                for(int i  = 0  ; i < listeJoueurs.nbr  ; i++)
                                {
                                    threadArgs args ; 
                                    args.socket = listeJoueurs.tab[i].socket ; 
                                    args.i = listeJoueurs.tab[i].id ; 
                                    
                                    int erreur = pthread_create(&threadsH[i], NULL, (void *)FaireJouerH, &args) ; 
                                    if (erreur != 0) {
                                        fprintf(stderr," Erreur de la creation du thread  faire jouer   J°%d  ", (i));
                                        exit(EXIT_FAILURE);
                                    }
                                    // chaque threads attends le suivant 
                                    int erreur1 = pthread_join(threadsH[i],NULL);
                                    if (erreur1 != 0) {
                                        fprintf(stderr, "Erreur lors de l'attente du thread H  (faire jouer ) %d\n", i);
                                        exit(EXIT_FAILURE);
                                    }

                                }
                                // faire jouer chaque Robots
                                for(int i  = 0  ; i < listeRobots.nbr  ; i++)
                                {
                                    threadArgs args ; 
                                    args.socket = listeRobots.tab[i].socket ; 
                                    args.i = listeRobots.tab[i].id ; 
                                    
                                    int erreur = pthread_create(&threadsR[i], NULL, (void *)FaireJouerR, &args) ; 
                                    if (erreur != 0) {
                                        fprintf(stderr," Erreur de la creation du thread  faire jouer   R°%d  ", (i));
                                        exit(EXIT_FAILURE);
                                    }
                                    // chaque threads attends le suivant 
                                    int erreur1 = pthread_join(threadsR[i],NULL);
                                    if (erreur1 != 0) {
                                        fprintf(stderr, "Erreur lors de l'attente du thread R %d\n", i);
                                        exit(EXIT_FAILURE);
                                    }

                                }

                                //Envois du signale du passage a l'etape 2  
                                int  sigEtape2  = 1 ; 

                                for( int  i  = 0 ;  i < listeRobots.nbr ; i++)
                                {
                                    
                                        int errR =  send (listeRobots.tab[i].socket ,&sigEtape2 , sizeof(int ) , 0) ; 
                                        if(errR<0)
                                        {
                                            perror("Erreur etape R 2 ") ; 
                                        }
                                    
                                }

                                for( int  i  = 0 ;  i < listeJoueurs.nbr ; i++)
                                {
                                        int errJ  =  send (listeJoueurs.tab[i].socket ,&sigEtape2 , sizeof(int) , 0); 
                                        if(errJ<0)
                                        {
                                            perror("Erreur etape J  2 ") ; 
                                        } 

                                }


            
                


                            
                                printf("\n\n\033[1;33mCARTE RECUE\n\n");
                                printf("\033[0m");
                                // copie et affichage des cartes recus  
                                for (int i =0 ; i<nbrParticipant ; i++)
                                    {    p.tab[i] = cartesRecues[i] ; 
                                        afficheCarte(cartesRecues[i])  ; 
                                    }
                                printf("\n\n\033[1;33mCARTE RECUE\n\n");
                                printf("\033[0m");
                                triABulles(&p , nbrParticipant ) ; 


                                initPaquet(&pu , 5) ;  
                                

                                    for(int j =0   ; j < nbrParticipant  ;  j++)
                                    {
                                        copieNonTrier.tab[j] = cartesRecues[j];
                                    }
                        
                                for( int i =0 ; i< nbrParticipant ; i++)
                                {  
                                    for( int j =0   ; j < 4  ;  j++ )
                                        {
                                            pu.tab[j] = cartesEnJeu[j].tab[cartesEnJeu[j].nbr - 1];
                                            carteEnJeuNonTrier.tab[j] = cartesEnJeu[j].tab[cartesEnJeu[j].nbr - 1];
                                        }

                                // tris les cartes en jeu avec la ieme carte recue
                                    pu.tab[4] = p.tab[i];
                                    triABulles(&pu, 5);


            
                                    if (cpt==0)
                                    {
                                         // on range  les cartes 
                                        if (pu.tab[0].numero != p.tab[i].numero)
                                            {
                                            
                                                
                                                    int id   =  getIDwhithNum(&pu,p.tab[i]) ; 
                                                    int ligne  = getIDwhithNum(&carteEnJeuNonTrier ,pu.tab[id-1]  ) ; 
                                                    
                                                    int idp = getIDwhithNum(&copieNonTrier, p.tab[i]);

                                                    // on cherche  lme type de joueur qui a envoyé la carte  
                                                    if (idp > (listeJoueurs.nbr - 1))
                                                    { //ROBOTS
                                                        actualisation(p.tab[i], ligne, (idp - listeJoueurs.nbr), 0, 0);
                                                        // pas de ligne a prendre de la part du robots ni du joueur 
                                                        int sigR = 0;
                                                        int sigerrR = send(listeRobots.tab[idp- listeJoueurs.nbr].socket, &sigR, sizeof(int), 0);
                                                        if (sigerrR < 0)
                                                        {
                                                            perror("ERREUR  sigR if");
                                                        }
                                                       
                                                        


                                                    }
                                                    else
                                                    {
                                                        //JOUEUR
                                                        actualisation(p.tab[i], ligne, 0, (idp), 1);
                                        
                                                        int sigJ = 0;
                                                        int sigerrJ = send(listeJoueurs.tab[idp].socket, &sigJ, sizeof(int), 0);
                                                        if (sigerrJ < 0)
                                                        {
                                                            perror("ERREUR sigJ if");
                                                        } 
                                                    
                                                    }



                                                    
                                                

                                                    
                                            }
                                        // on prend une ligne 
                                        else if (pu.tab[0].numero == p.tab[i].numero)
                                        {   
                                            // on vas aller chercher le joueur qui a la plus petite carte 
                                            int idG =   getIDwhithNum(&copieNonTrier ,  p.tab[0] )  ; 

                                            if ( idG>(listeJoueurs.nbr-1 ) )
                                            {//ROBOTS  
                                                cpt++  ;  
                                                robot  ro  ; 
                                                ro  = listeRobots.tab[idG - listeJoueurs.nbr]; 
                                                threadArgs args ; 
                                                args.i = ro.id ; 
                                                args.p= ro.p  ; 
                                                args.socket = ro.socket ;   
                                                args.c = p.tab[0] ; 
                                            
                                                // on lance le robots que a la plus petite cartes joué de choisir un ligne
                                              
                                                int sigR = 1;
                                                int sigerrR = send(ro.socket, &sigR, sizeof(int), 0);
                                                if (sigerrR < 0)
                                                {
                                                    perror("ERREUR  sigR Pr_ligne R ");
                                                }
                                                int erreur = pthread_create(&threadsR[idG - listeJoueurs.nbr], NULL, (void *)casserLigneRobot, &args);
                                                if (erreur != 0)
                                                {
                                                    fprintf(stderr, " Erreur de la creation du thread  pour prendre un ligne    R°%d  ", (idG - listeJoueurs.nbr));
                                                    exit(EXIT_FAILURE);
                                                }
                                                //   le programme attend la fin du thread 
                                                int erreur1 = pthread_join(threadsR[idG - listeJoueurs.nbr],NULL);
                                                if (erreur1 != 0) {
                                                    perror("Erreur lors de l'attente du thread H  (casser ligne robot )\n");
                                                    exit(EXIT_FAILURE);
                                                }

                                        

                                            }
                                            else{
                                            //JOUEUR
                                                    cpt++  ; 
                                                    joueur jo  ; 
                                                    jo  = listeJoueurs.tab[idG]; 
                                                    threadArgs args ; 
                                                    args.i = jo.id ; 
                                                    args.p= jo.p  ; 
                                                    args.socket = jo.socket ;   
                                                    args.c = p.tab[0] ; 
                                                    
                                                    // on lance le robots que a la plus petite cartes joué de choisir un ligne 
                                                
                                                
                                                    int sigJ = 1;
                                                    int sigerrJ = send(jo.socket, &sigJ, sizeof(int), 0);
                                                    if (sigerrJ < 0)
                                                    {
                                                        perror("ERREUR sigR Pr_ligne J ");
                                                    }
                                                   
                                                    int erreur = pthread_create(&threadsH[idG], NULL, (void *)casserLigneJoueur, &args);
                                                    if (erreur != 0)
                                                    {
                                                        fprintf(stderr," Erreur de la creation du thread  pour prendre un ligne    J°%d  ", (idG));
                                                        exit(EXIT_FAILURE);
                                                    }
                                                    //   le programme attend la fin du thread 
                                                    int erreur1 = pthread_join(threadsH[idG],NULL);
                                                    if (erreur1 != 0) {
                                                        perror( "Erreur lors de l'attente du thread H  (casser ligne  joueur )\n");
                                                        exit(EXIT_FAILURE);
                                                    }

                                            }
                                        }
                                }
                                else{// c'est le cas ou on a deja pris  un carte 
                                                    
                                                    
                                                    
                                                    int id   =  getIDwhithNum(&pu,p.tab[i]) ; 
                                                    int ligne  = getIDwhithNum(&carteEnJeuNonTrier ,pu.tab[id-1]  ) ; 
                                                    
                                                    int idp = getIDwhithNum(&copieNonTrier, p.tab[i]);
                                                    if (idp > (listeJoueurs.nbr - 1))
                                                    { //ROBOTS
                                                    
                                                        actualisation(p.tab[i], ligne, (idp - listeJoueurs.nbr), 0, 0);
                                                        // pas de ligne a prendre de la part du robots ni du joueur 
                                                        int sigR = 0;
                                                        int sigerrR = send(listeRobots.tab[idp- listeJoueurs.nbr].socket, &sigR, sizeof(int), 0);
                                                        if (sigerrR < 0)
                                                        {
                                                            perror("ERREUR  sigR grand else");
                                                        }
                                                       


                                                    }
                                                    else
                                                    {
                                                        actualisation(p.tab[i], ligne, 0, (idp), 1);
                                                   
                                                        int sigJ = 0;
                                                        int sigerrJ = send(listeJoueurs.tab[idp].socket, &sigJ, sizeof(int), 0);
                                                        if (sigerrJ < 0)
                                                        {
                                                            perror("ERREUR sigJ grand else");
                                                        } 
                                                    
                                                    }
                                    }  

                                



                                
                                

                            


                                }


                                afficheCarteEnJeu() ; 
                                liberePaquet(&pu) ; 

                   
                    
            }

           
                



                bool RejoueJ [listeJoueurs.nbr ]  ; 
                bool RejoueR [listeRobots.nbr] ; 
              

             

                // reception  des reponces de chaque joueurs 
                for ( int i =0  ; i< listeJoueurs.nbr ; i++)
                    {
                            int e  =  recv (listeJoueurs.tab[i].socket ,&RejoueJ[i] , sizeof(bool) , 0); 
                                        if(e<0)
                                        {
                                            perror(" erreur  Rejoue  ") ; 
                                        }
                                       
                                rejoue =  rejoue && RejoueJ[i]  ;  


                    }

            


                       // reception  des reponces de chaque Robots 
                 for ( int i =0  ; i< listeRobots.nbr ; i++)
                    {
                            int e  =  recv (listeRobots.tab[i].socket ,&RejoueR[i] , sizeof(bool) , 0); 
                                        if(e<0)
                                        {
                                            perror(" erreur  Rejoue  ") ; 
                                        }
                                      
                                rejoue =  rejoue && RejoueR[i]  ;  


                    }


                //  envois la reponse a tous les participants 
                
                //Joueur

                for ( int i =0  ; i< listeJoueurs.nbr ; i++)
                    {
                        int roll =  send (listeJoueurs.tab[i].socket ,&rejoue, sizeof(bool ) , 0) ; 
                                        if(roll<0)
                                        {
                                            perror("Erreur Next Manche  ") ; 
                                        }
                                   
                    }

                // Robots 
                for ( int i =0  ; i< listeRobots.nbr ; i++)
                    {
                        int roll =  send (listeRobots.tab[i].socket ,&rejoue, sizeof(bool ) , 0) ; 
                                        if(roll<0)
                                        {
                                            perror("Erreur Next Manche  ") ; 
                                        }
                    }

                
                time_t t = time(NULL);
                struct tm *tm_info = localtime(&t);
                char buffer[26];
                strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);


                for( int i   = 0 ; i < listeJoueurs.nbr  ; i++)
                {
                    
                    fprintf(fichier, "\n    ___ [%s]\n Joueur[%d] A  => %d  \n", buffer,listeJoueurs.tab[i].id,listeJoueurs.tab[i].points );
               


                }
                for( int i   = 0 ; i < listeRobots.nbr ; i++)
                {
                
                    fprintf(fichier, "\n    ___ [%s]\n Robots[%d] A  => %d  \n", buffer,listeRobots.tab[i].id,listeRobots.tab[i].points ); 

                    
                }

                 bool perdu  = verifierPoints() ; 
            if(perdu)
                    {

                        pthread_exit(EXIT_SUCCESS); 
                    }




                
                if(rejoue) // on redistrubut les cartes on rejoue  ; 
                {   
                    liberePaquet(basePaquet) ; 
                    initialisation() ; 

                    manche++ ;  
                    fprintf(fichier,"________ Manche %d"  ,manche)  ; 
                     for (int i = 0; i <listeJoueurs.nbr ; i++) {
                            

                            
                                    int err  =  send(listeJoueurs.tab[i].socket, &i , sizeof(int), 0) ; 
                                    if (err < 0)
                                        {
                                        fprintf(stderr, "\n ID non envoyé !\n");
                                        }
                                    listeJoueurs.tab[i].p = *(tabPaquet+ (i )  )  ; 
                                    sendPaquet(listeJoueurs.tab[i].socket, *(tabPaquet+ (i )  ) ) ;  
                                



                                


                     }

                    for (int i = 0; i < listeRobots.nbr; i++) {
                
                           
                                          


                                      
                                            int err  =  send(listeRobots.tab[i].socket, &i , sizeof(int), 0) ; 
                                            if (err < 0)
                                                {
                                                fprintf(stderr, "\n  ID non envoyé !\n");
                                                }

                                            listeRobots.tab[i].p = *(tabPaquet+ (listeJoueurs.nbr+  i )  )  ; 
                                            sendPaquet(listeRobots.tab[i].socket, *(tabPaquet+ (listeJoueurs.nbr+  i )  ) ) ;  
                                        


                    




                                                }
                }
                
    
    
    }while(rejoue  ) ;

  
liberePaquet(&carteEnJeuNonTrier);
liberePaquet(&p) ; 
liberePaquet(&copieNonTrier) ; 


free(tabPaquet) ; 
sem_unlink("/partage");
printf(" END THREAD ") ; 
fflush(stdout); 
pthread_exit(EXIT_SUCCESS); 


}
