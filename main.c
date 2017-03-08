#include <windows.h>
#include <stdio.h>
#include <math.h>

#define OFFSET_X		0xD0
#define OFFSET_Y		0xD4
#define OFFSET_Z		0xD8
#define OFFSET_ID		0
#define OFFSET_NAME 	0x4
#define OFFSET_ISENEMY	0x94
#define OFFSET_ININVENT	0xBC


typedef struct {
	float x;
	float y;
	float z;
} Vector3;

HANDLE hProcess;
float view_matrix[4][4];
float proj_matrix[4][4];
int world_to_screen(float* from, float* to);
HWND hWnd;
float my_coords[3];
HDC hDC;
void get_process_handle();
int read_bytes(PCVOID addr, int num, void* buf);
void get_view_matrix();
void print_4x4_matrix(void *matrix);
void get_player_cord(float *x, float *y, float *z);
DWORD get_num_local_player();
void print_1x4_matrix(void *matrix);
void mul_matrix(void *matrix1, int x1, int y1, void *matrix2, int x2, int y2, void **matrix3);
int get_num_elemets_in_entity();
int get_name_by_num(int num, char* buf, int n);
int get_cord_by_num(int num, float *x, float *y, float *z);
//float get_dist_by_vec(float *vec1, float *vec2);
float get_dist_by_vec(const Vector3 vec1, const Vector3 vec2);
DWORD get_entity_pointer();
int get_unit_info_by_offset(int num, DWORD off , size_t size, void *data);




int main(int argc, char** argv)
{
	get_process_handle();
	//float from[3] = {380.23, 354.14, 0.50};
	//float to[3];
	//world_to_screen(from, to);
	// [[[BaseAddress + 0x26767C]+0x598]+0x1EC]+0x9F2
	//PDWORD BaseAddress = (PDWORD)0x01C12E98;
	//PDWORD pdwAddress = ( PDWORD )*( PDWORD )(BaseAddress + 0xA0);
	//PDWORD pdwFinalAddress = ( PDWORD )*( PDWORD )( pdwAddress + 150 );
	//itemFlippy_deathsBreath_Flippy_Global
	printf("hWnd:%d\n",(int)hWnd);
	//float x1, y1, z1;
	//get_cord_by_num(0x1D9, &x1, &y1, &z1);
	//float position_player[3] = {x1, y1, z1};
	//float position_in_screen[2];
	//get_player_cord(&position_player[0], &position_player[1], &position_player[2]);
	
	//world_to_screen(position_player, position_in_screen);
	
	// ITEM NAMES
	// itemFlippy_deathsBreath_Flippy_Global DROPED DEATHBREATH
	// CraftingMaterials_Flippy_Global DROPED COMMON(WHITE) REAGENT
	
	// float x, y, z;
	// get_cord_by_num(0x52, &x, &y, &z);
	// float from[3] = {x, y, z};
	// float to[3];
	// world_to_screen(from, to);
			
	// return 0;
	
	int max = get_num_elemets_in_entity();
	int j = 0;

	for (int i = 0; i <= max; i++){
		char buf[128] = {};
		if (get_name_by_num(i, buf, 128)){

			//if (strstr(buf, "flippy") != 0)
			{
				float x, y, z;
				get_cord_by_num(get_num_local_player(), &x, &y, &z);
				Vector3 hero = {x, y, z};

				get_cord_by_num(i, &x, &y, &z);
				Vector3 from = {x, y, z};
				printf("Dist:%7.2f Cord:%7.2f%7.2f%7.2f Name %X element: %s\n", get_dist_by_vec(hero, from), x, y, z, i, buf);
				j++;
			}
		}
	}
	printf("Total:%d", j);
	return 0;

	while(1) {
		if (GetAsyncKeyState(0x43)) {
			int max = get_num_elemets_in_entity();
			int j = 0;
			int closest_num = -1;

			for (int i = 0; i <= max; i++) {
				char buf[64] = {};
				if (get_name_by_num(i, buf, 64)) {
					if (strstr(buf, "itemFlippy_deathsBreath_Flippy_Global") != 0) {
						if (closest_num == -1) {
							closest_num = i;
						}
						
						float x, y, z;
						get_cord_by_num(i, &x, &y, &z);
						Vector3 from = {x, y, z};
						
						get_cord_by_num(get_num_local_player(), &x, &y, &z);
						Vector3 hero_cord = {x, y, z};
						
						get_cord_by_num(closest_num, &x, &y, &z);
						Vector3 closest_cord = {x, y, z};
						
						if (get_dist_by_vec(hero_cord, from) <  get_dist_by_vec(hero_cord, closest_cord)) {
							closest_num = i;
						}
						//float to[3];
						//world_to_screen(from, to);
						//SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM((int) to[0], (int) to[1]));
						//Sleep(100);
						//SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM((int) to[0], (int) to[1]));
						printf("Dist: %6.2f Cord: %6.2f %6.2f %6.2f Name %X element: %s\n", get_dist_by_vec(hero_cord, from), from.x, from.y, from.z, i, buf);
					}
				}
			}
			if (closest_num == -1) {
				continue;
			}
			float x, y, z;
			get_cord_by_num(closest_num, &x, &y, &z);
			float from[3] = {x, y, z};
			float to[3];
			world_to_screen(from, to);
			SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM((int) to[0], (int) to[1]));
			Sleep(100);
			SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM((int) to[0], (int) to[1]));
			Sleep(100);
		}
	}
	
	//get_view_matrix();
	//float matrix1[1][4] = {{1,2,3,4}};
	//float matrix2[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
	//printf("\n");
	//print_1x4_matrix(matrix1);
	//printf("\n");
	//print_4x4_matrix(matrix2);
	//printf("\n");
	//int x3,y3;
	//float *matrix3;
	//mul_matrix(matrix1, 1, 4, matrix2, 4, 4, (PVOID)&matrix3, &x3, &y3);
	//print_1x4_matrix(matrix3);
	//printf("%f\n", (*matrix3);
	CloseHandle(hProcess);
    return 0;
}



