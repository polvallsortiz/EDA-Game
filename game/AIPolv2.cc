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
#define PLAYER_NAME Polv2


struct PLAYER_NAME : public Player {

    /**
     * Factory: returns a new instance of this class.
     * Do not modify this function.
     */
    static Player* factory () {
        return new PLAYER_NAME;
    }

    /**
     * FUNCTIONS AND ALGORITHMS FOR THE PRINCIPAL FUNCTIONS
     */

    bool visited_function(int vertex_id, const vector<int>& visited) {
        int i = 0;
        bool found = false;
        while(i < (int)visited.size() and not found) {
            if(visited[i] == vertex_id) found = true;
            ++i;
        }
        return found;
    }

    bool own_bike_to_enter(int vertex_to_go) {
        int i = 0;
        while(i < (int)bike_positions.size()) {
            if(bike_positions[i] == vertex_to_go) return true;
            ++i;
        }
        return false;
    }

    int vertex_bonus_neighbour(int actual_vert_id) {
        vector<int> actual_neighbours = vertex(actual_vert_id).neighbours;
        int i = 0;
        while(i < (int)actual_neighbours.size()) {
            Vertex v1 = vertex(actual_neighbours[i]);
            if(v1.bonus != None) return v1.id;
            ++i;
        }
        return -1;
    }

    bool vertex_bonus(int actual_vertex) {
        return vertex(actual_vertex).bonus != None;
    }

    void empty_neighbours_funtion(const vector<int>& neighbours, vector<int>& empty_neighbours) {
        for (int i = 0; i < (int) neighbours.size(); i++) {
            int id = neighbours[i];
            if (vertex(id).wall == -1 and vertex(id).bike == -1) {
                empty_neighbours.push_back(id);
            }
        }
    }

    bool next_next(int vertex_id) {
        vector<int> neighbours = vertex(vertex_id).neighbours;
        vector<int> empty_neighbours;
        empty_neighbours_funtion(neighbours,empty_neighbours);
        if((int)empty_neighbours.size() < 2) return false;
        else return true;
    }

    bool warning(int vertex_id) {
        Vertex vertex1 = vertex(vertex_id);
        vector<int> neighbours = vertex1.neighbours;
        int i = 0;
        while(i < (int)neighbours.size()) {
            if(vertex(neighbours[i]).bike != -1) return true;
            ++i;
        }
        return false;
    }

    //DESDE VERTEX_ID (INCLOS OSIGUI list[0] ja és neighbour
    list<int> dfs_game(int vertex_id) {
        list<int> result;
        stack<int> stack1;
        vector<int> visited;
        visited.push_back(vertex_id);   //1
        stack1.push(vertex_id);              //2
        result.push_back(vertex_id);    //3
        while(!stack1.empty()) {
            vector<int> neighbours = vertex(vertex_id).neighbours;
            vector<int> adj;
            empty_neighbours_funtion(neighbours, adj);
            int j = 0;
            while (j < (int)adj.size() and visited_function(adj[j],visited)) ++j;
            if (j != (int)adj.size()) {
                visited.push_back(adj[j]);  //1
                stack1.push(adj[j]);             //2
                result.push_back(adj[j]);   //3
                vertex_id = adj[j];         //4 -> Assign i = v
            }
            else {
                vertex_id = stack1.top();
                stack1.pop();    //TREURE ELEMENT STACK
            }
        }
        return result;
    }


    void movement_general(const Bike& my_bike) {
        Movement movement1(my_bike.id);
        vector<int> neighbours_princ = vertex(my_bike.vertex).neighbours;
        vector<int> empty_princ;
        empty_neighbours_funtion(neighbours_princ,empty_princ);
        int possible_bonus = vertex_bonus_neighbour(my_bike.vertex);
        int i = 0;
        int vertex_dfs = -1;
        int size_large = 0;
        list<int> dfs;
        bool actual_warning = false;
        while (i < (int) empty_princ.size()) {
            dfs = dfs_game(empty_princ[i]);
            if (dfs.size() > 0 and next_next(empty_princ[i]) and not own_bike_to_enter(empty_princ[i])) {
                if(size_large == 0) {       //NO TENIM PATH AIXI QUE EM DE TROBAR-LO
                    size_large = dfs.size();
                    vertex_dfs = dfs.front();
                    actual_warning = warning(empty_princ[i]);
                }
                else {                      //JA TENIM PATH AIXI QUE RELAXAT
                    if(actual_warning) {    //ESTEM SOTA AMENAÇA
                        if(not warning(empty_princ[i])) {
                            actual_warning = false;
                            size_large = dfs.size();
                            vertex_dfs = dfs.front();
                        }
                    }
                    else {          //NO ESTEM AMB AMENAÇA
                        if(size_large < (int)dfs.size() and not warning(empty_princ[i])) {  //MILLOR PATH
                            size_large = dfs.size();
                            vertex_dfs = dfs.front();
                        }
                    }
                }
            }
            ++i;
        }
        if (vertex_dfs == -1) {
            cerr << "AQUI EXCEPCIONALMENT" << endl;
            if (empty_princ.size() > 0) vertex_dfs = empty_princ[0];
            else vertex_dfs = neighbours_princ[0];
        }
        bike_positions.push_back(vertex_dfs);
        movement1.next_vertex = vertex_dfs;
        movement1.use_bonus = (my_bike.bonus != None);
        command(movement1);
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
        }

    }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);