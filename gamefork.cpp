//
//  main.cpp
//  EricHervetProjet
//
//  Created by Yann Ouedraogo on 2015-04-14.
//  Copyright (c) 2015 Yann Ghislain Ouedraogo. All rights reserved.
//  eyo3005@umoncton.ca

#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <array>
#include <iterator>
#include <ctime>
using namespace std;

string test[200];string text="";char jj;// variable globale pour verifier le input de l'utilisateur

typedef struct unecase//structure de la case de d'une grille
{
    int numerodescase;//numero de case qui eventuellement seront modifier
    string valeurcase;// valeur qui contient les X O H
    int posiVerou;//les position digital des case
    bool alreadyPlay;//pour marquer les cases ou on a deja jouer
    bool retirable;//pour verifier si le joueur a le droit de retirer une case
}cunease;
std::map<string, std::map<int, int> > mymap;//on utilise un mapping de maping pour faciliter
//le placement de de pions on pourra donc jouer avec une lettre et un chiffre comme sur les echecs
class gril//classe qui contienne les fonctions de la grille
{
public:
    gril();//constructeur
    int cordoJoeur1X; int cordoJoeur1Y;//cordonner avec les quels joueur 1 joue
    int cordoJoeur2X; int cordoJoeur2Y;//cordonner avec lesquels joueur 2 joue
    int GetcordoJoeur1X(){return cordoJoeur1X;}//fonctions pour obtenir les positions des joueur
    int GetcordoJoeur1Y(){return cordoJoeur1Y;}//et eviter les use of non static data menber
    int GetcordoJoeur2X(){return cordoJoeur2X;}
    int GetcordoJoeur2Y(){return cordoJoeur2Y;}
    array<array<unecase,6>,6> grille;//c++11 nous offre la library array donc on fait un array de a deux dimensions car on a besoin dutiliser (at) pour qu'on nous throw une erreur au cas ou on sort des dimensions de la grille ces pour cette raison on l'utilise
    int joueur1Jeton;//jeuton du joueur1
    int joueur2Jeton;//jeuton du joueur2
    void deplacement(int,int);//fonction pour nous montrer les deplacements en L possible
    void affichage();//fonction qui nous permet de faire l'affichage
    void radometoile();//fonction qui genere les H donc les etoiles pour donner des bonus au joueur
    bool caluldepoint();//fonction qui fait les totaux des points des joueurs
    void pressplay();//fonction qui mappe les position avec des chiffre et lettre
    void Joeur1Gameur();//fonction qui permet au joueur1 de jouer
    void Joeur2Gameur();//fonction qui permet au joueur2 de jouer
    bool secureInput(string);//fonction de robusteste pour les input des joueurs
    bool WehaveAWinner();//fonction booleen qui nous fait savoir si on a un gagnant
    bool casevide();
    void MessageauVainceur();
};
gril::gril()//constructeur on initialise les variables avec lesquelle on travaillera
{
    //les jetons sont fixer a 9 pour les deux joueurs
    joueur1Jeton=9;
    joueur2Jeton=9;
    cordoJoeur1X=0;cordoJoeur1Y=0;//intialisation des coordone
    cordoJoeur2X=0;cordoJoeur2Y=0;
    int cptt=0;//compteur pour assigner chiffre de position
    //on utilisera une grille fixe a 6x6
    for (int n=0; n<6; n++)
    {
        for (int m=0; m<6; m++)
        {
            grille[n][m].alreadyPlay=false;//jamais jouer? donc initialiser a false
            grille[n][m].valeurcase=" ";//la grille est vide a la base
            grille[n][m].retirable=false;//on ne veut pas permetre le retrait de case vide
            grille[n][m].numerodescase=cptt;//numero est utiliser pour les calcul et gestion d'erreur on assignera des variable arbitraire
            //pour reconaitre si on peut jouer ou combien de point assigner
            grille[n][m].posiVerou=cptt;//Au cas ou on veut acceder a une case avec sa position digitale les chiffre ne seront pas modifier d'ou le nom position verouiller
            cptt++;//compteur s'incremente de 0 a 35
        }
    }
}
/*
 On constate que le deplacement en L se fait suivant un certain model d'avancement en abscisse et ordonner donc on essaille toutes les possibilites a chaque appel et on montre a l'utilisateur quel sont les deplacement en L possible a partir de ce point
 */
