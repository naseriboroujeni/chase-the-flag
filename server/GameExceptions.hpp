#ifndef GAME_EXCEPTIONS_HPP
#define GAME_EXCEPTIONS_HPP

#include <stdexcept>

class NoAvailableTagsException : public std::exception {
public:
   const char* what() const noexcept override {
      return "No more available tags in the pool.";
   }
};

#endif // GAME_EXCEPTIONS_HPP
