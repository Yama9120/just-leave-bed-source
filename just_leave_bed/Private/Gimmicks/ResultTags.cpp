#include "Gimmicks/ResultTags.h"

namespace ResultTags
{
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Equip, "Result.Equip", "Equipped the item before failing");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(FailHit, "Result.Fail.Hit", "Hit the player");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(FailMiss, "Result.Fail.Miss", "Missed: hit the floor or a wall, or expired before being equipped");
} // namespace ResultTags
