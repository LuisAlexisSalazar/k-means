
#include <sstream>   
#include <fstream>  
#include<vector>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<SFML/Graphics.hpp>
#include "CPunto.h"



int cantidad_registros = 1000;

vector<CPunto*> puntos;


//--------Limites----------
long double limite_superior_x = 0;
long double limite_inferior_x = 0;

long double limite_superior_y = 0;
long double limite_inferior_y = 0;
//------------------------------

vector<CPunto*> centroides;


//pickup_longitud y pickup_latitude.
void readcsv() {
    string line;
    fstream file;
    long double x, y;
    int i = 0;
    
    file.open("yellow_tripdata_2015-01-06.csv",ios::in);

    puntos.resize(cantidad_registros);
    getline(file, line);
    cout << line << endl;



    //----------Primer Punto---------
    getline(file, line);
    stringstream temp_line(line);
    string temp_dato;
    cout << line << endl;
    for (int j = 0; j < 7; j++) {
        getline(temp_line, temp_dato, ',');
        if (j == 5) {
            //cout << dato << endl;
            x = stod(temp_dato);

            limite_superior_x = x;
            limite_inferior_x = x;
        }
        else if (j == 6) {
            //cout << dato << endl;
            y = stod(temp_dato);
            limite_superior_y = y;
            limite_inferior_y = y;
        }

    }
    puntos[i] = new CPunto(x, y);
    i++;


    //----------Resto de Puntos---------
    while (getline(file, line) && i < cantidad_registros) {
        stringstream lineStream(line);
        string dato;
        

        //cout << line << endl;
        //Obtener la longitud y altitud
        for ( int j = 0; j < 7; j++) {   
            getline(lineStream, dato, ',');
            
            if (j == 5) {
                cout << dato << endl;
                x = stod(dato);

                if (limite_superior_x < x)
                    limite_superior_x = x;

                if (x < limite_inferior_x)
                    limite_inferior_x = x;
            }

            else if(j==6)  {
                cout << dato << endl;
                y = stod(dato);

                if (limite_superior_y < y)
                    limite_superior_y = y;

                if (y < limite_inferior_y)
                    limite_inferior_y = y;
            }
        }

        puntos[i] = new CPunto(x,y);
        i++;
    }
}

//sample.csv
void readcsv2() {
    string line;
    ifstream file("sample.csv");

    puntos.resize(cantidad_registros);
    getline(file, line);
    getline(file, line);
    

    //Primer Punto
    getline(file, line);
    stringstream temp_lines(line);
    string collu;
    long double t_x, t_y;
    getline(temp_lines, collu, ',');
    getline(temp_lines, collu, ',');
    getline(temp_lines, collu, ',');

    getline(temp_lines, collu, ',');
    getline(temp_lines, collu, ',');
    getline(temp_lines, collu, ',');

    stringstream ss;
    ss << collu;
    ss >> t_x;
    getline(temp_lines, collu, ',');
    stringstream ss2;
    ss2 << collu;
    ss2 >> t_y;
    puntos[0] = new CPunto(t_x, t_y);
    //----------------------
    

    for (int i = 1; i < cantidad_registros; i++)
    {
        getline(file, line);

        stringstream lines(line);

        string col;

        long double x, y;

        getline(lines, col, ',');
        getline(lines, col, ',');
        getline(lines, col, ',');

        getline(lines, col, ',');
        getline(lines, col, ',');
        getline(lines, col, ',');

        stringstream ss;
        ss << col;
        ss >> x;
  

        getline(lines, col, ',');
        stringstream ss2;
        ss2 << col;
        ss2 >> y;


        if (limite_superior_x < x)
            limite_superior_x = x;

        if (x < limite_inferior_x)
            limite_inferior_x = x;
        
        if (limite_superior_y < y)
            limite_superior_y = y;

        if (y < limite_inferior_y)
            limite_inferior_y = y;


        //cout << y << '\n';
        puntos[i] = new CPunto(x, y);
    }
}

void print_puntos() {
    for (int i = 0; i < cantidad_registros; i++) {
        cout << i << endl;
        puntos[i]->print();
    }
}

void print_limites() {
    cout << "limite superior x : " << limite_superior_x << endl;
    cout << "limite inferior x : " << limite_inferior_x << endl;

    cout << "limite superior y : " << limite_superior_y << endl;
    cout << "limite inferior y : " <<limite_inferior_y << endl;
}

void print_centroide() {
    for (int i = 0; i < centroides.size(); i++) {
        cout << i << endl;
        centroides[i]->print_c();
    }
}

void llenar_centroides(int k) {
    centroides.resize(k);
    srand(time(NULL));

    long double x;
    long double y;

    //print_limites();

    CPunto* punto_temp;
    int numero_random;
    int indice;
    for (int i = 0; i < k; i++) {
        numero_random = rand();
        indice = numero_random % puntos.size();
        //cout << indice <<endl;
        punto_temp = puntos[ indice ];

        x = punto_temp->x;
        y = punto_temp->y;
        centroides[i] = new CPunto(x, y);
    }
}


