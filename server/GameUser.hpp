#ifndef GAME_USER_HPP
#define GAME_USER_HPP

#include <string>

using namespace std;

// Forward declaration to avoid circular dependencies
class GameRoom;

/**
 * @brief Represents a user in the game with associated properties.
*/
class GameUser {

private:
   /**
    * @brief The username of the game user.
   */
   string userName;

   /**
    * @brief The X-coordinate of the game user.
   */
   int x;

   /**
    * @brief The Y-coordinate of the game user.
   */
   int y;

   /**
    * @brief The number of collected points by the game user.
   */
   int collectedPoints;

   /**
    * @brief Pointer to the GameRoom object that the user has joined.
   */
   GameRoom *joinedRoom;

public:
   /**
    * @brief Constructor for the GameUser class.
    *
    * @param userName The username of the game user.
   */
   GameUser(string userName);

   /**
    * @brief Sets the room for the game user has joined.
    *
    * @param joinedRoom Pointer to the GameRoom object user has joined.
   */
   void setRoom(GameRoom* joinedRoom);

   /**
    * @brief Gets the room that the game user has joined.
    *
    * @return Pointer to the joined GameRoom object.
   */
   GameRoom *getRoom();

   /**
    * @brief Destructor for the GameUser class.
   */
   ~GameUser();
};

#endif // GAME_USER_HPP
