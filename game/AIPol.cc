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

     void recursive_path (int id_vertex, list<int>& resultaux, vector<int>& visited) {
        Vertex vertex1 = vertex(id_vertex);
        vector<int> neighbours = vertex1.neighbours;
        vector<int> empty_neighbours;
        empty_neighbours_funtion(neighbours,empty_neighbours);
        if(empty_neighbours.size() != 0) {
            //TODO: Make a better solution than not the first one
            int i = 0;
            bool found = false; //FOUND SOME VERTEX THAT IS NOT VISITED
            while(i < empty_neighbours.size() and not found) {
                if(not visited_function(empty_neighbours[i],visited)) found = true;
                else ++i;
            }
            if(found) {
                resultaux.push_back(empty_neighbours[i]);
                visited.push_back(empty_neighbours[i]);
                recursive_path(empty_neighbours[i], resultaux,visited);
            }

        }
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

    int next_longest_vertex (int id_vertex) {
        int result;
        Vertex vertex1 = vertex(id_vertex);
        vector<int> neighbours = vertex1.neighbours;
        vector<int> empty_neighbours;
        empty_neighbours_funtion(neighbours,empty_neighbours);
        list<int> resultprinc;
        resultprinc.clear();
        list<int> resultaux;
        resultaux.clear();
        vector<int> visited;
        visited.clear();
        for(int i = 0; i < empty_neighbours.size(); ++i) {
            recursive_path(empty_neighbours[i],resultaux,visited);
            if(resultaux.size() > resultprinc.size()) resultprinc = resultaux;
            resultaux.clear();
            visited.clear();
        }
        list<int>::iterator it = resultprinc.begin();
        while(not in_empty(empty_neighbours,*it) and it != resultprinc.end()) ++it;
        if(it == resultprinc.end() and !empty_neighbours.empty()) result = empty_neighbours[0];
        else result = *it;
        return result;
    }

    /**
     * FUNCTIONS AND ALGORITHMS FOR THE PRINCIPAL FUNCTIONS
     */

    /**
     * vertex_bonus_neighbour function
     * @param actual_vert_id
     * @return vertex id if bonus is a neighbour, -1 otherwise
     */
    int vertex_bonus_neighbour(int actual_vert_id) {
        vector<int> bv = bonus_vertices();
        vector<int> actual_neighbours = vertex(actual_vert_id).neighbours;
        for(int i = 0; i < bv.size(); ++i) {
            for(int j = 0; j < actual_neighbours.size(); ++j) {
                if(bv[i] == actual_neighbours[j]) return bv[i];
            }
         }
        return -1;
    }
    void empty_neighbours_funtion(const vector<int>& neighbours, vector<int>& empty_neighbours) {
        for (int i = 0; i < (int) neighbours.size(); i++) {
            int id = neighbours[i];
            if (vertex(id).wall == -1) {
                empty_neighbours.push_back(id);
            }
        }
    }

    void movement_general(const Bike& my_bike) {
        cerr << "MOVEMENT GENERAL" << endl;
        Movement movement1(my_bike.id);
        int actual_vertex = my_bike.vertex;
        int next_vertex1 = next_longest_vertex(actual_vertex);
        movement1.next_vertex = next_vertex1;
        if(my_bike.bonus != None) movement1.use_bonus = true;
        bike_positions.push_back(next_vertex1);
        command(movement1);
    }
    /**
     * Function to prove if the next vertex has empty neighbours
     * @param vertex1
     * @return True if has neighbours, False if not
     */
    bool next_next(int vertex_id) {
        Vertex vertex1 = vertex(vertex_id);
        vector<int> neighbours = vertex1.neighbours;
        vector<int> empty_neighbours;
        empty_neighbours_funtion(neighbours,empty_neighbours);
        return !empty_neighbours.empty();
    }

    /**
     * Attributes for your player can be defined here.
     */
    vector<int> bike_positions;

    /**
     * Play method.
     *
     * This method will be invoked once per each round.
     * You have to read the board and place your actions
     * for this round.
     */
    void play () {

        vector<int> my_bikes = bikes(me());
        for (int i = 0; i < (int)my_bikes.size(); ++i) {

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
            //to_bonus(my_bike);
            //REPLACING ALL CONTINUE!

            /*
            // Find all empty neighbours
            vector<int> neighbours = vertex(my_bike.vertex).neighbours;
            vector<int> empty_neighbours;
            for (int i = 0; i < (int)neighbours.size(); i++) {
                int id = neighbours[i];
                if (vertex(id).wall == -1) {
                    empty_neighbours.push_back(id);
                }
            }
            // Create an empty movement
            Movement movement(my_bike.id);

            //RECTE
            if(!empty_neighbours.empty()) movement.next_vertex = empty_neighbours[0];
            if(my_bike.bonus != None) movement.use_bonus = true;

            // Set next_vertex to a random empty neighbour, if any, or to a random neighbour otherwise
            if (!empty_neighbours.empty()) {
                movement.next_vertex = empty_neighbours[ rand() % (int)empty_neighbours.size() ];
            } else {
                movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
            }

            // Use bonus randomly when we have one
            if (my_bike.bonus != None && rand()%5 > 3) {
                movement.use_bonus = true;
            }
            // Command the movement
            command(movement);
             */

        }

    }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);



