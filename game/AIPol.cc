#include "Player.hh"
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

    /**
     * DFS ALG from EDA code pdf
     * @param G
     * @return The list of Deep First Search from the vertex 0
     */
    list<int> dfs_ite (const graph& G) {
        int n = G.size();
        list<int> L;
        stack<int> S;
        vector<boolean> vis(n, false);
        for (int u = 0; u < n; ++u) {
            S.push(u);
            while (not S.empty()) {
                int v = S.top();  S.pop();
                if (not vis[v]) {
                    vis[v] = true;
                    L.push_back(v);
                    for (int w : G[v]) {
                        S.push(w);
                    }
                }
            }
        }
        return L;
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
    void movement_with_turbo(const vector<int>& empty, Movement& movement) {
        cerr << "TURBO MOVEMENT" << endl;
        movement.use_bonus = true;
    }
    void movement_with_ghost(const vector<int>& neighbours, Movement& movement) {
        cerr << "GHOST MOVEMENT" << endl;
        movement.use_bonus = true;
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

    bool in_positions(int vertex_id) {
        int i = 0;
        bool found = false;
        while(i < bike_positions.size() and not found) {
            if(bike_positions[i] == vertex_id) found = true;
            ++i;
        }
        return found;
    }
    /**
     * Attributes for your player can be defined here.
     */
    vector<int> bike_positions;

   /**
    * to_bonus function, try to go to bonus if is neigh
    * @param bike_id
    */
    void to_bonus(const Bike& my_bike) {
       cerr << "to_bonus function" << endl;
       Movement movement(my_bike.id);
       int actual_vertex_bike_id = my_bike.vertex;
       vector<int> neighbours = vertex(actual_vertex_bike_id).neighbours;
       vector<int> empty_neighbours;
       empty_neighbours_funtion(neighbours,empty_neighbours);
       //ESTEM DESPRES DE DEIXAR ANAR EL BONUS
        if(round() >= bonus_round()) {
            //INTENTEM ANAR AL BONUS SI ÉS POSSIBLE
            int possible = vertex_bonus_neighbour(actual_vertex_bike_id);
            if(possible != -1) {
                movement.next_vertex = possible;
                movement.use_bonus = false;
            }
            else {
                if(my_bike.bonus == Turbo) movement_with_turbo(empty_neighbours,movement);
                else if(my_bike.bonus == Ghost) movement_with_ghost(neighbours,movement);

                    if(!empty_neighbours.empty()) {
                        int i = 0;
                        bool found = false;
                        while(i < empty_neighbours.size() and not found) {
                            if(next_next(empty_neighbours[i]) and vertex(empty_neighbours[i]).wall == -1 and not in_positions(empty_neighbours[i])) found = true;
                            else ++i;
                        }
                        movement.next_vertex = empty_neighbours[i];
                    }
                    else movement.next_vertex = neighbours[0];
                movement.use_bonus = false;
            }
        }
        else {
            if(!empty_neighbours.empty()) {
                int i = 0;
                bool found = false;
                while(i < empty_neighbours.size() and not found) {
                    if(next_next(empty_neighbours[i]) and vertex(empty_neighbours[i]).wall == -1 and not in_positions(empty_neighbours[i])) found = true;
                    else ++i;
                }
                movement.next_vertex = empty_neighbours[i];
            }
            else movement.next_vertex = neighbours[0];
            movement.use_bonus = false;
        }
       bike_positions.push_back(movement.next_vertex);
       command(movement);
    }

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
            to_bonus(my_bike);
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



