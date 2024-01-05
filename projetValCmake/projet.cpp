#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <SFML/Graphics/Font.hpp>
using namespace std;

//---- PARTIE CLASSE ET FONCTION ----

//---- CLASSE STATION ----
class Station {
private:
    string nom;//nom de la station
    int id;//id de la station
    int distanceV1;
    int distanceV2;
   

public:

    Station(const string& nom, int id, int distanceV1 ,int distanceV2) : nom(nom), id(id), distanceV1(distanceV1), distanceV2(distanceV2) {}//constructeur de la classe station


    string getNom() const {
        return nom;//renvoi le nom de la station
    }

    int getDistanceV1() const {
        return distanceV1;//renvoi la distance a laquelle

    }

    int getDistanceV2() const {
        return distanceV2;//renvoi la distance a laquelle

    }

    void afficherInformations1() const {// affiche les informations de la station
        cout << "Nom de la station : " << nom << endl;
        cout << "ID de la station : " << id << endl;
        cout << "Distance : " << distanceV1 << endl;

    }
    void afficherInformations2() const {// affiche les informations de la station
        cout << "Nom de la station : " << nom << endl;
        cout << "ID de la station : " << id << endl;
        cout << "Distance : " << distanceV2 << endl;

    }
};

//---- CLASSE METRO ----
class Metro {
private:
    string nom;
    int passager;
    double vitesse;  // Vitesse du métro en km/h
    double vitessemax = 150;// L'acceleration maximale que peut atteindre mon métro
    double acceleration = 1;
    double deceleration = 1;
    bool freinage = 0;
    bool voie = 0;
    float position;    // Position actuelle du métro sur la ligne (en mètres)
    int stationIndex; // Index de la station actuelle dans le vecteur de stations
    vector<Station> stations;  // Vecteur de stations


public:
    // Constructeur de la classe
    Metro(const string& nom, double vitesse, const vector<Station>& stations)
        : nom(nom), passager(0), vitesse(vitesse), position(0), stationIndex(0), stations(stations) {
        cout << "Metro instancie avec succes" << endl;
    }

    int getPass() {
        return passager;
    }
    int getStatInd() {
        return stationIndex;
    }
    float getPos() {
        return(position);
    }
    bool getVoie() {
        return voie;
    }

    //Fonction vitesse , gérant la déceleration et l'accéleration
    int Vitesse(int distanceVersProchaineStationBIS) {
        
        double distancetempo = distanceVersProchaineStationBIS - this->position ;
        double distanceFreinage = (vitesse * (vitesse + deceleration) / (2 * deceleration)) + this->vitesse ;
       

        if (distancetempo == this->vitesse) {
            
            return this->vitesse;
        }
            
        if (this->vitesse <= 30 && this->freinage && this->vitesse >= 15 ) {


            if (distancetempo <= this->vitesse+5) {
                this->vitesse -= 5;
               

            }
            return this->vitesse;
        }

        if (distancetempo < distanceFreinage && !this->freinage) {
            this->freinage = 1;
            
            return this->vitesse;
        }

        if (vitesse < vitessemax && !this->freinage) {
            this->vitesse += acceleration;
            

            return this->vitesse;
        }

        if (this->freinage) {
            this->vitesse -= deceleration;
            

            return this->vitesse;
        }



        return this->vitesse;
    }





    // Fonction de déplacement du métro 
    void deplacer() {
        while (true) {
            if (!voie) {
                this->position = 0;
                stationIndex = 0;
                cout << stations.size() << endl;

                while (stationIndex < stations.size() - 1) {
                    int distanceVersProchaineStation = stations[stationIndex + 1].getDistanceV1();
                    double tempsDeplacement = static_cast<double>(distanceVersProchaineStation) / vitesse * 3600; // Temps en secondes
                    double tempsArret = 5.0; // Temps d'arrêt à chaque station en secondes

                    // Déplacement entre les stations
                    while (position < stations[stationIndex + 1].getDistanceV1()) {
                        if (position >= stations.back().getDistanceV1()) {
                            break;
                        }
                        this_thread::sleep_for(std::chrono::milliseconds(20)); // Pause d'une seconde entre les mises à jour de position
                        position += vitesse; // Mise à jour de la position chaque seconde

                        Vitesse(distanceVersProchaineStation);



                        // Affichage de l'état du métro
                        cout << "Metro " << nom << " en route vers la station " << stations[stationIndex + 1].getNom();
                        cout << ". Position : " << position << " metres." << endl;
                    }
                    this->vitesse = 0;
                    this->freinage = 0;

                    // Le métro atteint la prochaine station
                    position = stations[stationIndex + 1].getDistanceV1();
                    stationIndex+=1;

                    // Affichage de l'arrivée à la station
                    cout << "Metro " << nom << " arrive a la station " << stations[stationIndex].getNom() << "." << endl;
                    gererPassagers();

                    // Pause d'arrêt à la station
                    this_thread::sleep_for(chrono::seconds(static_cast<int>(tempsArret)));
                }
                voie = true;

            }

            else {
                
                this->position = stations[stations.size()-1].getDistanceV2();
                stationIndex = stations.size()-1;
                cout << stations.size() << endl;


                while (stationIndex > 0) {

                    int distanceVersProchaineStation = stations[stationIndex - 1].getDistanceV2();
                    double tempsDeplacement = static_cast<double>(distanceVersProchaineStation) / vitesse * 3600; // Temps en secondes
                    double tempsArret = 5.0; // Temps d'arrêt à chaque station en secondes

                    // Déplacement entre les stations
                    while (position > stations[stationIndex - 1].getDistanceV2()) {

                        position = position+vitesse;

                        Vitesse(distanceVersProchaineStation);



                        // Affichage de l'état du métro
                        cout << "Metro " << nom << " en route vers la station " << stations[stationIndex - 1].getNom();
                        cout << ". Position : " << position << " metres." << endl;

                        this_thread::sleep_for(std::chrono::milliseconds(20)); // Pause d'une seconde entre les mises à jour de position

                    }
                    this->vitesse = 0;
                    this->freinage = 0;

                    // Le métro atteint la prochaine station
                    position = stations[stationIndex - 1].getDistanceV2();
                    --stationIndex;

                    // Affichage de l'arrivée à la station
                    cout << "Metro " << nom << " arrive a la station " << stations[stationIndex].getNom() << "." << endl;
                    gererPassagers();

                    // Pause d'arrêt à la station
                    this_thread::sleep_for(chrono::seconds(static_cast<int>(tempsArret)));
                }
                voie = false;

            }


        }




        
    }

