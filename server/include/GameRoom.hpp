#ifndef GAME_ROOM_HPP
#define GAME_ROOM_HPP

#include <string>
#include <vector>

using namespace std;

// Forward declaration to avoid circular dependencies
class GameUser;

/**
 * @brief Represents a game room that can contain multiple users.
*/
class GameRoom {

private:
   /**
    * @brief String that indicates room's name.
   */
   string name;

   /**
    * @brief Vector to store the users joined in the room.
   */
   vector<GameUser*> joinedUsers;

   /**
    * @brief The player that others have to chase.
   */
   GameUser* targetPlayer;

   /**
    * @brief Indicates if there is an ongoing game in the room.
   */
   bool isPlaying;

   /**
    * Updates isPlaying based on number of users in the room.
   */
   void updatePlayingStatus();

   /**
    * @brief Starts the game in the room.
   */
   void startGame();

   /**
    * @brief Stops the ongoing game in the room.
   */
   void stopGame();

public:
   /**
    * @brief Default constructor for the GameRoom class.
   */
   GameRoom(string name);

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
    * @brief Gets the name assosiated to the room.
    * 
    * @return The name of the room.
   */
   string getName();

   /**
    * @brief Gets all users joined the room.
    * 
    * @return Vector of joined users.
   */
   vector<GameUser*> getAllUsers();

   /**
    * @brief Gets room's target player.
    * 
    * @return The target player.
   */
   GameUser* getTargetPlayer();

   /**
    * @brief Gets the playing status of the game room.
    * 
    * @return True if the game is in progress, false otherwise.
   */
   bool getIsPlaying();

   /**
    * @brief Sets new player as target.
   */
   void restartGame();

   /**
    * @brief Destructor for the GameRoom class.
   */
   ~GameRoom();
};

#endif // GAME_ROOM_HPP
