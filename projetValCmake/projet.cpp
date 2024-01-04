#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <mutex>
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
    double vitesse;  // Vitesse du m�tro en km/h
    double vitessemax = 150;// L'acceleration maximale que peut atteindre mon m�tro
    double acceleration = 1;
    double deceleration = 1;
    bool freinage = 0;
    bool voie = 0;
    float position;    // Position actuelle du m�tro sur la ligne (en m�tres)
    int stationIndex; // Index de la station actuelle dans le vecteur de stations
    vector<Station> stations;  // Vecteur de stations


public:
    // Constructeur de la classe
    Metro(const string& nom, double vitesse, const vector<Station>& stations)
        : nom(nom), passager(0), vitesse(vitesse), position(0), stationIndex(0), stations(stations) {
        cout << "Metro instancie avec succes" << endl;
    }

    float getPos() {
        return(position);
    }

    //Fonction vitesse , g�rant la d�celeration et l'acc�leration
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





    // Fonction de d�placement du m�tro 
    void deplacer() {
        while (true) {
            if (!voie) {
                this->position = 0;
                stationIndex = 0;
                cout << stations.size() << endl;

                while (stationIndex < stations.size() - 1) {
                    int distanceVersProchaineStation = stations[stationIndex + 1].getDistanceV1();
                    double tempsDeplacement = static_cast<double>(distanceVersProchaineStation) / vitesse * 3600; // Temps en secondes
                    double tempsArret = 5.0; // Temps d'arr�t � chaque station en secondes

                    // D�placement entre les stations
                    while (position < stations[stationIndex + 1].getDistanceV1()) {
                        if (position >= stations.back().getDistanceV1()) {
                            break;
                        }
                        this_thread::sleep_for(std::chrono::milliseconds(20)); // Pause d'une seconde entre les mises � jour de position
                        position += vitesse; // Mise � jour de la position chaque seconde

                        Vitesse(distanceVersProchaineStation);



                        // Affichage de l'�tat du m�tro
                        cout << "Metro " << nom << " en route vers la station " << stations[stationIndex + 1].getNom();
                        cout << ". Position : " << position << " metres." << endl;
                    }
                    this->vitesse = 0;
                    this->freinage = 0;

                    // Le m�tro atteint la prochaine station
                    position = stations[stationIndex + 1].getDistanceV1();
                    stationIndex+=1;

                    // Affichage de l'arriv�e � la station
                    cout << "Metro " << nom << " arrive a la station " << stations[stationIndex].getNom() << "." << endl;
                    gererPassagers();

                    // Pause d'arr�t � la station
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
                    double tempsArret = 5.0; // Temps d'arr�t � chaque station en secondes

                    // D�placement entre les stations
                    while (position > stations[stationIndex - 1].getDistanceV2()) {

                        position = position+vitesse;

                        Vitesse(distanceVersProchaineStation);



                        // Affichage de l'�tat du m�tro
                        cout << "Metro " << nom << " en route vers la station " << stations[stationIndex - 1].getNom();
                        cout << ". Position : " << position << " metres." << endl;

                        this_thread::sleep_for(std::chrono::milliseconds(20)); // Pause d'une seconde entre les mises � jour de position

                    }
                    this->vitesse = 0;
                    this->freinage = 0;

                    // Le m�tro atteint la prochaine station
                    position = stations[stationIndex - 1].getDistanceV2();
                    --stationIndex;

                    // Affichage de l'arriv�e � la station
                    cout << "Metro " << nom << " arrive a la station " << stations[stationIndex].getNom() << "." << endl;
                    gererPassagers();

                    // Pause d'arr�t � la station
                    this_thread::sleep_for(chrono::seconds(static_cast<int>(tempsArret)));
                }
                voie = false;

            }


        }




        
    }

    // M�thode pour ajouter des passagers au m�tro
    void ajouterPassagers(int nombrePassagers) {


        passager += nombrePassagers;
        cout << "Ajout de " << nombrePassagers << " passagers dans le metro." << endl;
         cout << "Nombre total de passagers dans le metro : " << passager << endl;
        cout << "--------------" << endl;
    }

    void gererPassagers() {
        // Faire descendre une partie des passagers actuels
        if (voie) {
            int passagersADescendre = rand() % (passager + 1); // Entre 1 et la moiti� des passagers actuels
            passager -= passagersADescendre;

            cout << passagersADescendre << " passagers descendent du metro." << endl;
            cout << "Nombre total de passagers dans le metro : " << passager << endl;
            cout << "--------------" << endl;

            // Faire monter de nouveaux passagers
            int espaceDisponible = (passager < 50) ? (50 - passager) : 0; // Calculer l'espace disponible dans le m�tro. IMPORTANT de faire le check ici pour ne pas avoir une division par 0

            if (espaceDisponible > 0) {
                int passagersAMonter = rand() % (espaceDisponible + 1); // Entre 0 et l'espace disponible

                // V�rifier s'il y a plus de gens sur le quai que d'espace disponible dans le m�tro
                if (stations[stationIndex].getDistanceV1() >= passagersAMonter) {
                    cout << passagersAMonter << " passagers montent dans le metro." << endl;
                    passager += passagersAMonter;
                }
                else {
                    cout << "Il y a trop de gens sur le quai. Certains ne peuvent pas monter." << endl;
                }
            }
            else {
                cout << "Le m�tro est plein. Aucun passager ne peut monter." << endl;
            }

            cout << "Nombre total de passagers dans le metro : " << passager << endl;
            cout << "--------------" << endl;
        }
        else {
            int passagersADescendre = rand() % (passager + 1); // Entre 1 et la moiti� des passagers actuels
            passager -= passagersADescendre;

            cout << passagersADescendre << " passagers descendent du metro." << endl;
            cout << "Nombre total de passagers dans le metro : " << passager << endl;
            cout << "--------------" << endl;

            // Faire monter de nouveaux passagers
            int espaceDisponible = (passager < 50) ? (50 - passager) : 0; // Calculer l'espace disponible dans le m�tro. IMPORTANT de faire le check ici pour ne pas avoir une division par 0

            if (espaceDisponible > 0) {
                int passagersAMonter = rand() % (espaceDisponible + 1); // Entre 0 et l'espace disponible

                // V�rifier s'il y a plus de gens sur le quai que d'espace disponible dans le m�tro
                if (stations[stationIndex].getDistanceV2() >= passagersAMonter) {
                    cout << passagersAMonter << " passagers montent dans le metro." << endl;
                    passager += passagersAMonter;
                }
                else {
                    cout << "Il y a trop de gens sur le quai. Certains ne peuvent pas monter." << endl;
                }
            }
            else {
                cout << "Le m�tro est plein. Aucun passager ne peut monter." << endl;
            }

            cout << "Nombre total de passagers dans le metro : " << passager << endl;
            cout << "--------------" << endl;
        }

        
    }
};

