#include "TagPool.hpp"

#include <cstdint>

#include "GameExceptions.hpp"

TagPool::TagPool() {
   nextTag = {byte(0), byte(0)};
}

array<byte, 2> TagPool::allocateTag() {
   array<byte, 2> tag;

   if (!availableTags.empty()) {
      tag = availableTags.front();
      availableTags.pop();
   }
   else {
      if (nextTag[1] == byte(255)) {
         if (nextTag[0] == byte(255)) {
            throw NoAvailableTagsException();
         }
         nextTag[0] = static_cast<byte>(static_cast<int>(nextTag[0]) + 1);
         nextTag[1] = byte(0);
      }
      else {
         nextTag[1] = static_cast<byte>(static_cast<int>(nextTag[1]) + 1);
      }
      tag = nextTag;
   }

   return tag;
}

void TagPool::deallocateTag(const array<byte, 2> &tag) {
   availableTags.push(tag);
}

TagPool::~TagPool() {
}