void gril:: deplacement(int x,int y)//deplacement du cavalier
{
    bool dispo[6][6];// on initialise un tableau de booleen a false
    for (int n=0; n<6; n++)//tous ceux qui auront des true a leur position seront valider
    {   for (int m=0; m<6; m++)//comme deplacement possible apres on gerera ce qui est permis
        {
            dispo[n][m]=false;
        }
    }
    //on denombre 8 possibiliter de variation des abscisses et ordonnes pour atteindre
    //un deplacement en L on essaille d'aller a tous ces endroits pour placer des true quit
    //a essailler de sortir du tableau et attrapper l'erreur avec un catch invisible a l'utilisateur
    try
    {
        (grille.at(x+2).at(y+1));//test une position
        dispo[x+2][y+1]=true;//si on y arrive marque le avec un true
    }
    catch (const std::out_of_range& oor)//on ne veut pas afficher les erreurs de out of range
    {
        //std::cerr << " " << oor.what();
    }
    try
    {
        (grille.at(x+2).at(y-1));//test une position
        dispo[x+2][y-1]=true;//si on y arrive marque le avec un true
    }
    catch (const std::out_of_range& oor)//on ne veut pas afficher les erreurs de out of range
    {
       // std::cerr << " " << oor.what();
    }
    try
    {
        (grille.at(x-2).at(y+1));//test une position
        dispo[x-2][y+1]=true;//si on y arrive marque le avec un true
    }
    catch (const std::out_of_range& oor)//on ne veut pas afficher les erreurs de out of range
    {
       // std::cerr << " " << oor.what();
    }
    try
    {
        (grille.at(x-2).at(y-1));//test une position
        dispo[x-2][y-1]=true;//si on y arrive marque le avec un true
    }
    catch (const std::out_of_range& oor)//on ne veut pas afficher les erreurs de out of range
    {
       // std::cerr << " " << oor.what();
    }
    try
    {
        (grille.at(x+1).at(y+2));//test une position
        dispo[x+1][y+2]=true;//si on y arrive marque le avec un true
    }
    catch (const std::out_of_range& oor)//on ne veut pas afficher les erreurs de out of range
    {
     //   std::cerr << "" << oor.what();
    }
    try
    {
        (grille.at(x+1).at(y-2));//test une position
        dispo[x+1][y-2]=true;//si on y arrive marque le avec un true
    }
    catch (const std::out_of_range& oor)//on ne veut pas afficher les erreurs de out of range
    {
     //   std::cerr << " " << oor.what();
    }
    try
    {
        (grille.at(x-1).at(y+2));//test une position
        dispo[x-1][y+2]=true;//si on y arrive marque le avec un true
    }
    catch (const std::out_of_range& oor)//on ne veut pas afficher les erreurs de out of range
    {
        //std::cerr << "" << oor.what();
    }
    try
    {
        (grille.at(x-1).at(y-2));//test une position
        dispo[x-1][y-2]=true;//si on y arrive marque le avec un true
    }
    catch (const std::out_of_range& ooor)//on ne veut pas afficher les erreurs de out of range
    {
        //std::cerr << " " << ooor.what();
    }
    int v=0;//incrementer le tableau text plus bas
    /*Cette grosse boucle ameliore la robustese et facilite le placement des positions
     en lui donnant des coordonnés mappées ou il pourrait jouer pas forcement DEVRAIT en plus
     de sauvegarder ces possibilités et sil joue une position qui n'est pas dans la liste
     on renvoie un message en lui precisant quoi faire
     */
    for(int abac=0;abac<8;abac++)
    {
        test[abac]=" ";
    }
    for (int n=0; n<6; n++)
    {
        for (int m=0; m<6; m++)//on est a l'interieur du tableau de booleen
        {
            if (dispo[n][m]==1)//si le deplacement est possible
            {
                int conv=0;string s1="";//on fait une conversion de la position en string pour les comparaison plus tard
                for(auto const &it1 : mymap)
                {
                    for(auto const &it2 : it1.second)//on recherche la correspondance dans le map
                    {
                        if (it2.second==grille[n][m].posiVerou)//s'il match
                        {
                            //on l'affiche a l'utilisateur
                            cout<<"Position disponible : "<<it1.first<<""<<it2.first<<"\n";
                            text=it1.first;//on ajoute directement la lettre a la string
                            conv=it2.first;//on s'apprete a faire un conversion int string
                            stringstream os;//variable de sstream
                            test[v]=text;
                            os<<conv;//on fait la conversion avec string stream
                            s1=os.str();
                            test[v].append(s1);//on ajoute le chiffre a la string
                            v++;//on increment l'indice du tableau de string
                        }
                    }
                }
                
            }

        }
    }

}
//generateur aleatoire d'etoile
void gril::radometoile()
{
    int abs=0;
    int ordo=0;
    srand(time(NULL));//initialisation du temp
    for (int n=0; n<9; n++)
    {
        abs=rand()%6;//une valeur aleatoire de l'indice du tableau entre 0 et 6
        ordo=rand()%6;
        grille[abs][ordo].valeurcase="H";// mettre les etoile comme valeur
        grille[abs][ordo].numerodescase=100;// valeur arbitraire pour savoir si le numero es a 100 ces que ces forcement une etoile

    }
    //on assigne le pourcentage de debut ici au cas ou le random place une etoile a la position du depart
    (grille[5][0]).valeurcase="%";
    (grille[5][0]).numerodescase=50;
}
// affichage de la grille il a ete fait en fonction des lignes
void gril::affichage()
{
    cout<<"  a     b    c    d    e    f\n";
    cout << " +----+----+----+----+----+----+";
    cout<<endl;
    cout<<"0";
    for (int i=0; i<=5; i++)
    {
        cout<<"|";
        cout<<" "<<grille[0][i].valeurcase<<"  ";
        if (i==5)
        {
            cout<<"|0";
        }
    }
    cout<<"\n";
    
    int a=0;// compteur pour afficher chaque chaque valeur de la ligne 1 p
    cout << " +----+----+----+----+----+----+";
    cout<<endl;
    cout<<"1";
    for (int i=5; i<=10; i++)
    {
        cout<<"|";
        cout<<" "<<grille[1][a].valeurcase<<"  ";
        if (a==5)
        {
            cout<<"|1";
        }
        a++;
    }
    cout<<"\n";
    int b=0;// compteur pour afficher chaque chaque valeur de la deuxime ligne
    cout << " +----+----+----+----+----+----+";
    cout<<endl;
    cout<<"2";
    for (int i=10; i<16; i++)
    {
        cout<<"|";
        cout<<" "<<grille[2][b].valeurcase<<"  ";
        if (b==5)
        {
            cout<<"|2";
        }
        b++;
    }
    cout<<"\n";
    int c=0;// compteur pour afficher chaque chaque valeur de la troisieme ligne
    cout << " +----+----+----+----+----+----+";
    cout<<endl;
    cout<<"3";
    for (int i=15; i<21; i++)
    {
        cout<<"|";
        cout<<" "<<grille[3][c].valeurcase<<"  ";
        if (c==5)
        {
            cout<<"|3";
        }
        c++;
    }
    cout<<"\n";
    int d=0;// compteur pour afficher chaque chaque valeur de la 4e ligne
    cout << " +----+----+----+----+----+----+";
    cout<<endl;
    cout<<"4";
    for (int i=20; i<26; i++)
    {
            cout<<"|";
        cout<<" "<<grille[4][d].valeurcase<<"  ";
        if (d==5)
        {
            cout<<"|4";
        }
        d++;
    }
    cout<<"\n";
    int e=0;// compteur pour afficher chaque chaque chiffre de la 5e ligne
    cout << " +----+----+----+----+----+----+";
    cout<<endl;
    cout<<"5";
    for (int i=20; i<26; i++)
    {
        cout<<"|";
        cout<<" "<<grille[5][e].valeurcase<<"  ";
        if (e==5)
        {
            cout<<"|5";
        }
        e++;
    }
    cout<<"\n";
    cout << " +----+----+----+----+----+----+";
    cout<<endl;
    cout<<"  a     b    c    d    e    f\n";
    caluldepoint();

}
//press play fait le mapping de lettre et chiffre pour avoir une representation similaire au jeux d'echec
void gril::pressplay()
{
    mymap["a"][0] = 0;mymap["b"][0] = 1;mymap["c"][0] = 2;mymap["d"][0] = 3;mymap["e"][0] = 4;mymap["f"][0] = 5;
    mymap["a"][1] = 6;mymap["b"][1] = 7;mymap["c"][1] = 8;mymap["d"][1] = 9;mymap["e"][1] = 6;mymap["f"][1] = 11;
    mymap["a"][2] = 12;mymap["b"][2] = 13;mymap["c"][2] = 14;mymap["d"][2] = 15;mymap["e"][2] = 16;mymap["f"][2] = 17;
    mymap["a"][3] = 18;mymap["b"][3] = 19;mymap["c"][3] = 20;mymap["d"][3] = 21;mymap["e"][3] = 22;mymap["f"][3] = 23;
    mymap["a"][4] = 24;mymap["b"][4] = 25;mymap["c"][4] = 26;mymap["d"][4] = 27;mymap["e"][4] = 28;mymap["f"][4] = 29;
    mymap["a"][5] = 30;mymap["b"][5] = 31;mymap["c"][5] = 32;mymap["d"][5] = 33;mymap["e"][5] = 34;mymap["f"][5] = 35;
}
//secure input verifie si le input de l'utilisateur est dans la liste des deplacement en L possible pour empecher de faire nimporte quoi
bool gril::secureInput(string in)
{
    bool p=false;
   
    for (int i=0;i<8; i++)
    {
        if (test[i]==in)
            p=true;
    }
    return p;
}
//fonction pour permettre au joueur de jouer
void gril::Joeur1Gameur()
{
    //le joueur peut jouer ou retirer une case
    cout<<"Joeur 1 a votre tour\n";
    cout<<"======================="<<endl;
    cout<<"1. Jouer un pion"<<endl;
    cout<<"2. Retirer un pion"<<endl;
    cout<<"======================="<<endl;
    int numberchoice;
    do
    {
        cout<<"Entrez votre choix s'il vous plait:\n";
        cin>>numberchoice;
        if(!cin)
        {
            //on verifie qu'il a bien rentrer un entier comme choix
            cout << "Ce n'est pas un entier! Entrez un entier s'il vous plait";
            cin.clear();
            cin.ignore();
        }
    }
    while (numberchoice<1 || numberchoice>2);
    switch(numberchoice)// sil arrive a ce niveau cest quil a fait un choix valide de s'il veut jouer ou retirer une piece
    {
        case 1:
        {  cout<<"Entrez le deplacement";// on lui demande ou il veut jouer
            cout<<endl;
            string posipreci="";string posipre="";
            string position=""; int frechi=0;
            cin>>position;
            while(position.length()>2||position.length()<=0)//on veut juste un string de 2 caractere
            {
                cout<<"Le deplacement entrez n'est pas valide\n";
                cout<<"Entrez un deplacement valide s'il vous plait\n";
                cin>>position;
            }
            while (secureInput(position)!=true)// si le caractere est bon on verifie sil est dans la liste et si oui on le laisse jouer
            {
                cout<<"Seul les deplacements en L sont valide\n";
                cout<<"Entrez un deplacement valide";
                cin>>position;
            }
            posipre=position.substr(0,1);
            posipreci=position.substr(1,1);
            stringstream ss;
            ss.str(posipreci);
            ss>>frechi;
            for (int n=0; n<6; n++)
            {
                for (int m=0; m<6; m++)
                {
                    if (grille[n][m].posiVerou==mymap[posipre][frechi])
                    {
                        if (grille[n][m].alreadyPlay==false)
                        {
                            if (grille[n][m].numerodescase==100)//si il ya une etoile a la case ou il veut jouer
                            {
                                grille[n][m].valeurcase="X";// le joueur 1 aura toujours des X
                                grille[n][m].numerodescase=650;//numero arbitraire pour savoir que yavais une etoile avant ici
                                grille[n][m].alreadyPlay=true;// on marque qu'on a jouer ici
                                joueur1Jeton--;//on diminue sont nombre de jeuton disponible
                                cordoJoeur2X=n;cordoJoeur2Y=m;//on dit au joueur deux ou on a jouer
                            }
                            else
                            {
                                grille[n][m].valeurcase="X";// le joueur 1 aura toujours des X
                                grille[n][m].numerodescase=600;//sil nyavait rien avant on met un chiffre arbitraire
                                grille[n][m].alreadyPlay=true;// on change le booleen pour dire qu'on a jouer ici
                                grille[n][m].retirable=true;// les case au il y avait pas d'etoile sont retirable donc marque qu'il est possible de le faire
                                joueur1Jeton--;//on diminue le nombre de jeuton
                                cordoJoeur2X=n;cordoJoeur2Y=m;//on dit au joueur deux ou on a jouer
                                
                            }
                            
                        }
                        else
                        {
                            // si la case est occuper ou sil veut jouer sur la case d'un autre joueur est considerer comme de la triche on lui envoie un message et on passe la main au joueur2 avec toute les indications il devrait pas essailler de tricher :)
                            cout<<"Vous n'etes pas autoriser a jouer sur une case deja occuper ou occuper par Joueur2\n";
                            cout<<"Vous etes obliger de lui passer la main\n";
                        }
                    }
                }
            }
            break;
        }
        case 2:
        {
            cout<<"Entrez la position du jeton a retirer"<<endl;//entrer le jeuton a retirer
            cout<<endl;
            string posipreci="";string posipre="";//toute ces variable serve au conversion de input string a int
            string position=""; int frechi=0;
            cin>>position;
            while(position.length()>2||position.length()<=0)// on verifie si la string entrer depasse pas une longeur de 2
            {
                cout<<"Le deplacement entrez n'est pas valide\n";
                cout<<"Entrez un deplacement valide s'il vous plait\n";
                cin>>position;
            }
            posipre=position.substr(0,1);
            posipreci=position.substr(1,1);
            stringstream ss;
            ss.str(posipreci);//on fait des conversion string int pour retrouver les positions
            ss>>frechi;
            // pressplay();
            for (int n=0; n<6; n++)
            {
                for (int m=0; m<6; m++)
                {
                    if (grille[n][m].posiVerou==mymap[posipre][frechi])
                    {
                        if (grille[n][m].retirable==true&&grille[n][m].valeurcase=="X")
                        {
                            grille[n][m].valeurcase=" ";// on change la valeur de la case
                            grille[n][m].retirable=false;//on remet la case a un etat au le retrait n'est pas possible
                            joueur1Jeton++;//on lui redonne son jeuton
                        }//note aussi qu'il garde les point obtenu sil retire un jeton pour plus de fun
                    }
                    else
                        cout<<"Retrait de pion echouer cause de position inexistante\n";
                }
            }
            break;
        }
        default:
            cout<<"Erreur choix";
    }
}
//fonction pour permettre au joueur2 de jouer
void gril::Joeur2Gameur()
{
    cout<<"Joeur 2 a votre tour\n";
    cout<<"======================="<<endl;
    cout<<"1. Jouer un pion"<<endl;
    cout<<"2. Retirer un pion"<<endl;
    cout<<"======================="<<endl;//le joueur peut jouer ou retirer une case
    int numberchoice;
    do
    {
        cout<<"Entrez votre choix s'il vous plait:";
        cin>>numberchoice;
        if(!cin)
        {
            cout << "Ce n'est pas un entier! Entrez un entier s'il vous plait";
            cin.clear();
            cin.ignore();
            //on verifie qu'il a bien rentrer un entier comme choix
        }
    }
    while (numberchoice<1 || numberchoice>2);
    switch(numberchoice)
    {
        case 1:
        {
            cout<<"Entrez le deplacement";
            cout<<endl;
            string posipreci="";string posipre="";
            string position=""; int frechi=0;
            cin>>position;// on lui demande ou il veut jouer
            while(position.length()>2||position.length()<=0)//on veut juste un string de 2 caractere
            {
                cout<<"Le deplacement entrez n'est pas valide\n";
                cout<<"Entrez un deplacement valide s'il vous plait\n";
                cin>>position;
            }
            while (secureInput(position)!=true)
            {
                cout<<"Seul les deplacements en L sont valide\n";
                cout<<"Entrez un deplacement valide";
                cin>>position;// si le caractere est bon on verifie sil est dans la liste et si oui on le laisse jouer
            }
            posipre=position.substr(0,1);
            posipreci=position.substr(1,1);
            stringstream ss;
            ss.str(posipreci);
            ss>>frechi;
            cout<<frechi<<endl;
            for (int n=0; n<6; n++)
            {
                for (int m=0; m<6; m++)
                {
                    if (grille[n][m].posiVerou==mymap[posipre][frechi])
                    {
                        if (grille[n][m].alreadyPlay==false)
                        {
                            if (grille[n][m].numerodescase==100)//si il ya une etoile a la case ou il veut jouer
                            {
                                grille[n][m].valeurcase="O";// le joueur 2 aura toujours des O
                                grille[n][m].numerodescase=750;//numero arbitraire pour savoir que yavais une etoile avant ici et que le Joeur2 l'a changer
                                grille[n][m].alreadyPlay=true;// on marque qu'on a jouer ici
                                cordoJoeur1X=n;cordoJoeur1Y=m;//on dit au joueur 1 ou on a jouer
                                joueur2Jeton--;//on diminue d'un jeton de l'ensemble des jeton de l'utilisateur
                            }
                            else
                            {
                                grille[n][m].valeurcase="O";// le joueur 2 aura toujours des O
                                grille[n][m].numerodescase=700;//sil nyavait rien avant on met un chiffre arbitraire
                                grille[n][m].alreadyPlay=true;// on change le booleen pour dire qu'on a jouer ici
                                grille[n][m].retirable=true;// les case au il y avait pas d'etoile sont retirable donc marque qu'il est possible de le faire
                                joueur2Jeton--;//on diminue le nombre de jeuton
                                cordoJoeur1X=n;cordoJoeur1Y=m;//on dit au joueur 1 ou on a jouer
                                
                            }
                        }
                        else
                        {
                            cout<<"Vous n'etes pas autoriser a jouer sur une case deja occuper par Joueur2\n";
                            cout<<"Vous etes obliger de lui passer la main\n";
                        }
                        
                    }
                }
            }
            break;
        }
        case 2:
        {
            cout<<"Entrez la position du jeton a retirer"<<endl;//entrer le jeuton a retirer
            cout<<endl;
            string posipreci="";string posipre="";//toute ces variable serve au conversion de input string a int
            string position=""; int frechi=0;
            cin>>position;
            while(position.length()>2||position.length()<=0)// on verifie si la string entrer depasse pas une longeur de 2
            {
                cout<<"Le deplacement entrez n'est pas valide\n";
                cout<<"Entrez un deplacement valide s'il vous plait\n";
                cin>>position;
            }
            posipre=position.substr(0,1);
            posipreci=position.substr(1,1);
            stringstream ss;
            ss.str(posipreci);//on fait des conversion string int pour retrouver les positions
            ss>>frechi;
            // pressplay();
            for (int n=0; n<6; n++)
            {
                for (int m=0; m<6; m++)
                {
                    if (grille[n][m].posiVerou==mymap[posipre][frechi])
                    {
                        if (grille[n][m].retirable==true&&grille[n][m].valeurcase=="O")
                        {
                            grille[n][m].valeurcase=" ";// on change la valeur de la case
                            grille[n][m].retirable=false;//on remet la case a un etat au le retrait n'est pas possible
                            joueur2Jeton++;//on lui redonne son jeuton
                        //note aussi qu'il garde les point obtenu sil retire un jeton pour plus de fun
                        }
                        else
                        {
                            cout<<"Vous essailler de retirer une case qui vous appartient pas\n";
                            cout<<"Vous devez passez la main\n";
                        }
                    }
                }
            }
            break;
        }
        default:
            cout<<"Erreur choix";
    }

}

