#include <ultra64.h>
#include <actor_types.h>

/**
 * @brief Updates the paddle boat actor.
 * Actor Used in DK's Jungle Parkway.
 * Rotate only the paddle wheel, for Position and Rotation of the boat it's in vehicle.
 *
 * @Param boat
 */
void ShipStrategy(struct PaddleWheelBoat* boat) {
    boat->WheelRot += DEGREES(5);
}
