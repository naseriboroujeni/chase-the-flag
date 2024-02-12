#include <cstdint>

/**
 * @brief Enumerates the different types of messages exchanged between the server and clients.
*/
enum class MessageType : uint8_t {
   SystemMessage = 0x00, /**< Represents a system message exchanged between the server and clients. */
   PlayerUpdate = 0x01   /**< Represents a player update message exchanged between the server and clients. */
};

/**
 * @brief Enumerates the different types of system messages exchanged between the server and clients.
 * These message types does not require user's specific tag.
*/
enum class SystemMessageType : uint8_t {
   AssignTag = 0x00, /**< Send user's assigned tag on openning th socket.*/
   CreateRoom = 0x01, /**< Request for creating a new game room. */
   ListRooms = 0x02,  /**< Request for creating a new game room. */
};

/**
 * @brief Enumerates the different types of player update messages exchanged between the server and clients.
 * These message types require user's specific tag.
*/
enum class PlayerUpdateType : uint8_t {
   Move = 0x00,        /**< Request for updating the player's movement in the game. */
   SendMessage = 0x01, /**< Request for sending a chat message. */
   JoinRoom = 0x02,    /**< Request for joining a specific game room. */
   LeaveRoom = 0x03,   /**< Request for leaving the current game room. */
};

enum class MoveType : uint8_t {
   UP = 0x00,
   DOWN = 0x01,
   RIGHT = 0x02,
   LEFT = 0x03
};
