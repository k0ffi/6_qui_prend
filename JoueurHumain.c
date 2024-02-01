#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include "foncGesJeu.h"
#include "foncH.h"
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

        printf("===>  connexion reussi ") ; 
        jouerH(sockfd) ; 
    }

    


  close(sockfd) ; 

    return 0 ; 

} 





void jouerH(int socket)
{
     bool rejouer  =false  ; 
     char reponse[10];
     bool condition  = true ; 



do
{
       
    int result =0 ;     
   int  i  = 0 , choix  =0 ; 
   int err  =  recv(socket , &i , sizeof(int) , 0 ) ; 

 if (err < 0)
    {
        fprintf(stderr," ID  non recu ") ; 
    }

    paquet p1  ; 
    initPaquet(&p1,10)   ; 
    paquet p = receivePaquet(socket ,  p1 ) ; 
   
    listeJoueurs.tab[i].p = p  ; 





   for(int u=0 ; u<10 ; u++ )
   {

       
           
            printf("\n+++++++++++++++++++++\n") ;
            printf("        JOUEUR [%d]      " , i) ; 
            printf("\n+++++++++++++++++++++\n") ;
            


            
        
            affichePaquet(&listeJoueurs.tab[i].p) ;




            //  Attendre le signale de jouer ou pas 
            int signale1 = 0  ; 
            int err1  = recv(socket ,&signale1  , sizeof(int) , 0 ) ; 
            if (err1>0)
            {
                // choisir la carte a envoyé
                do
                {
                
                printf("\n Vous jouer quel carte ? \n indice <?>  [ 0 , %d ]  "  , listeJoueurs.tab[i].p.nbr-1) ; 
                result  = scanf("%d" ,&choix) ; 
                 // Nettoie le tampon d'entrée en cas d'entrée incorrecte
                                                    while (getchar() != '\n');
                } while (result!=1  ||choix >listeJoueurs.tab[i].p.nbr ||  choix<0  );
                carte c  =  PrendreCarte(&listeJoueurs.tab[i].p , choix) ; 
                
                sendCarte(socket ,c  ) ;  
                
            }


            //  attendre si  le robots a finis 

            int  letsGo= 0  ; 
            int errLG  = recv(socket ,&letsGo  , sizeof(int) , 0 ) ; 
            if( errLG >0)
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
                                //
                                        int signale2 = 0  ; 
                                        int err2  = recv(socket ,&signale2  , sizeof(int) , 0 ) ;  
                                        if (err2>0)
                                        {
                                            int indiceLigne =9 ;
                                            do
                                            {
                                                    printf("\n\n >> Choisissez la ligne que vous voulez prendre [0-3]   : ") ; 
                                                    result = scanf("%d" , &indiceLigne) ;  
                                                    
                                                    // Nettoie le tampon d'entrée en cas d'entrée incorrecte
                                                    while (getchar() != '\n');

                                            } while (result != 1 || indiceLigne>3 ||  indiceLigne<0  );
                                            
                                            
                                            int errSend  = send(socket , &indiceLigne  , sizeof(int) , 0) ;
                                            if ( errSend>0)
                                            {
                                                printf("\n🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲🥲   \n")  ; 
                                                printf("\n>>       Vous avez   à pris la ligne %d      ", indiceLigne) ; 
                                            }
                                            else{
                                                fprintf(stderr, " Ligne non envoyée ") ; 
                                            }
                                        } 
                                }
                            
                               
                            
                            
                    }
                    printf("\033[2J\033[1;1H"); // efface le terminale 
                    affichePaquet(&listeJoueurs.tab[i].p) ;
            }



           
    

  }
   do {
        
        printf("\033[33m>>Voulez-vous rejouer ? yes/no: ");
        scanf("%s", reponse);

        if (strcmp(reponse, "yes") == 0) {
            rejouer = true;
             condition = true;
        } else if (strcmp(reponse, "no") == 0) {
            rejouer = false;
             condition = true;
        } else {
            printf("Réponse invalide. Utilisez 'yes' ou 'no'.\n");
            condition = false;  // Réinitialise rejouer en cas de réponse invalide
        }
    } while (!condition);

    // envois de la reponse 

    int rep =  send (socket ,&rejouer , sizeof(bool ) , 0) ; 
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