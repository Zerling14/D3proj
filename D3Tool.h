#ifndef D3TOOL_H
#define D3TOOL_H

typedef struct {
	float x;
	float y;
	float z;
} Vector3;

typedef struct {
	Vector3 *list;
	int size;
} EnemyPosList;

int world_to_screen(float* from, float* to);
int get_enemy_pos_list(EnemyPosList **list);
void get_player_cord(float *x, float *y, float *z);

#endif