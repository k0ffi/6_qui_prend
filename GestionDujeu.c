
/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
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

void error(char *msg)
{
    perror(msg);
    exit(1);
}




// =====================DEBUT===================
int main(int argc, char *argv[])
{

    
    manche = 1  ; 
   fichier = fopen("logfileStat.txt", "a");
       if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }
    fprintf(fichier, "\n ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" )  ; 
    

    tabPaquet = (paquet * )malloc(sizeof(paquet)*(100) ) ; 

    // declaration des threads  qui vont gerer  les connexions  individuellement


    int sockfdR, sockfdH, portnoR, portnoH, tailleAddrCli;
    int   nbrJ , nbrR  ;

    struct sockaddr_in serv_addrR, serv_addrH, cli_addr;

    printf("Veuillez entrer le nombre de joueurs pour cette partie   :  ") ; 
    scanf("%d", &nbrJ) ; 
    printf("Veuillez entrer le nombre de robots pour cette partie   : ") ; 
    scanf("%d", &nbrR) ; 
    listeRobots.nbr=nbrR ;
    listeJoueurs.nbr=nbrJ ;
    //  creation de tous les cartes 
    initialisation() ; 
    afficheCarteEnJeu()  ;  


  

    // creation du socket serveur Robot
    // socket TCP
    sockfdR = socket(AF_INET, SOCK_STREAM, 0);

    // creation du socket serveur Humain
    // socket TCP
    sockfdH = socket(AF_INET, SOCK_STREAM, 0);

    // verification de la creation du  socket

    if (sockfdR < 0)
        error("Ouverture de la connnexion echoué ( robot  ) ");

    // verification de la creation du  socket

    if (sockfdR < 0)
        error("Ouverture de la connnexion echoué ( joueur ) ");

    // initialisation de la zone memoire de l'addresse du serveur ( Gestion jeu  ) a zeros
    bzero((char *)&serv_addrR, sizeof(serv_addrR));
    bzero((char *)&serv_addrH, sizeof(serv_addrH));
    // on converti l'argument 1 en entier ( port )
    portnoR = 8080;
    serv_addrR.sin_family = AF_INET;
    serv_addrR.sin_addr.s_addr = INADDR_ANY;
    serv_addrR.sin_port = htons(portnoR);

    //  =========connexion Humain===============
    portnoH = 8084;
    serv_addrH.sin_family = AF_INET;
    serv_addrH.sin_addr.s_addr = INADDR_ANY;
    serv_addrH.sin_port = htons(portnoH);
    
    printf("\033[5m+++++++++++++++++++ En attente de connexion ++++++++++++++++++++\n");
    printf("\033[0m");
    // etablissement de la connexion  avec le Robot
    if (bind(sockfdR, (struct sockaddr *)&serv_addrR, sizeof(serv_addrR)) < 0)
    {
        error("\nERREUR  de connexion  avec le Robot");
    }
    // eta
    if (bind(sockfdH, (struct sockaddr *)&serv_addrH, sizeof(serv_addrH)) < 0)
    {
        error("\nERREUR  de connexion avec le Joueur");
    }

    // serveur a l'ecoute de 10 connexion Robots
    listen(sockfdR, 10);

    // serveur a l'ecoute de  max 10 connexions Humains
    listen(sockfdH, 10);

    tailleAddrCli = sizeof(cli_addr);

   

        
   
        



        


            // Les connexions  clients 
            
            for (int i = 0; i < nbrJ; i++) {
            
                newsocketH.i=i ;
                newsocketH.socket= accept(sockfdH, (struct sockaddr *)&cli_addr, (socklen_t *)&tailleAddrCli);
                joueur jo ; 
                jo.id =i ; 
                jo.points = 0 ; 
                jo.score = 0 ; 
                jo.socket = newsocketH.socket ; 
                 printf(" \nsocket %d  J  ==> %d" , i  , newsocketH.socket) ; 
                listeJoueurs.tab[i] = jo ; 

                // creation  de chaque Threads 
                int erreur = pthread_create(&threadsH[i], NULL, (void *)connexionHumain, &newsocketH);
                
                if (erreur != 0) {
                    printf(" Erreur de la creation du thread de connexion avec J°%d  ",i);
                    exit(EXIT_FAILURE);
                }
                // chaque threads attends le suivant 
                int erreur1 = pthread_join(threadsH[i],NULL);
                if (erreur1 != 0) {
                    fprintf(stderr, "Erreur lors de l'attente du thread J %d\n", i);
                    exit(EXIT_FAILURE);
                }
            }

            // les connexions Robots 
            
            for (int i = 0; i < nbrR; i++) {
                
                newsocketR.i =i ;  
                newsocketR.socket= accept(sockfdR, (struct sockaddr *)&cli_addr, (socklen_t *)&tailleAddrCli);
                printf(" \nsocket %d  R  ==> %d" , i  , newsocketR.socket) ; 
                robot ro ; 
                ro.id =i ; 
                ro.points =0 ; 
                ro.score = 0 ; 
                ro.socket = newsocketR.socket ; 
                
                listeRobots.tab[i] = ro ; 
                // creation  de chaque Threads 
                int erreur = pthread_create(&threadsR[i], NULL, (void *)connexionRobot, &newsocketR) ; 
                if (erreur != 0) {
                    fprintf(stderr," Erreur de la creation du thread de connexion avec R°%d  ", (i));
                    exit(EXIT_FAILURE);
                }
                // chaque threads attends le suivant 
                int erreur1 = pthread_join(threadsR[i],NULL);
                if (erreur1 != 0) {
                    fprintf(stderr, "Erreur lors de l'attente du thread R %d\n", i+1);
                    exit(EXIT_FAILURE);
                }
            }
            

            // echanges de données entre la gestions et les joueurs.
            pthread_t  gestionJeu ; 
            int erreur = pthread_create(&gestionJeu, NULL, (void *)Jeu , NULL) ; 
            if (erreur != 0) {
                    fprintf(stderr," Erreur de la creation du thread de  la  Gestion du Jeu  " );
                    exit(EXIT_FAILURE);
                }
            int erreur2 = pthread_join(gestionJeu,NULL);
                if (erreur2 != 0) {
                    fprintf(stderr, "Erreur lors de l'attente du thread ");
                    exit(EXIT_FAILURE);
                }
                
            afficheCarteEnJeu() ; 
            



           






   

    

        //fermeture de  tous les sockets 
        for( int i   = 0 ; i < nbrJ ; i++)
        {
            
            close(listeJoueurs.tab[i].socket) ; 


        }
        for( int i   = 0 ; i < nbrR ; i++)
        {
        
            
            close(listeRobots.tab[i].socket) ;

            
        }




        // verifie qui a perdu 
    

            for (int i = 0; i < listeJoueurs.nbr; i++) {
                int points = listeJoueurs.tab[i].points;
                if (points >= 66) {
                                fprintf(fichier," \n\n Joueur [%d] à perdu " ,listeJoueurs.tab[i].id)  ; 
                                printf("  \n\nJoueur [%d] à perdu " ,listeJoueurs.tab[i].id)  ; 
                                 }
                         }



            for (int i = 0; i < listeRobots.nbr; i++) {
                        int points = listeRobots.tab[i].points; 
                        if (points >= 66) {
                            fprintf(fichier," \n\n Robot [%d] à perdu " ,listeRobots.tab[i].id)  ; 
                            printf("  \n\n Robot [%d] à perdu " ,listeRobots.tab[i].id)  ; 
                        
                                            }
            }


        char scriptPath[] = "GestionStat.sh";

        // execution du fichier bash 
        int result = system(scriptPath);

        
        if (result == -1) {
            // La fonction system() a échoué
            perror("Erreur lors de l'exécution du script Bash");
        } else {
            // La fonction system() a réussi
            printf("Script Bash exécuté avec succès\n");
        }


        // ============+END+=================//
        fclose(fichier) ; 

        close(sockfdR);
        return 0;
}