    // Méthode pour ajouter des passagers au métro
    void ajouterPassagers(int nombrePassagers) {


        passager += nombrePassagers;
        cout << "Ajout de " << nombrePassagers << " passagers dans le metro." << endl;
         cout << "Nombre total de passagers dans le metro : " << passager << endl;
        cout << "--------------" << endl;
    }

    void gererPassagers() {
        // Faire descendre une partie des passagers actuels
        if (voie) {
            int passagersADescendre = rand() % (passager + 1); // Entre 1 et la moitié des passagers actuels
            passager -= passagersADescendre;

            cout << passagersADescendre << " passagers descendent du metro." << endl;
            cout << "Nombre total de passagers dans le metro : " << passager << endl;
            cout << "--------------" << endl;

            // Faire monter de nouveaux passagers
            int espaceDisponible = (passager < 50) ? (50 - passager) : 0; // Calculer l'espace disponible dans le métro. IMPORTANT de faire le check ici pour ne pas avoir une division par 0

            if (espaceDisponible > 0) {
                int passagersAMonter = rand() % (espaceDisponible + 1); // Entre 0 et l'espace disponible

                // Vérifier s'il y a plus de gens sur le quai que d'espace disponible dans le métro
                if (stations[stationIndex].getDistanceV1() >= passagersAMonter) {
                    cout << passagersAMonter << " passagers montent dans le metro." << endl;
                    passager += passagersAMonter;
                }
                else {
                    cout << "Il y a trop de gens sur le quai. Certains ne peuvent pas monter." << endl;
                }
            }
            else {
                cout << "Le métro est plein. Aucun passager ne peut monter." << endl;
            }

            cout << "Nombre total de passagers dans le metro : " << passager << endl;
            cout << "--------------" << endl;
        }
        else {
            int passagersADescendre = rand() % (passager + 1); // Entre 1 et la moitié des passagers actuels
            passager -= passagersADescendre;

            cout << passagersADescendre << " passagers descendent du metro." << endl;
            cout << "Nombre total de passagers dans le metro : " << passager << endl;
            cout << "--------------" << endl;

            // Faire monter de nouveaux passagers
            int espaceDisponible = (passager < 50) ? (50 - passager) : 0; // Calculer l'espace disponible dans le métro. IMPORTANT de faire le check ici pour ne pas avoir une division par 0

            if (espaceDisponible > 0) {
                int passagersAMonter = rand() % (espaceDisponible + 1); // Entre 0 et l'espace disponible

                // Vérifier s'il y a plus de gens sur le quai que d'espace disponible dans le métro
                if (stations[stationIndex].getDistanceV2() >= passagersAMonter) {
                    cout << passagersAMonter << " passagers montent dans le metro." << endl;
                    passager += passagersAMonter;
                }
                else {
                    cout << "Il y a trop de gens sur le quai. Certains ne peuvent pas monter." << endl;
                }
            }
            else {
                cout << "Le métro est plein. Aucun passager ne peut monter." << endl;
            }

            cout << "Nombre total de passagers dans le metro : " << passager << endl;
            cout << "--------------" << endl;
        }

        
    }
};

