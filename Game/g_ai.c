/*
Variables:
	dodge %
	block %
	player average distance from boss
	attack range of currently equipped weapon
	AI decision making latency
	distance_threshold

Determing what the players strategy is:
	Is the player being more aggressive, attacking more often and dodging only to set up a flurry?
	Is the player a melee or ranged based?
*/

typedef enum {AGGRESSIVE, DEFENSIVE, RANGED, MELEE} strategy;

typedef struct
{

	float block_percentage;
	float dodge_percentage;
	float avg_dist;
	stategy player_strategy[2];

} PlayerData;

typedef struct
{

	stategy boss_strategy[2];

} AI;