int main() {
    Station rame1("Bourg-en-Vol", 1, 0, 0);
    Station rame2("Volucite", 2, 500,500);
    Station rame3("Renouet", 3, 1200,1200);
    Station rame4("Argenta", 4, 1450,1450);
    Station rame5("Rotombourg", 5,2000,2000);
    Station rame6("Verdansk", 1, 0, 0);
    Station rame7("Tarkov", 2, 500, 500);
    Station rame8("Neuilly", 3, 1200, 1200);
    Station rame9("Boutroux", 4, 1450, 1450);
    Station rame10("", 5, 2000, 2000);
    rame1.afficherInformations1();
    rame2.afficherInformations1();
    rame3.afficherInformations1();
    rame4.afficherInformations1();
    rame5.afficherInformations1();
    vector<Station> stations = { rame1, rame2, rame3, rame4, rame5 };
    Metro metro("Ligne A", 0, stations);
    Metro metroB("Ligne ABis", 4, stations);
    thread metro1(&Metro::deplacer, &metro);
    thread metro2(&Metro::deplacer, &metroB);
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Syst�me de m�tro");
    while (window.isOpen()) {
        window.clear(sf::Color::White);
        sf::RectangleShape metroShape(sf::Vector2f(75,25)); // Exemple de forme de m�tro (cercle de rayon 20 pixels)
        metroShape.setFillColor(sf::Color::Red); // Couleur du m�tro (ici rouge)
        if (metro.getPos() < 1920) {
            metroShape.setPosition(metro.getPos(), 50);
        }
        if(metro.getPos()>=1920) {
            metroShape.setPosition(metro.getPos()-1920, 200);
        }
        window.draw(metroShape);
        // Dessin de la station
        sf::CircleShape stationShape(20);
        stationShape.setFillColor(sf::Color::Black);
        sf::RectangleShape ligne(sf::Vector2f(1920, 5));
        ligne.setPosition(0, 66);
        ligne.setFillColor(sf::Color::Red);
        window.draw(ligne);
        for (int i = 0; i < stations.size(); i++) {
            stationShape.setPosition(stations[i].getDistanceV1()+15,50);
            window.draw(stationShape);
        }
        window.display();
    }
    metro1.join();
    return 0;
}