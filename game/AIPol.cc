#include "Player.hh"
#include <stack>
#include <queue>
#include <cmath>
#include <iostream>
#include <list>
using namespace std;

/**
 * Write the name of your player and save this file
 * with a name matching the format "AIyourname.cc".
 */
#define PLAYER_NAME Pol


struct PLAYER_NAME : public Player {

    /**
     * Factory: returns a new instance of this class.
     * Do not modify this function.
     */
    static Player* factory () {
        return new PLAYER_NAME;
    }

    bool visited_function(int vertex_id, const vector<int>& visited) {
        int i = 0;
        bool found = false;
        while(i < visited.size() and not found) {
            if(visited[i] == vertex_id) found = true;
            ++i;
        }
        return found;
    }

    bool in_empty(const vector<int> empty_neighbours, int vertex_id) {
        int i = 0;
        bool found = false;
        while(i < empty_neighbours.size() and not found) {
            if(empty_neighbours[i] == vertex_id) found = true;
            ++i;
        }
        return found;
    }

    bool own_bike_to_enter(int vertex_to_go) {
        int i = 0;
        bool found = false;
        while(i < bike_positions.size() and not found) {
            if(bike_positions[i] == vertex_to_go) found = true;
            ++i;
        }
        return found;
    }


    /**
     * FUNCTIONS AND ALGORITHMS FOR THE PRINCIPAL FUNCTIONS
     */

    void empty_neighbours_funtion(const vector<int>& neighbours, vector<int>& empty_neighbours) {
        for (int i = 0; i < (int) neighbours.size(); i++) {
            int id = neighbours[i];
            if (vertex(id).wall == -1 or vertex(id).bike == -1) {
                empty_neighbours.push_back(id);
            }
        }
    }

    /* PRINCIPAL
     * RECURSIVE
     * FUNCTION
     * */
    void recursive_path(int vertex_id, vector<int>& vectorref, bool first) {
        cerr << "REC" << endl;
        Vertex ver = vertex(vertex_id);
        vector<int> neighbours = ver.neighbours;
        vector<int> empty_neighbours;
        empty_neighbours_funtion(neighbours,empty_neighbours);
        //CAS BASE (JA VISITAT)
        if(not first and ocupada(vertex_id)) return;
        //CAS RECURSIU
        else {
            for(int i = 0; i < empty_neighbours.size() and next_next(empty_neighbours[i]) and not ocupada(empty_neighbours[i]) and not own_bike_to_enter(empty_neighbours[i]); ++i) {
                vectorref.push_back(empty_neighbours[i]);
                recursive_path(empty_neighbours[i],vectorref,false);
            }
        }
    }



    void movement_general(const Bike& my_bike) {
        cerr << "MOVEMENT GENERAL" << endl;
        Movement movement1(my_bike.id);
        int actual_vertex = my_bike.vertex;
        vector<int> aux;
        recursive_path(actual_vertex,aux,true);
        movement1.next_vertex = aux[0];
        if(my_bike.bonus != None) movement1.use_bonus = true;
        bike_positions.push_back(movement1.next_vertex);
        command(movement1);
    }
    /**
     * Function to prove if the next vertex has neighbours with empty neighbours
     * @param vertex1
     * @return True if has neighbours, False if not
     */
    bool next_next(int vertex_id) {
        Vertex vertex1 = vertex(vertex_id);
        vector<int> neighbours = vertex1.neighbours;
        vector<int> empty_neighbours;
        empty_neighbours_funtion(neighbours,empty_neighbours);
        if(empty_neighbours.empty() or empty_neighbours.size() == 1) return false;
        else return true;
    }

    void update_map() {
        for(int i = 0; i < nb_vertices(); ++i) {
            Vertex vertex1 = vertex(i);
            if(vertex1.wall != -1 or vertex1.bike != -1) actual_situation[i] = true;
        }
    }

    bool ocupada(int vertex_id) {
        return actual_situation[vertex_id];
    }

    void escriure_ocupada() {
        for(int i = 0; i < nb_vertices(); ++i) {
            if(actual_situation[i]) cerr << i << " ";
        }
        cerr << endl;
    }

    /**
     * Attributes for your player can be defined here.
     */
    bool first = true;
    int vertices;
    vector<int> bike_positions;
    vector<bool> actual_situation;

    /**
     * Play method.
     *
     * This method will be invoked once per each round.
     * You have to read the board and place your actions
     * for this round.
     */
    void play () {
        vertices = nb_vertices();
        if(first) {
            first = false;
            for (int i = 0; i < vertices; ++i) {
                actual_situation.push_back(false);
            }
        }
        vector<int> my_bikes = bikes(me());
        update_map();
        escriure_ocupada();
        for (int i = 0; i < (int)my_bikes.size(); ++i) {
            update_map();
            const Bike& my_bike = bike(my_bikes[i]);

            // Do something only if this bike is alive
            if (!my_bike.alive) {
                continue;
            }

            //Only turbo bikes can move at odd rounds
            if (round() % 2 && my_bike.turbo_duration <= 0) {
                continue;
            }
            movement_general(my_bike);
        }

    }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);