// float get_dist_by_vec(float *vec1, float *vec2)
// {
	// float tmp_vec[3] = {vec1[0] - vec2[0], vec1[1] - vec2[1], vec1[2] - vec2[2]};
	// return sqrt(pow(tmp_vec[0], 2) + pow(tmp_vec[1], 2) + pow(tmp_vec[2], 2)); 
// }

float get_dist_by_vec(const Vector3 vec1, const Vector3 vec2)
{
	Vector3 tmp_vec = {vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z};
	return sqrt(pow(tmp_vec.x, 2) + pow(tmp_vec.y, 2) + pow(tmp_vec.z, 2)); 
}

int world_to_screen(float* from, float* to)
{
	RECT rect;
	
	GetWindowRect(FindWindow(0, "Diablo III"), &rect);
	float point[1][4] = {{from[0], from[1],from[2],1}};
	get_view_matrix();
	float *point_from_vm = NULL;
	mul_matrix(point, 1, 4, view_matrix, 4, 4, (PVOID)&point_from_vm);
	
	if(point_from_vm == NULL){
		printf("w2c vm error\n");
		return 0;
	}
	float *point_from_pm = NULL;
	point_from_vm[0] = point_from_vm[0] * (1 / point_from_vm[2]);
	point_from_vm[1] = point_from_vm[1] * (1 / point_from_vm[2]);
	
	mul_matrix(point_from_vm, 1, 4, proj_matrix, 4, 4, (PVOID)&point_from_pm);
	
	if(point_from_pm == NULL){
		printf("w2c pm error\n");
		return 0;
	}
	//printf("\n");	
	//print_1x4_matrix(point_from_vm);
	//print_1x4_matrix(point_from_pm);
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
	//printf("%d,%d\n", (int)to[0], (int)to[1]);
	SetCursorPos((int)to[0], (int)to[1]);
	//Sleep(100);
	//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	
	return 1;
}

void get_process_handle()
{
	DWORD pid =0;
	hWnd = FindWindow(0,"Diablo III");
	if(hWnd == 0) {
		printf("FindWindow failed, %ld\n", GetLastError());
		return;
	}
	GetWindowThreadProcessId(hWnd, &pid);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if(hProcess == 0) {
		printf("OpenProcess failed, %ld\n",GetLastError());
		return;
	}
	
	
}

void get_view_matrix()
{
	// [[[BaseAddress + 0x26767C]+0x598]+0x1EC]+0x9F2
	PDWORD BaseAddress = (PDWORD)0x01C12E98;
	//PDWORD pdwAddress = ( PDWORD )*( PDWORD )(BaseAddress + 0xA0);
	//PDWORD pdwFinalAddress = ( PDWORD )*( PDWORD )( pdwAddress + 150 );
	DWORD tmp;
	DWORD tmp1;
	//DWORD tmp2;
	read_bytes((PCVOID)(BaseAddress), sizeof(tmp), &tmp);
	read_bytes((PCVOID)(tmp + 0xA00), sizeof(tmp1), &tmp1);
	read_bytes((PCVOID)(tmp1+ 0x150), 64, view_matrix);
	read_bytes((PCVOID)(tmp1+ 0x190), 64, proj_matrix);
	//printf("Pointer:%lX\nPointer1:%lX\n\n",tmp,tmp1);
	//print_4x4_matrix(view_matrix);
	//printf("\n");
	//print_4x4_matrix(proj_matrix);
}

