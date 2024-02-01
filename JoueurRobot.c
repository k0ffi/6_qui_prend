#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <pthread.h>
#include "foncGesJeu.h"
#include "foncR.h"
#include <stdbool.h>
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int  main( int argc , char *argv[])
{

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
   

     if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
     

     } 
    server = gethostbyname(argv[1]);
     // port 
     portno = atoi(argv[2]); 
     // creation d'un socket TCP
     sockfd = socket(AF_INET, SOCK_STREAM, 0); 
     if (sockfd < 0) 
        error("ERREUR d'ouverture du  socket");
      


    if (server == NULL) {
        fprintf(stderr,"ERREUR aucun hote trouver \n");
        exit(0);
    }
    // initialisation de la zone memoire pour stocke l'addresse du serveur 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    // famille d'addresse en IPV4 
    serv_addr.sin_family = AF_INET;
    // on copie de nom de l'hote dans la zone memoire de s_addr
    memcpy((char *)&serv_addr.sin_addr.s_addr,
       (char *)server->h_addr,
       server->h_length);
       serv_addr.sin_port = htons(portno);
    // on tente la connexion entre le client et le serveur 
    if (connect(sockfd,(const struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    { 
        error("ERREUR  de connexion ");
    }else
    {
        printf(" ===>  connexion reussi") ; 
         jouerR(sockfd) ; 
         
    }


  close(sockfd) ; 

    return 0 ; 
}

void jouerR(int socket)
{
    bool rejouer  =false  ; 

    do
    {
    
    
            int  i  = 0 ; 
          
            int err  =  recv(socket , &i , sizeof(int) , 0 ) ; 
            if (err < 0)
            {
                fprintf(stderr," ID non recu ") ; 
            }
            

            paquet p1  ; 
            initPaquet(&p1,10)   ; 
            paquet p = receivePaquet(socket ,  p1 ) ; 

            listeRobots.tab[i].p = p  ; 


            
            for(int u=0 ; u< 10;  u ++ )
            {

                    
                    printf("\n+++++++++++++++++++++\n") ;
                    printf("        ROBOT [%d]      " , i) ; 
                    printf("\n+++++++++++++++++++++\n") ;
                    
                
                
                    affichePaquet(&listeRobots.tab[i].p) ;
                    
                    //  Attendre le signale de jouer ou pas 
                    int signale1 = 0  ; 
                    int err1  = recv(socket ,&signale1  , sizeof(int) , 0 ) ;  
                    if (err1>0)
                    {
                        // choisir la carte a envoyé
                        int indexCarte  = choisir(listeRobots.tab[i].p.nbr) ; 
                        carte c  =  PrendreCarte(&listeRobots.tab[i].p , indexCarte) ;  
                        sendCarte(socket ,c  ) ;  
                        
                    }
                    else{
                        printf(" signale non recu  ") ; 
                    }
                    
                    

                    int  letsGo= 0  ; 
                    int errLG  = recv(socket ,&letsGo  , sizeof(int) , 0 ) ; 
                    if(errLG>0)
                    {
                       
                            int sig = -1 ; 
                            int sigerr = recv(socket ,&sig  , sizeof(int) , 0 ) ;
                            if(sigerr<0)
                            {
                                    perror("ERREUR") ; 
                            }
                            else{
                                    if(sig==1)
                                    {
                                        int signale2 = 0  ; 
                                        int err2  = recv(socket ,&signale2  , sizeof(int) , 0 ) ;  
                                        if (err2>0)
                                        {
                                            int indiceLigne  = choisir(4) ; 
                                            int errSend  = send(socket , &indiceLigne  , sizeof(int) , 0) ;
                                            if ( errSend>0)
                                            {
                                                printf("\n🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲\n")  ; 
                                                printf("\n>>          R[%d]   à pris la ligne %d      ", i,indiceLigne) ; 
                                            }
                                            else{
                                                fprintf(stderr, " Ligne non envoyée ") ; 
                                            }
                                        } 
                                    }
                                
                                
                                
                                }
                        

                        printf("\033[2J\033[1;1H"); // efface le terminale 
                        affichePaquet(&listeRobots.tab[i].p)  ; 
                    }

            }


            

            bool oui =  true ; 
            int rep =  send (socket ,&oui , sizeof(bool ) , 0) ; 
                                        if(rep<0)
                                        {
                                            perror("Erreur reponse ") ; 
                                        }
                                        
                                     
            bool retour  = false ;
            int rtr  = recv(socket ,&retour , sizeof(bool ) , 0) ; 
            if(rtr<0)
                                                {
                                                    perror("Erreur retour  ") ; 
                                                } 
                                     
            rejouer =retour ; 
     
    } while (rejouer);
   
}





int choisir( int nbr ) 
{
    srand(time(NULL))  ; 
    return rand()%nbr ; 
    
}