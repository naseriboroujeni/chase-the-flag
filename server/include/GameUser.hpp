#ifndef GAME_USER_HPP
#define GAME_USER_HPP

#include <string>
#include <array>

#include <websocketpp/common/connection_hdl.hpp>

#include "Location.hpp"

using namespace std;
using ConnectionHdl = websocketpp::connection_hdl;

// Forward declaration to avoid circular dependencies
class GameRoom;

/**
 * @brief Represents a user in the game with associated properties.
*/
class GameUser {

private:
   /**
    * @brief The tag assigned to user.
   */
   array<byte, 2> userTag;

   /**
    * @brief The username of the game user.
   */
   string username;

   ConnectionHdl connection;

   /**
    * @brief Pointer to the Location object representing the user's position in the game.
   */
   Location* location;

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
    * @param username The username of the game user.
   */
   GameUser(array<byte, 2> userTag, ConnectionHdl connection);

   /**
    * @brief Gets the tag assigned to the game user.
    *
    * @return The user tag.
   */
   array<byte, 2> getTag();

   /**
    * @brief Increases player's collected points.
   */
   void addPoint();

   /**
    * @brief Gets the WebSocket connection handle for the user.
    *
    * @return Pointer to the connection handle.
   */
   ConnectionHdl getConnection();

   /**
    * @brief Sets the username for the game user.
    *
    * @param userName The username of the game user.
   */
   void setUsername(string userName);

   /**
    * @brief Gets the username of the game user.
    *
    * @return The username.
   */
   string getUsername();

   /**
    * @brief Gets the Location object representing the user's position in the game.
    *
    * @return Pointer to the Location object.
   */
   Location* getLocation();

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
