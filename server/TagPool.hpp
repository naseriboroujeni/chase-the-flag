#ifndef TAG_POOL_HPP
#define TAG_POOL_HPP

#include <array>
#include <queue>

using namespace std;

const int TAG_SIZE = 2;

/**
 * @brief A class representing a pool of unique 2-byte tags that can be allocated and deallocated.
*/
class TagPool {

private:
   /**
    * @brief Set of available 2-byte tags in the pool.
   */
   queue<array<byte, TAG_SIZE>> availableTags;
   array<byte, TAG_SIZE> nextTag;

public:
   /**
    * @brief Constructor for the TagPool class.
   */
   TagPool();

   /**
    * @brief Allocates a unique 2-byte tag from the pool.
    *
    * @return The allocated tag.
   */
   array<byte, 2> allocateTag();

   /**
    * @brief Deallocates a previously allocated tag and adds it back to the pool.
    *
    * @param tag The tag to deallocate.
   */
   void deallocateTag(const array<byte, 2> &tag);

   /**
    * @brief Destructor for the TagPool class.
   */
   ~TagPool();
};

#endif // TAG_POOL_HPP
