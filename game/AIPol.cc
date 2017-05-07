#include "Player.hh"
#include <queue>
#include <cmath>

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
     * FUNCTIONS AND ALGORITHMS FOR THE PRINCIPAL FUNCTIONS
     */

    int vertex_nearest_bonus(int actual_vert_id) {

    }

    bool possible_to_bonus(int id) {
        const Bike& my_bike = bike(id);

    }


    /**
     * Attributes for your player can be defined here.
     */
    //vector<int> my_awesome_vector_of_integers;

    /**
     * Without BONUS Strategy
     * the half 1/2 of rounds can be played
     */
    //1st strategy, go to the bonus if not, other
    void to_bonus() {
        //ESTEM ABANS DE DEIXAR ANAR EL BONUS
        if(round() < bonus_round()) {
            //INTENTEM ANAR AL BONUS SI ÉS POSSIBLE
            if(possible_to_bonus()) {

            }
        }
        else if()
    }

    //2nd strategy, if not bonus, try to go to the ghost
    void to_ghost() {

    }

    //3rd strategy, if not ghost, try to no make a tail
    void no_tail() {

    }

    /**
     * With BONUS Strategy (Ghost or Bonus)
     */
    void with_bonus() {

    }

    void with_ghost() {

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
            /*if (!empty_neighbours.empty()) {
                movement.next_vertex = empty_neighbours[ rand() % (int)empty_neighbours.size() ];
            } else {
                movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
            }

            // Use bonus randomly when we have one
            if (my_bike.bonus != None && rand()%5 > 3) {
                movement.use_bonus = true;
            }
            */
            // Command the movement
            command(movement);

        }

    }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);



