#include "TagPool.hpp"

#include <cstdint>

TagPool::TagPool() {
   for (uint8_t firstByte = 0; firstByte <= 255; ++firstByte) {
      for (uint8_t secondByte = 0; secondByte <= 255; ++secondByte) {
         array<byte, 2> tag = {(byte)firstByte, (byte)secondByte};
         availableTags.insert(tag);
      }
   }
}

array<byte, 2> TagPool::allocateTag() {
   auto it = availableTags.begin();
   array<byte, 2> allocatedTag = *it;
   availableTags.erase(it);
   return allocatedTag;
}

void TagPool::deallocateTag(const array<byte, 2> &tag) {
   availableTags.insert(tag);
}

TagPool::~TagPool() {
}
