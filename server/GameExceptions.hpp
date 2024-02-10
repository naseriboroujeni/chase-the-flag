#ifndef GAME_EXCEPTIONS_HPP
#define GAME_EXCEPTIONS_HPP

#include <stdexcept>

class NoAvailableTagsException : public std::exception {
public:
   const char* what() const noexcept override {
      return "No more available tags in the pool.";
   }
};

class NotInAValidRoomException : public std::exception {
public:
   const char* what() const noexcept override {
      return "Player is not in a valid room.";
   }
};

class InvalidMessageException : public std::exception {
public:
   const char* what() const noexcept override {
      return "Message sent to server is not a valid message type.";
   }
};

#endif // GAME_EXCEPTIONS_HPP