int main() {
    //déclaration des rames---------------------------------------
    Station rame1("Bourg-en-Vol", 1, 0, 0);
    Station rame2("Volucite", 2, 500,500);
    Station rame3("Renouet", 3, 1200,1200);
    Station rame4("Argenta", 4, 1450,1450);
    Station rame5("Rotombourg", 5,1800,1800);
    Station rame6("Verdansk", 6, 0, 0);
    Station rame7("Levski", 7, 856, 856);
    Station rame8("Neuilly", 8, 1100, 1100);
    Station rame9("Boutroux", 9, 1850, 1850);
    //------------------------------------------------------------
    vector<Station> LigneA = { rame1, rame2, rame3, rame4, rame5 };
    vector<Station> ligneB = { rame6,rame7,rame8,rame9 };
    Metro metro("Ligne A", 0, LigneA);
    Metro metroB("Ligne B", 6, ligneB);
    thread metro1(&Metro::deplacer, &metro);
    thread metro2(&Metro::deplacer, &metroB);
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Système de métro");
    sf::Font Parisine;
    if (!Parisine.loadFromFile("Parisine Regular.otf")) {
        throw("erreur chargement police");
    }
    //debut de la bocle SFML---------------------------------------
    while (window.isOpen()) {
        window.clear(sf::Color::White);
        //création des deux metros----------------------
        sf::RectangleShape metroShape(sf::Vector2f(75,25));
        sf::RectangleShape metroShape2(sf::Vector2f(75, 25));
        metroShape.setFillColor(sf::Color::Red);
        metroShape2.setFillColor(sf::Color::Green);
        //-----------------------------------------------
        //déplacement graphique des metros---------------
        metroShape.setPosition(metro.getPos(), 50);
        metroShape2.setPosition(metroB.getPos(), 500);
        window.draw(metroShape);
        window.draw(metroShape2);
        // Dessin de la station--------------------------
        sf::CircleShape stationShape(20);
        int cpt1=0;
        stationShape.setFillColor(sf::Color::Black);
        sf::RectangleShape ligneARec(sf::Vector2f(1920, 5));
        sf::RectangleShape ligneBRec(sf::Vector2f(1920, 5));
        ligneARec.setPosition(0, 66);
        ligneBRec.setPosition(0, 516);
        ligneARec.setFillColor(sf::Color::Red);
        ligneBRec.setFillColor(sf::Color::Green);
        window.draw(ligneARec);
        window.draw(ligneBRec);
        sf::Text nomStation;
        sf::Text affPass;
        sf::Text affPass2;
        affPass.setColor(sf::Color::Black);
        affPass2.setColor(sf::Color::Black);
        affPass.setFont(Parisine);
        affPass2.setFont(Parisine);
        nomStation.setFont(Parisine);
        nomStation.setCharacterSize(24);
        nomStation.setFillColor(sf::Color::Black);

        if (metro.getStatInd() < LigneA.size() - 1 || metro.getStatInd()-1 == 1) {
            affPass.setString("Prochain arrêt :" + LigneA[metro.getStatInd() + 1].getNom());
        }
        if (metro.getStatInd() == LigneA.size() - 1 || (metro.getVoie() == true && metro.getStatInd()>1)) {
            affPass.setString("Prochain arrêt :" + LigneA[metro.getStatInd() - 1].getNom());
        }

        if (metroB.getStatInd() < ligneB.size() - 1) {
            affPass2.setString("Prochain arrêt :" + ligneB[metroB.getStatInd() + 1].getNom());
        }
        if ((metroB.getStatInd() == ligneB.size() - 1) || (metroB.getVoie() == true && metroB.getStatInd()>6)) {
            affPass2.setString("Prochain arrêt :" + ligneB[metroB.getStatInd()-1].getNom());
        }

        for (int i = 0; i < LigneA.size(); i++) {
            string nomStat = LigneA[i].getNom();
            nomStation.setString(nomStat);
            nomStation.setPosition(LigneA[i].getDistanceV1()-10, 85);
            stationShape.setPosition(LigneA[i].getDistanceV1()+15,50);
            affPass.setPosition(200, 700);
            affPass2.setPosition(200, 800);
            window.draw(nomStation);
            window.draw(stationShape);
        }
        for (int i = 0; i < ligneB.size(); i++) {
            string nomStat = ligneB[i].getNom();
            nomStation.setString(nomStat);
            nomStation.setPosition(ligneB[i].getDistanceV1() - 5, 535);
            stationShape.setPosition(ligneB[i].getDistanceV1() + 15, 500);
            window.draw(stationShape);
            window.draw(nomStation);
            sf::Text LigneAT;
            sf::Text LigneBT;
            LigneAT.setFont(Parisine);
            LigneBT.setFont(Parisine);
            LigneAT.setString("Ligne A :");
            LigneAT.setColor(sf::Color::Red);
            LigneBT.setString("Ligne B :");
            LigneBT.setColor(sf::Color::Green);
            LigneAT.setPosition(80, 700);
            LigneBT.setPosition(80, 800);
            window.draw(LigneAT);
            window.draw(LigneBT);
            window.draw(affPass);
            window.draw(affPass2);
        }

        window.display();
    }
    metro1.join();
    metro2.join();
    return 0;
}
//fin programme