//Funcion para mover los puntos ya que estan muy cercanos
vector<float> escalas(2, 0);
void determinar(int i) {
    long escala = 5;

    if (i == 0)
    {
        escalas[0] = 0;
        escalas[1] = escala;
    }

    else if (i == 1)
    {
        escalas[0] = -escala;
        escalas[1] = 0;
    }

    else if (i == 2)
    {
        escalas[0] = 0;
        escalas[1] = -escala;
    }

    else if (i == 3)
    {

        escalas[0] = escala;
        escalas[1] = 0;
    }

    else if (i == 4)
    {
        escalas[0] = escala / 2;
        escalas[1] = 0;
    }

    else if (i == 5)
    {
        escalas[0] = -escala;
        escalas[1] = 0;
    }

    else if (i == 6)
    {
        escalas[0] = -escala;
        escalas[1] = -escala;
    }
}



//-----------------Algoritmo de K-means---------------
vector<int> totalPointsCluster;

void actualizar_centroides(int k) {
    
    totalPointsCluster.clear();
    totalPointsCluster.resize(k);

    vector<long double> total_cluster_x(k,0);
    vector<long double> total_cluster_y(k,0);
    vector<int> cantidad_point_cluster(k, 0);

    int indice;
    for (int i = 0; i < puntos.size(); i++)
    {
        indice = puntos[i]->cluster_pertenece;
        total_cluster_x[indice] += puntos[i]->x;
        total_cluster_y[indice] += puntos[i]->y;
        cantidad_point_cluster[indice] += 1;
    }

  

    //Sacar el promedio
    long double promedio_x;
    long double promedio_y;
    for (int i = 0; i < k; i++)
    {
        //Si es 0 entonces el centroide se mantiene
        if (cantidad_point_cluster[i] != 0) {
            promedio_x = total_cluster_x[i] / cantidad_point_cluster[i];
            promedio_y = total_cluster_y[i] / cantidad_point_cluster[i];
            centroides[i]->update_coordenadas(promedio_x, promedio_y);
        }
    }
}


bool exist_changes() {
    bool ans = false;

    for (int i = 0; i < puntos.size(); i++) {

        long double distancia = puntos[i]->distance_euclidiana(*(centroides[0]));
        long double temp;
        int indice = 0;

        for (int j = 1; j < centroides.size(); j++)
        {
            temp = puntos[i]->distance_euclidiana(*(centroides[j]));

            if (temp < distancia) {
                distancia = temp;
                indice = j;
            }
        }

        //Si el cluster al que pertenece es diferente
        if (indice != puntos[i]->cluster_pertenece)
            ans = true;
        
        puntos[i]->cluster_pertenece = indice;
    }

    return ans;

}

void k_means(int k) {
    
    llenar_centroides(k);
    //print_centroide();

    //Designar Cluster a cada punto
    for (int i = 0; i < puntos.size(); i++) {

        
        long double distancia = puntos[i]->distance_euclidiana(*(centroides[0]));
        long double temp;
        int indice = 0;
     
        for (int j = 1; j < centroides.size(); j++)
        {
            temp = puntos[i]->distance_euclidiana(*(centroides[j]));

            if (temp < distancia) {
                distancia = temp;
                indice = j;
            }
        }
        puntos[i]->cluster_pertenece = indice;
    }

    actualizar_centroides(k);
    


    while (exist_changes()) {
        actualizar_centroides(k);
    }
}
//-----------------------------


//-----------Dibujo con SFML------------
void Dibujar() {
    //Ancho y largo
    int ancho = (limite_superior_x - limite_inferior_x) * 5;
    int largo = (limite_superior_y - limite_inferior_y) * 5;

    sf::RenderWindow renderWindow(sf::VideoMode(ancho, largo), "Demo Game");
    sf::Event event;
    sf::View view1;

    view1.setCenter(limite_inferior_x / 2, limite_superior_y / 2);


    //----------Colores-------
    sf::Color colores[7] = {
        sf::Color::Blue,sf::Color::Red,
        sf::Color::Green, sf::Color::Black,
        sf::Color::Magenta,sf::Color::Cyan,
        sf::Color::Yellow
    };


    vector<sf::CircleShape> circulos(puntos.size());

    for (int i = 0; i < puntos.size(); i++) {
        circulos[i].setRadius(0.3 * 5);
        determinar(i);
        long add_pos_x = escalas[0];
        long add_pos_y = escalas[1];
        circulos[i].setPosition(puntos[i]->x + add_pos_x, puntos[i]->y + add_pos_y);

        circulos[i].setFillColor(colores[puntos[i]->cluster_pertenece]);
    }

    while (renderWindow.isOpen()) {
        renderWindow.setView(view1);


        while (renderWindow.pollEvent(event)) {
            if (event.type == sf::Event::EventType::Closed)
                renderWindow.close();
            if (event.type == sf::Event::MouseWheelMoved)
                view1.zoom(1.f + event.mouseWheel.delta * 0.05f);

        }

        renderWindow.clear(sf::Color::White);

        for (int i = 0; i < circulos.size(); i++)
            renderWindow.draw(circulos[i]);



        renderWindow.display();
    }
}




int main()
{
    readcsv2();
    print_puntos();
    k_means(7);
    print_limites();
    print_centroide();
    Dibujar();
    

    return 0;
}

