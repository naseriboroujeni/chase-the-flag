#ifndef GAME_ROOM_HPP
#define GAME_ROOM_HPP

#include <vector>

using namespace std;

// Forward declaration to avoid circular dependencies
class GameUser;

/**
 * @brief Represents a game room that can contain multiple users.
*/
class GameRoom
{

private:
   /**
    * @brief Vector to store the users joined in the room.
   */
   vector<GameUser*> joinedUsers;

public:
   /**
    * @brief Default constructor for the GameRoom class.
   */
   GameRoom();

   /**
    * @brief Adds a user to the game room.
    *
    * @param joinedUser Pointer to the GameUser object to be added.
   */
   void addUser(GameUser* joinedUser);

   /**
    * @brief Removes a user from the game room.
    *
    * @param joinedUser Pointer to the GameUser object to be removed.
   */
   void removeUser(GameUser* joinedUser);

   /**
    * @brief Destructor for the GameRoom class.
   */
   ~GameRoom();
};

#endif // GAME_ROOM_HPP