void get_player_cord(float *x, float *y, float *z)
{
	//DWORD tmp = get_entity_pointer();
	get_cord_by_num(get_num_local_player(), x, y, z);
	// read_bytes((PCVOID)(tmp + (0x2F8 * get_num_local_player() + 0xD0)),	sizeof(x), x);
	// read_bytes((PCVOID)(tmp + (0x2F8 * get_num_local_player() + 0xD4)),	sizeof(y), y);
	// read_bytes((PCVOID)(tmp + (0x2F8 * get_num_local_player() + 0xD8)),	sizeof(z), z);
	//read_bytes((PCVOID)(tmp1 + (0x1A590 + 0xD0)),	sizeof(x), x);
	//read_bytes((PCVOID)(tmp1 + (0x1A590 + 0xD4)),	sizeof(y), y);
	//printf("Pointer:%lX:x:%f:y:%f:z:%f\n", tmp, *x, *y, *z);
	//read_bytes((PCVOID)(tmp),				sizeof(tmp), &tmp1);
}

DWORD get_num_local_player()
{
	DWORD baseNumPlayer = 0x1C12E98; //- Base entity list(not entity)
	DWORD tmp;
	DWORD tmp1;
	DWORD result = 0;
	read_bytes((PCVOID)(baseNumPlayer),			sizeof(tmp), &tmp);
	read_bytes((PCVOID)(tmp + 0x8F4),			sizeof(tmp), &tmp1);
	read_bytes((PCVOID)(tmp1 + 0x3c), 			1, &result);
	//printf("get_num_local_player:%lX\n",result);
	return result;
}

void print_1x4_matrix(void *matrix)
{
	for (int i = 0; i < 4; i++){
		printf("%10.4f ", *(float*)(matrix+4*i));
	}
	printf("\n");
}

void print_4x4_matrix(void *matrix)
{
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			printf("%10.4f ", *(float*)(matrix+4*j+4*4*i));
		}
		printf("\n");
	}
}

void mul_matrix(void *matrix1, int x1, int y1, 
				 void *matrix2, int x2, int y2, 
				 void **matrix3)
{
	if(y1 != x2){
		printf("mul_matrix: incorrect matrix\n");
		return;
	}
	*matrix3 = calloc(x1 * y2, sizeof(float));
	//*(float *)*matrix3 = 1;
	//printf("%p\n", (float *) *matrix3 );
	
	for (int i = 0; i < x1; i++){
		for (int j = 0; j < y2; j++){
			float sum = 0;
			for (int k = 0; k < y1; k++){
				sum += *((float *) matrix1 + 4 * i + k) * *((float *) matrix2 + 4 * k + j);
				//printf("m1:%f : m2:%f\n",*((float *) matrix1 + 4 * i + k), *((float *) matrix2 + 4 * k + j));
			}
			*((float *) *matrix3 + 4 * i + j) = sum;
			//printf("P:%p sum:%d\n",(float *) *matrix3 + 4 * i + j, (int)sum);
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
	//printf("num elements in entity:%lX\n",tmp1);
	return (int)tmp1;
}

int get_name_by_num(int num, char* buf, int n)
{
	DWORD tmp = get_entity_pointer();
	DWORD tmp1;
	if (!get_unit_info_by_offset(num, OFFSET_ID , sizeof(tmp1), &tmp1)) {
		printf("Error get name by num\n");
		return 0;
	}
	//read_bytes((PCVOID)(tmp + (0x2F8 * num) ),	4, &tmp1);
	if ((int) tmp1 == -1) {
		return 0;
	}
	if (!get_unit_info_by_offset(num, OFFSET_NAME , n, buf)) {
		printf("Error get name by num\n");
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
	//DWORD tmp = get_entity_pointer();
	// read_bytes((PCVOID)(tmp + (0x2F8 * num + OFFSET_X)), sizeof(x), x);
	// read_bytes((PCVOID)(tmp + (0x2F8 * num + OFFSET_Y)), sizeof(y), y);
	// read_bytes((PCVOID)(tmp + (0x2F8 * num + OFFSET_Z)), sizeof(z), z);
	//printf("Pointer:%lX:x:%f:y:%f:z:%f\n", tmp, *x, *y, *z);
}

int get_unit_info_by_offset(int num, DWORD off , size_t size, void *data)
{
	DWORD tmp = get_entity_pointer();
	
	if (!read_bytes((PCVOID)(tmp + (0x2F8 * num + off)), size, data)) {
		printf("Error get unit info by offset\n");
		return 0;
	}
	return 1;
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
	//DWORD lpflOldProtect;
	//VirtualProtectEx(hProcess, (PVOID)addr, num, 0x40, &lpflOldProtect);
	//printf("%ld\n", lpflOldProtect);
    int r = ReadProcessMemory(hProcess, addr, buf, num, &sz);
	//printf("%d\n", r);
	//VirtualProtectEx(hProcess, (PVOID)addr, num, lpflOldProtect, &lpflOldProtect);
	if (r == 0 || sz == 0) {
        printf("RPM error, %lX\n", GetLastError());
		return 0;
	}
	return 1;
}