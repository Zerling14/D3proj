#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "D3Tool.h"

#define OFFSET_X		0xD0
#define OFFSET_Y		0xD4
#define OFFSET_Z		0xD8
#define OFFSET_ID		0
#define OFFSET_NAME 	0x4
#define OFFSET_ISENEMY	0x94
#define OFFSET_ININVENT	0xBC
#define OFFSET_ISITEM	0x180

HANDLE hProcess;
float view_matrix[4][4];
float proj_matrix[4][4];

HWND hWnd;
float my_coords[3];
HDC hDC;

int init_d3tool()
{
	get_process_handle();
	if (hWnd == NULL) {
		return 1;
	}
	return 0;
}

int get_enemy_pos_list(EnemyPosList **list)
{
	init_d3tool();
	int max = get_num_elemets_in_entity();
	int j = 0;
	*list = malloc(sizeof(EnemyPosList));
	(**list).list = calloc(max, sizeof(Vector3));
	for (int i = 0; i <= max; i++) {
		char buf[128] = {};
		if (!get_name_by_num(i, buf, 128)) {
			continue;
		}
		if (!get_unit_is_enemy_by_num(i)) {
			continue;
		}
		(**list).list[j] = get_cord_by_num_in_vec(i);
		j++;
	}
	(*list)->size = j;
	return 0;
}

void print_entity_list()
{
	int max = get_num_elemets_in_entity();
	if (max == 0xFFFF) {
		printf("entity list is FFFF\n");
		return;
	}
	int j = 0;
	for (int i = 0; i <= max; i++) {
		char buf[128] = {};
		if (!get_name_by_num(i, buf, 128)) {
			continue;
		}
		float x, y, z;
		get_cord_by_num(get_num_local_player(), &x, &y, &z);
		Vector3 hero = {x, y, z};
		get_cord_by_num(i, &x, &y, &z);
		Vector3 from = {x, y, z};
		printf("Dist:%7.2f Cord:%7.2f %7.2f%7.2f Name %X element: %s\n", get_dist_by_vec(hero, from), x, y, z, i, buf);
		j++;
	}
	printf("Total: %d\n", j);
}

void up_item()
{
	int max = get_num_elemets_in_entity();
	int j = 0;
	int closest_num = -1;
	for (int i = 0; i <= max; i++) {
		char buf[128] = {};
		if (get_name_by_num(i, buf, 128)) {
			if (!get_item_is_item(i) || get_item_in_inventory_by_num(i) || strstr(buf, "HealthGlobe") || 0) {
				continue;
			}
			if (closest_num == -1) {
				closest_num = i;
			}

			float x, y, z;
			get_cord_by_num(get_num_local_player(), &x, &y, &z);
			Vector3 hero = {x, y, z};

			get_cord_by_num(closest_num, &x, &y, &z);
			Vector3 closest_cord = {x, y, z};

			get_cord_by_num(i, &x, &y, &z);
			Vector3 from = {x, y, z};

			if (get_dist_by_vec(hero, from) <  get_dist_by_vec(hero, closest_cord)) {
				closest_num = i;
			}
			printf("Dist:%7.2f Cord:%7.2f %7.2f%7.2f Name %X element: %s\n", get_dist_by_vec(hero, from), x, y, z, i, buf);
			j++;
		}
	}
	while (1) {
		char tmpbuf[64];
		if ((closest_num == -1) || (get_item_in_inventory_by_num(closest_num)) || (!get_name_by_num(closest_num, tmpbuf, 64)) || (!get_item_is_item(closest_num))) {
			return;
		}
		printf("closest_num: %d name %s\n", closest_num, tmpbuf);
		float x, y, z;
		get_cord_by_num(closest_num, &x, &y, &z);
		float from[3] = {x, y, z};
		float to[3];
		world_to_screen(from, to);
		board_normalizing(&to[0], &to[1], 1400, 150, 250, 700);
		SendMessage(hWnd, WM_KEYDOWN, VK_MENU, 0);
		SendMessage(hWnd, WM_KEYUP, VK_MENU, 0);
		SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM((int) to[0], (int) to[1]));
		Sleep(100);
		SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM((int) to[0], (int) to[1]));
		Sleep(200);
		if (GetAsyncKeyState(0x43)) {
			break;
		}
	}
}

