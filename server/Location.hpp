#ifndef LOCATION_HPP
#define LOCATION_HPP

enum class MoveType : uint8_t {
   UP = 0x00,
   DOWN = 0x01,
   RIGHT = 0x02,
   LEFT = 0x03
};

/**
 * @brief The Location class represents a point in a 2D coordinate system.
*/
class Location
{

private:
   /**
    * @brief The X-coordinate of the location.
   */
   int x;

   /**
    * @brief The Y-coordinate of the location.
   */
   int y;

   /**
    * @brief The maximum X-coordinate value for the location.
   */
   int maxX;

   /**
    * @brief The maximum Y-coordinate value for the location.
   */
   int maxY;

public:
   /**
    * @brief Default constructor for the Location class.
   */
   Location(/* args */);

   /**
    * @brief Moves the location upwards.
   */
   void goUp();

   /**
    * @brief Moves the location downwards.
   */
   void goDown();

   /**
    * @brief Moves the location to the right.
   */
   void goRight();

   /**
    * @brief Moves the location to the left.
   */
   void goLeft();

   /**
    * @brief Gets the Y-coordinate of the location.
    * 
    * @return The Y-coordinate.
   */
   int getY();

   /**
    * @brief Gets the X-coordinate of the location.
    * 
    * @return The X-coordinate.
   */
   int getX();

   /**
    * @brief Destructor for the Location class.
   */
   ~Location();
};

#endif // LOCATION_HPP