//s'il ya plus de case ou jouer on arrete le jeux
bool gril::casevide()
{
    bool emptycase=false;
    for (int n=0; n<6; n++)
    {
        for (int m=0; m<6; m++)
        {
            if (grille[n][m].valeurcase==" ")//s'il n'y a ne serait qu'une case vide le bool est a true
            {
                emptycase=true;
            }
        }
    }
    return emptycase;
}
//Fonction de calcul de point
bool gril::caluldepoint()
{
    //cest ici on voi l'utiliter de l'assignation des chiffre arbitraire
    int pointdujoueur1=0;
    int pointdujoueur2=0;
    for (int n=0; n<6; n++)
    {
        for (int m=0; m<6; m++)
        {
            if (grille[n][m].numerodescase==600)// si une case est a 600 il appartient au joueur1
            {
                pointdujoueur1+=5;
            }
            if (grille[n][m].numerodescase==700)//si une case est a 700 il appartient au joueur2
            {
                pointdujoueur2+=5;
            }
            if (grille[n][m].numerodescase==750)//si une case est a 750 il appartient au joueur2 et il avait une etoile avant donc Bonus
            {
                pointdujoueur2+=10;
            }
            if (grille[n][m].numerodescase==650)//si une case est a 650 il appartient au joueur1 et il avait une etoile avant donc Bonus
            {
                pointdujoueur1+=10;
            }
        }
    }
    cout<<"==========Score=============\n";
    cout<<"Joueur1:"<<pointdujoueur1<<"      Joueur2:"<<pointdujoueur2<<"\n";
    cout<<"Joueur1 X:"<<joueur1Jeton<<"     Joueur2 O:"<<joueur2Jeton<<"\n";
    cout<<"==========Score=============\n";
    if(pointdujoueur1>=100||pointdujoueur2>=100)
        return true;// si il a un joueur qui obtient plus de 100 on met a true le booleen
    else
        return false;// sinon elle sera tout le temp false
}
bool gril:: WehaveAWinner()//On a un gagnant si un joueur a +de 100 ou s'il n'a plus de jeton ou s'il n'y a plus de case ou jouer
{
    if(caluldepoint()==1||joueur1Jeton==0||joueur2Jeton==0||casevide()==0)
        return true;
    else
        return false;
}
//Partie Comunication de processus
int main(int argc, const char * argv[])
{
    //appel de fonction de la class
    gril Jouons;
    Jouons.pressplay();//fait le mapping
    Jouons.radometoile();//genere les etoiles
    int Fils1_Fils2[2];//tableau pour le pipe 0 mode read 1 mode write
    int ret=pipe(Fils1_Fils2);//initialisation du pipe pour la comunication entre pere et fils
    if(ret==-1)
    {
        perror("pipe a echoue l'allocation");
        exit(EXIT_FAILURE);//gestion d'erreur au cas ou on echou l'allocation du pipe
    }
    int n = fork();//creation d'un processus
    if (n<0)
    {
        perror("Echoue a la creation de processus");
        exit(EXIT_FAILURE);//gestion d'erreure en cas d'erreur de creation du processus
    }
    if (n != 0)//A ce niveau ces quo'on est dans le pere
    {
        int p = fork();//creation d'un autre processus pour le fils2
        if (p!=0)
        {
            //zone de d'appelle de fonction du pere
            int EcritVals = write(Fils1_Fils2[1], &Jouons, sizeof(gril));
            int c, w;
            w = wait(&c);w = wait(&c);//attendre que les deux fils se termine
            EcritVals = read(Fils1_Fils2[0], &Jouons, sizeof(gril));//a la fin de
            Jouons.caluldepoint();//on affiche le score a la fin
        }
        else //FILS2 on fait jouer dans fils 2 le joueur2
        {
            bool FirsTimeHere = false ; int EtatGrille;
            while(true)//on fait une boucle infini pour permetre la communication continuelle
            {
                EtatGrille = read(Fils1_Fils2[0], &Jouons, sizeof(gril)); // Lecture de l'état de la grille
                
                if (!FirsTimeHere)
                {
                    //on utilise un read bloquant qui attend que les modification apporter par le fils qui ecrire dans le tube afin quil puisse lire dans le tube
                    //sinon il attendra tant que le tube sera vide
                    //apres quand on viendra plus ici ces le read plus haut qui sera blockant
                    
                    EtatGrille = read(Fils1_Fils2[0], &Jouons, sizeof(gril));
                    
                    FirsTimeHere = true;//on revient plus ici nos sleep nous permetrons de synchroniser la communication
                }
                if (!Jouons.WehaveAWinner())  // Jouer tant qu'on a pas un gagant
                {
                    printf("Valeur de fork = %d ",p);
                    printf("processus %d de pere %d\n", (int) getpid(), (int) getppid());//Affichage du processus pour savoir que les de
                    Jouons.affichage();
                    Jouons.deplacement(Jouons.GetcordoJoeur2X(),Jouons.GetcordoJoeur2Y());//on recupere nos cordoner a partir desquel on jouera
                    Jouons.Joeur2Gameur();//demande au joueur2 de jouer
                    Jouons.affichage();// affiche sa modification
                    
                    EtatGrille = write(Fils1_Fils2[1], &Jouons, sizeof(gril));//on ecrit ce quil a modifier
                }
                else//Apparament on a un gagnant
                {
                    EtatGrille = write(Fils1_Fils2[1], &Jouons, sizeof(gril));//On ecrit l'etat de la grille au pere
                    break;
                }
                sleep(2);//On l'endort juste deux seconde le temps de passer la mais quand il se reveille le tube est vide il a un read blcokant plus haut et il attend de recevoir des donner dans le tube
            }
            exit(2);//On demande au fils2 de se suicider a la fin de la partie
        }
    }
    else//FILS1 on fait jouer dans fils 1 le joueur1
    {
        printf("Valeur de fork = %d ",n);
        printf("processus %d de pere %d\n", (int) getpid(), (int) getppid());//affichage du numero du processus associer a son pere pour verifier si il est le meme que celui du pere declarer plus haut
        
        bool FirsTimeHere = false ; int EtatGrille;
        while(true)
        {
            if(!FirsTimeHere)//Comme on sait ou commencer on assigne directment les positions
            {
                Jouons.affichage();
                Jouons.deplacement(5,0);//le joueur connais ces deplacement possible
                Jouons.Joeur1Gameur();//on lui demande de jouer
                FirsTimeHere = true;//on change le b
                EtatGrille= write(Fils1_Fils2[1], &Jouons, sizeof(gril));//on envoie la grille au joueur2 pour quil joue
            }
            else
            {
                EtatGrille = read(Fils1_Fils2[0], &Jouons, sizeof(gril));//on lie la grille modifier
    
                if(!Jouons.WehaveAWinner())//si on a pas de gagant on laisse le fils1 jouer
                {
                    Jouons.deplacement(Jouons.GetcordoJoeur1X(),Jouons.GetcordoJoeur1Y());
                    Jouons.Joeur1Gameur();
                    Jouons.affichage();
                    EtatGrille = write(Fils1_Fils2[1], &Jouons, sizeof(gril));//fini de jouer? renvoie le au fils2
                }
                else
                {
                    EtatGrille = write(Fils1_Fils2[1], &Jouons, sizeof(gril));//si on a un gagnant on ecrit les modifications de la grille que le pere interpretera
                    break;
                }
            }
            sleep(2);//le sleep de quelque seconde permet au fils2 de reprendre la main
        }
        exit(1);// on demande au fils de se tuer quand on fini de jouer
    }
    return 0;
}