void board_normalizing(float *x, float *y, float top_cap_x, float top_cap_y, float bottom_cap_x, float bottom_cap_y)
{
	RECT wndCord;
	GetWindowRect(hWnd, &wndCord);
	float center_x, center_y;
	center_x = (wndCord.right - wndCord.left) / 2;
	center_y = (wndCord.bottom - wndCord.top) / 2;
	if (*x > top_cap_x) {
		float tmp_x, tmp_y;
		tmp_y = (((top_cap_x - *x) * (center_y - *y)) / (center_x - *x)) + *y;
		tmp_x = top_cap_x;
		*x = tmp_x;
		*y = tmp_y;
	}
	if (*x < bottom_cap_x) {
		float tmp_x, tmp_y;
		tmp_y = (((bottom_cap_x - *x) * (center_y - *y)) / (center_x - *x)) + *y;
		tmp_x = bottom_cap_x;
		*x = tmp_x;
		*y = tmp_y;
	}
	if (*y < top_cap_y) {
		float tmp_x, tmp_y;
		tmp_x = (((top_cap_y - *y) * (center_x - *x)) / (center_y - *y)) + *x;
		tmp_y = top_cap_y;
		*x = tmp_x;
		*y = tmp_y;
	}
	if (*y > bottom_cap_y) {
		float tmp_x, tmp_y;
		tmp_x = (((bottom_cap_y - *y) * (center_x - *x)) / (center_y - *y)) + *x;
		tmp_y = bottom_cap_y;
		*x = tmp_x;
		*y = tmp_y;
	}
}

Vector3 get_cord_by_num_in_vec(int num)
{
	float x, y, z;
	get_cord_by_num(num, &x, &y, &z);
	Vector3 vec = {x, y, z};
	return vec;
}

float get_dist_by_vec(const Vector3 vec1, const Vector3 vec2)
{
	Vector3 tmp_vec = {vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z};
	return sqrt(pow(tmp_vec.x, 2) + pow(tmp_vec.y, 2) + pow(tmp_vec.z, 2)); 
}

void move_cursor_to_vec3(Vector3 vec)
{
	float from[3] = {vec.x, vec.y, vec.z};
	float to[2];
	world_to_screen(from, to);
	SetCursorPos((int)to[0], (int)to[1]);
}

void move_cursor_to_unit_by_num(int num)
{
	float x, y, z;
	get_cord_by_num(num, &x, &y, &z);
	Vector3 vec = {x, y, z};
	float from[3] = {vec.x, vec.y, vec.z};
	float to[2];
	world_to_screen(from, to);
	SetCursorPos((int)to[0], (int)to[1]);
}

int world_to_screen(float* from, float* to)
{
	RECT rect;
	
	GetWindowRect(FindWindow(0, "Diablo III"), &rect);
	float point[1][4] = {{from[0], from[1],from[2],1}};
	get_view_matrix();
	float *point_from_vm = NULL;
	mul_matrix(point, 1, 4, view_matrix, 4, 4, (PVOID)&point_from_vm);
	
	if (point_from_vm == NULL) {
		printf("w2c vm error\n");
		return 0;
	}
	float *point_from_pm = NULL;
	point_from_vm[0] = point_from_vm[0] * (1 / point_from_vm[2]);
	point_from_vm[1] = point_from_vm[1] * (1 / point_from_vm[2]);
	
	mul_matrix(point_from_vm, 1, 4, proj_matrix, 4, 4, (PVOID)&point_from_pm);
	
	if (point_from_pm == NULL) {
		printf("w2c pm error\n");
		return 0;
	}
	to[0] = point_from_pm[0] * -1;
	to[1] = point_from_pm[1] * -1;
	int width = (int)(rect.right - rect.left);
	int height = (int)(rect.bottom - rect.top);
	float x = width/2;
	float y = height/2;
	x += 0.5 * to[0] * width + 0.5;
    y -= 0.5 * to[1] * height + 0.5;
	to[0] = x + rect.left;
	to[1] = y + rect.top;
	free(point_from_vm);
	free(point_from_pm);
	
	return 1;
}

