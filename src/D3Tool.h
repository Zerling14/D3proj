#ifndef D3TOOL_H
#define D3TOOL_H
#include <stdio.h>
#include <math.h>
#include <windows.h>
typedef struct {
	float x;
	float y;
	float z;
} Vector3;

typedef struct {
	Vector3 *list;
	int size;
} EnemyPosList;

int init_d3tool();
int world_to_screen(float* from, float* to);
int get_enemy_pos_list(EnemyPosList **list);
void get_player_cord(float *x, float *y, float *z);
void get_process_handle();
int read_bytes(PCVOID addr, int num, void* buf);
void get_view_matrix();
void print_4x4_matrix(void *matrix);
DWORD get_num_local_player();
void print_1x4_matrix(void *matrix);
void mul_matrix(void *matrix1, int x1, int y1, void *matrix2, int x2, int y2, void **matrix3);
int get_num_elemets_in_entity();
int get_name_by_num(int num, char* buf, int n);
int get_cord_by_num(int num, float *x, float *y, float *z);
Vector3 get_cord_by_num_in_vec(int num);
float get_dist_by_vec(const Vector3 vec1, const Vector3 vec2);
DWORD get_entity_pointer();
int get_unit_info_by_offset(int num, DWORD off , size_t size, void *data);
int get_unit_is_enemy_by_num(int num);
int get_item_in_inventory_by_num(int num);
int get_item_is_item(int num);
void move_cursor_to_vec3(Vector3 vec);
void move_cursor_to_unit_by_num(int num);
DWORD get_unit_info_by_offset_unsafe(int num, DWORD off);
void board_normalizing(float *x, float *y, float top_cap_x, float top_cap_y, float bottom_cap_x, float bottom_cap_y);
void up_item();
void print_entity_list();

#endif