void get_process_handle()
{
	DWORD pid = 0;
	hWnd = FindWindow(0, "Diablo III");
	if (hWnd == 0) {
		printf("FindWindow failed, %ld\n", GetLastError());
		return;
	}
	GetWindowThreadProcessId(hWnd, &pid);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (hProcess == 0) {
		printf("OpenProcess failed, %ld\n", GetLastError());
		return;
	}
	
	
}

void get_view_matrix()
{
	PDWORD BaseAddress = (PDWORD)0x01C12E98;
	DWORD tmp;
	DWORD tmp1;
	read_bytes((PCVOID)(BaseAddress), sizeof(tmp), &tmp);
	read_bytes((PCVOID)(tmp + 0xA00), sizeof(tmp1), &tmp1);
	read_bytes((PCVOID)(tmp1+ 0x150), 64, view_matrix);
	read_bytes((PCVOID)(tmp1+ 0x190), 64, proj_matrix);
}

void get_player_cord(float *x, float *y, float *z)
{
	get_cord_by_num(get_num_local_player(), x, y, z);
}

DWORD get_num_local_player()
{
	DWORD baseNumPlayer = 0x205c0f0; //- Base entity list(not entity)
	DWORD tmp;
	DWORD tmp1;
	DWORD result = 0;
	read_bytes((PCVOID)(baseNumPlayer),	sizeof(tmp), &tmp);
	read_bytes((PCVOID)(tmp + 0x8E4), sizeof(tmp), &tmp1);
	read_bytes((PCVOID)(tmp1 + 0x3c), 1, &result);
	return result;
}

void print_1x4_matrix(void *matrix)
{
	for (int i = 0; i < 4; i++) {
		printf("%10.4f ", *(float*)(matrix+4*i));
	}
	printf("\n");
}

void print_4x4_matrix(void *matrix)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%10.4f ", *(float*)(matrix+4*j+4*4*i));
		}
		printf("\n");
	}
}

void mul_matrix(void *matrix1, int x1, int y1, 
				 void *matrix2, int x2, int y2, 
				 void **matrix3)
{
	if (y1 != x2) {
		printf("mul_matrix: incorrect matrix\n");
		return;
	}
	*matrix3 = calloc(x1 * y2, sizeof(float));
	
	for (int i = 0; i < x1; i++) {
		for (int j = 0; j < y2; j++) {
			float sum = 0;
			for (int k = 0; k < y1; k++) {
				sum += *((float *) matrix1 + 4 * i + k) * *((float *) matrix2 + 4 * k + j);
			}
			*((float *) *matrix3 + 4 * i + j) = sum;
		}
	}
}

int get_num_elemets_in_entity()
{
	DWORD baseEntityList = 0xFFFDDEC0;
	DWORD tmp;
	DWORD tmp1;

	read_bytes((PCVOID)(baseEntityList),			sizeof(tmp), &tmp);
	read_bytes((PCVOID)(tmp),						sizeof(tmp), &tmp1);
	read_bytes((PCVOID)(tmp1), 						sizeof(tmp), &tmp);
	read_bytes((PCVOID)(tmp + 0x160), 				sizeof(tmp), &tmp1);
	read_bytes((PCVOID)(tmp1),						sizeof(tmp), &tmp);
	read_bytes((PCVOID)(tmp + 0x108), 				sizeof(tmp), &tmp1);
	return (int)tmp1;
}

int get_name_by_num(int num, char* buf, int n)
{
	//DWORD tmp = get_entity_pointer();
	DWORD tmp1;
	if (!get_unit_info_by_offset(num, OFFSET_ID , sizeof(tmp1), &tmp1)) {
		printf("Error get name by num\n");
		return 0;
	}
	if ((int) tmp1 == -1) {
		return 0;
	}
	if ((int) (tmp1 & 0xFFFF) != num) {
		return 0;
	}
	if (!get_unit_info_by_offset(num, OFFSET_NAME , n, buf)) {
		printf("Error get name by num\n");
		return 0;
	}
	if (buf[0] == 0) {
		return 0;
	}
	return 1;
}

int get_cord_by_num(int num, float *x, float *y, float *z)
{
	if (!get_unit_info_by_offset(num, OFFSET_X , sizeof(float), x)) {
		printf("Error cord by num\n");
		return 0;
	}
	if (!get_unit_info_by_offset(num, OFFSET_Y , sizeof(float), y)) {
		printf("Error cord by num\n");
		return 0;
	}
	if (!get_unit_info_by_offset(num, OFFSET_Z , sizeof(float), z)) {
		printf("Error cord by num\n");
		return 0;
	}
	return 1;
}

int get_unit_is_enemy_by_num(int num)
{
	int tmp;
	get_unit_info_by_offset(num, 0x188, sizeof(tmp), &tmp);
	int tmp1;
	get_unit_info_by_offset(num, 0x190, sizeof(tmp1), &tmp1);
	int tmp2;
	get_unit_info_by_offset(num, 0x180, sizeof(tmp2), &tmp2);
	if ((tmp == 0xA) && (tmp1 != 1) && (tmp2 > 0)) {
		return 1;
	}
	return 0;
}
int get_item_in_inventory_by_num(int num)
{
	DWORD tmp;
	get_unit_info_by_offset(num, OFFSET_ININVENT, sizeof(tmp), &tmp);
	if (tmp == 1) {
		return 1;
	} else if ((int)tmp == -1) {
		return -1;
	}
	
	return 0;
}

int get_item_is_item(int num)
{
	DWORD tmp;
	get_unit_info_by_offset(num, OFFSET_ISITEM, sizeof(tmp), &tmp);

	if (((int)tmp == -1) || (!(get_unit_info_by_offset_unsafe(num, 0x180) == 5))) {
		return 0;
	}
	return 1;
}

int get_unit_info_by_offset(int num, DWORD off , size_t size, void *data)
{
	DWORD tmp = get_entity_pointer();

	if (!read_bytes((PCVOID)(tmp + (0x2F0 * num + off)), size, data)) {
		printf("Error get unit info by offset\n");
		return 0;
	}
	return 1;
}

DWORD get_unit_info_by_offset_unsafe(int num, DWORD off)
{
	DWORD tmp = get_entity_pointer();
	DWORD data;
	if (!read_bytes((PCVOID)(tmp + (0x2F0 * num + off)), sizeof(data), &data)) {
		printf("Error get unit info by offset\n");
		return 0;
	}
	return data;
}

DWORD get_entity_pointer()
{
	DWORD baseEntityList = 0xFFFDDEC0;
	DWORD tmp;
	DWORD tmp1;

	read_bytes((PCVOID)(baseEntityList),			sizeof(tmp), &tmp);
	read_bytes((PCVOID)(tmp),						sizeof(tmp), &tmp1);
	read_bytes((PCVOID)(tmp1), 						sizeof(tmp), &tmp);
	read_bytes((PCVOID)(tmp + 0x160), 				sizeof(tmp), &tmp1);
	read_bytes((PCVOID)(tmp1),						sizeof(tmp), &tmp);
	read_bytes((PCVOID)(tmp + 0x120),				sizeof(tmp), &tmp1);
	read_bytes((PCVOID)(tmp1),						sizeof(tmp), &tmp);
	return tmp;
}

int read_bytes(PCVOID addr, int num, void *buf)
{
	SIZE_T sz = 0;
    int r = ReadProcessMemory(hProcess, addr, buf, num, &sz);
	if (r == 0 || sz == 0) {
        printf("RPM error, %lX\n", GetLastError());
		return 0;
	}
	return 1;
}