#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define height 20
#define width 100

int max(int x, int y) {
	return x > y ? x : y;
}

int min(int x, int y) {
	return x > y ? y : x;
}

typedef struct {
	int start_index, end_index;
	int dis;
	char show;
} Edge;

typedef struct {
	int x, y;
	char name[20];
	char describe[100];
} Node;


Edge* edgeList;
int edgeSize = 0;
Node* nodeList;
int nodeSize = 0;
int maxNodeX = -1, maxNodeY = -1;

int road[100];
int road_index = 0;
int print_road = 0;
int all_road[100];
int all_road_index = 0;
int now_print = 0;

int in_road(int index) {
	int i;
	if (print_road == 1) {
		for (i = 0; i < all_road_index; i++) {
			if (all_road[i] == index) {
				return 1;
			}
		}
		return 0;
	} else if (print_road == 2) {
//		return 1;
//		printf("%d: all_road[%d]: %d\n", __LINE__, index, all_road[index]);
		if (all_road[now_print] == index) {
//			now_print += 1;
			return 1;
		}
		return 0;
	}
	
//	scanf("%d");
}

void printMap() {
	int i, j;

	system("cls");
	char empty = ' ';
	char map[height][width];
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			map[i][j] = empty;
		}
	}
	for (i = 0; i < nodeSize; i++) {
		Node n = nodeList[i];
		int len = strlen(n.name);
		for (j = 0; j < len; j++) {
			map[n.y][n.x + j] = n.name[j];
		}
	}

	for (i = 0; i < edgeSize; i++) {
		Edge e = edgeList[i];
		Node start = nodeList[edgeList[i].start_index];
		Node end = nodeList[edgeList[i].end_index];
		if (e.show == '-') {
			int _i;
			for (_i = min(start.x, end.x); _i < max(end.x, start.x); _i++) {
				if (map[start.y][_i] == empty) {
					if (print_road && in_road(i)) {
						map[start.y][_i] = '*';
					} else {
						map[start.y][_i] = '-';
					}

				}

			}
		}

		if (e.show == '|') {
			int _i;
			for (_i = min(start.y, end.y); _i < max(end.y, start.y); _i++) {
				int _x = start.x + strlen(start.name) / 2;
				if (map[_i][_x] == empty) {
					if (print_road && in_road(i)) {
						map[_i][_x] = '*';
					} else {
						map[_i][_x] = '|';
					}
					//					map[i][_x] = '|';
				} else if (map[_i][_x] == '-') {
					map[_i][_x] = '+';
				}

			}
		}

		if (e.show == '\\') {
			float det = 0;
			int _i;
			for (_i = min(start.y, end.y); _i <= max(end.y, start.y); _i++) {
				int _x = start.x + 0.5 * strlen(start.name);

				if (map[_i][(int)(_x + det)] == empty) {
					if (print_road && in_road(i)) {
						map[_i][(int)(_x + det)] = '*';
					} else {
						map[_i][(int)(_x + det)] = '\\';
					}
					//					map[i][(int)(_x + det)] = '\\';
				}

				det += 1.0 * (max(end.x, start.x) - min(end.x, start.x) - 0.3 * strlen(end.name))
				       / (max(end.y, start.y) - min(end.y, start.y));


			}
		}

		if (e.show == '/') {
			float det = 0;
			int _i;
			for (_i = min(start.y, end.y); _i <= max(end.y, start.y); _i++) {
				int _x = end.x + 0.5 * strlen(end.name);

				if (map[_i][(int)(_x + det)] == empty) {
					//					map[i][(int)(_x + det)] = '/';
					if (print_road && in_road(i)) {
						map[_i][(int)(_x + det)] = '*';
					} else {
						map[_i][(int)(_x + det)] = '/';
					}
				}
				det -= 1.0 * (max(end.x, start.x) - min(end.x, start.x) + 0.3 * strlen(end.name))
				       / (max(end.y, start.y) - min(end.y, start.y));
			}
		}
	}

	for (i = 0; i < 5; i++) {
		printf("\t");
	}
	printf("辽宁理工学院滨海校区地图示意");
	for (i = 0; i < 2; i++) {
		printf("\n");
	}
	for (i = 0; i < height; i++) {
		for (j = 0; j < 10; j++) {
			printf(" ");
		}
		for (j = 0; j < width; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
	for (i = 0; i < 2; i++) {
		printf("\n");
	}
}

int* copyArray(int *src) {
	int i;
	int* arr = (int*)malloc(nodeSize * sizeof(int));
	if (src) {
		for (i = 0; i < nodeSize; i++) {
			arr[i] = src[i];
		}
	} else {
		for (i = 0; i < nodeSize; i++) {
			arr[i] = -1;
		}
	}
	return arr;
}
/**
 * @brief 将数组访问的节点打印出来
 *
 * @param visit 要打印的数组
 **/
void printVisit(int* visit) {
	int i, j;
	for (i = 0; i <= nodeSize; i++) {
		for (j = 0; j < nodeSize; j++) {
			if (visit[j] == i) {
				printf("%s\t", nodeList[j].name);
			}
		}
	}
}
/**
 * @brief 深度优先搜索寻路
 *
 * @param start 开始节点
 * @param end 结束节点
 * @param depth 深度
 * @param visit 访问数组
 * @param res 当前结果
 * @dis 最小距离
 * @param nowDis 当前距离
 *
 * @return
 **/


int __dfsRoad(int start, int end, int depth, int* visit, int*res, int*dis, int nowDis) {

	int i;
	if (start == end && depth != 0) {

		visit[start] = depth;

		if (*dis > nowDis) {
			for (i = 0; i < road_index; i++) {
				all_road[i] = road[i];
			}
			all_road_index = road_index;
			*dis = nowDis;
			for (i = 0; i < nodeSize; i++) {
				res[i] = visit[i];
			}
		}
		return 1;
	}
	if (visit[start] != -1) {
		return 0;
	}

	for (i = 0; i < edgeSize; i++) {
		Edge e = edgeList[i];
		int* _visit;
		int _end;
		road[road_index] = i;
		road_index += 1;
		if (e.start_index == start) {
			_end = e.end_index;
			_visit = copyArray(visit);
			_visit[start] = depth;
			__dfsRoad(_end, end, depth + 1, _visit, res, dis, nowDis + e.dis);
		}

		if (e.end_index == start) {
			_end = e.start_index;
			_visit = copyArray(visit);
			_visit[start] = depth;
			__dfsRoad(_end, end, depth + 1, _visit, res, dis, nowDis + e.dis);
		}

		road_index -= 1;
		road[road_index] = 0;
	}
}

int searchRoad() {
	int i;
	print_road = 1;
	int* visit = copyArray(NULL);
	int* res = copyArray(NULL);
	int dis = 2 << 20;
	int start_index = -1, end_index = -1;
	printf("请输入起点: ");
	char start[50] = {0};
	scanf("%s", start);
	for (i = 0; i < nodeSize; i++) {
		if (strcmp(start, nodeList[i].name) == 0) {
			start_index = i;
			break;
		}
	}
	if (start_index == -1) {
		printf("未找到地名 %s\n", start);
		return -1;
	}
	char end[50] = {0};
	printf("请输入终点: ");
	scanf("%s", end);
	for (i = 0; i < nodeSize; i++) {
		if (strcmp(end, nodeList[i].name) == 0) {
			end_index = i;
			break;
		}
	}
	if (end_index == -1) {
		printf("未找到地名 %s\n", end);
		return -1;
	}
	if (end_index == start_index) {
		printf("不需要走了");
	}
	__dfsRoad(start_index, end_index, 0, visit, res, &dis, 0);
	printMap();
	printf("经过系统推荐 从%s到%s的最短路径为: \n", start, end);

	printVisit(res);
	printf("\n");
	printf("需要走的距离为: %d\n", dis);

}
/**
 * @brief 获取当前游览了多少个景点
 *
 * @param visit 访问数组
 *
 * @return 游览的景点个数
 **/
int getSightSize(int *visit) {
	int res = 0, i = 0;
	for (i = 0; i < nodeSize; i++) {
		if (visit[i] != -1) res++;
	}
	return res;
}

/**
 * @brief 深度优先搜索进行观赏(游览的景点最多)
 *
 * @param start 开始的节点编号
 * @param end 结束的节点编号
 * @param depth 深度(当前走了几段)
 * @param visit 一个数组, 用于标记走的是否有重复
 * @param res 一个数组, 用于标记结果
 * @param dis 一个指针, 用于记录当前已经走过的最长的路径
 * @param nowDis 当前走过的距离
 *
 * @return
 **/
int __dfsSight(int start, int end, int depth, int* visit, int*res, int*dis, int nowDis) {
	int i;
	if (start == end && depth != 0) {
		// 走到end了
		visit[start] = depth;

		if (getSightSize(visit) > getSightSize(res)) {
			// 如果游览的景点更多 就更新
			*dis = nowDis;
			for (i = 0; i < nodeSize; i++) {
				res[i] = visit[i];
			}
			for (i = 0; i < road_index; i++) {
				all_road[i] = road[i];
			}
			all_road_index = road_index;

		} else if (getSightSize(visit) == getSightSize(res) && *dis > nowDis) {
			// 如果游览的景点相同多 选距离近的那个
			*dis = nowDis;
			for (i = 0; i < nodeSize; i++) {
				res[i] = visit[i];
			}
			for (i = 0; i < road_index; i++) {
				all_road[i] = road[i];
			}
			all_road_index = road_index;
		}
		return 1;
	}
	if (visit[start] != -1) {
		return 0;
	}

	for (i = 0; i < edgeSize; i++) {
		Edge e = edgeList[i];
		int* _visit;
		int _end;
		road[road_index] = i;
		road_index += 1;
		if (e.start_index == start) {
			_end = e.end_index;
			_visit = copyArray(visit);
			_visit[start] = depth;
			__dfsSight(_end, end, depth + 1, _visit, res, dis, nowDis + e.dis);
		}

		if (e.end_index == start) {
			_end = e.start_index;
			_visit = copyArray(visit);
			_visit[start] = depth;

			__dfsSight(_end, end, depth + 1, _visit, res, dis, nowDis + e.dis);


		}
		road[road_index] = 0;
		road_index -= 1;
	}
}

int sight() {
	int i;
	print_road = 2;
	int* visit = copyArray(NULL);
	int* res = copyArray(NULL);
	int dis = 2 << 20;
	int start_index = -1, end_index = -1;
	printf("请输入起点: ");
	char start[50] = {0};
	scanf("%s", start);
	for (i = 0; i < nodeSize; i++) {
		if (strcmp(start, nodeList[i].name) == 0) {
			start_index = i;
			break;
		}
	}
	if (start_index == -1) {
		printf("未找到地名 %s\n", start);
		return -1;
	}
	char end[50] = {0};
	printf("请输入终点: ");
	scanf("%s", end);
	for (i = 0; i < nodeSize; i++) {
		if (strcmp(end, nodeList[i].name) == 0) {
			end_index = i;
			break;
		}
	}
	if (end_index == -1) {
		printf("未找到地名 %s\n", end);
		return -1;
	}
	__dfsSight(start_index, end_index, 0, visit, res, &dis, 0);
	for(i=0;i<all_road_index;i++){
		printMap();
		printf("经过系统推荐 从%s到%s的最适合游览路径为: \n", start, end);
			if (end_index == start_index) {
				printf("%s\t", start);
			}
		
			printVisit(res);
		
			printf("\n");
			printf("需要走的距离为: %d\n", dis);
//			printf("%d: all_road[%d]: %d\n", __LINE__, i, all_road[i]);
			
			now_print = i;
			getchar();
	}
	print_road = 0;
	printMap();
//	system("cls");
}

void printDec() {
	int i;
	char line[] = "\t\t\t------------------------------------------------\n";
	printf("%s", line);
	printf("\t\t\t|%15s|%30s|\n", "地点名称", "地点描述");
	printf("%s", line);
	for (i = 0; i < nodeSize; i++) {
		printf("\t\t\t|%15s|%30s|\n", nodeList[i].name, nodeList[i].describe);
	}
	printf("%s\n", line);
}


void printHelp() {
	printf("1. 显示地图将显示我校地图简图\n2. 建筑信息将显示建筑的信息\n");
	printf("3. 我要寻路需要你输入起点和终点, 系统自动选择最短路径\n");
	printf("4. 赛博游校需要你输入起点和终点, 系统将选择可逛景点最多且距离最短的路线\n");
}

int menu() {
	char tab[] = "\t\t\t\t";
	int res = -1;
	while (res < 0 || res > 5) {
		printf("%s欢迎使用辽宁理工学院滨海校区导航系统\n", tab);
		printf("1. 显示地图%s2. 建筑信息\n", tab);
		printf("3. 我要寻路%s4. 赛博游校\n", tab);
		printf("5. 系统帮助%s0. 退出系统\n请输入你的选择(0-4): ", tab);
		scanf("%d", &res);
	}
	return res;
}

int loadInfo() {
	int i;
	FILE *fp = fopen("info.txt", "r");
	if (!fp) {
		return 0;
	}
	fscanf(fp, "%d%d", &nodeSize, &edgeSize);
	nodeList = (Node*)(malloc(nodeSize * sizeof(Node)));
	edgeList = (Edge*)(malloc(edgeSize * sizeof(Edge)));

	for (i = 0; i < nodeSize; i++) {
		fscanf(fp, "%d%d%s%s", &nodeList[i].x, &nodeList[i].y, nodeList[i].name, nodeList[i].describe);
		maxNodeX = max(maxNodeX, nodeList[i].x);
		maxNodeY = max(maxNodeY, nodeList[i].y);
	}



	for (i = 0; i < edgeSize; i++) {
		Edge e;
		fscanf(fp, "%d%d%d", &e.start_index,
		       &e.end_index, &e.dis);


		Node start = nodeList[e.start_index];
		Node end = nodeList[e.end_index];
		if (start.x > end.x) {
			Node t = start;
			start = end;
			end = t;
			int ti = e.start_index;
			e.start_index = e.end_index;
			e.end_index = ti;
		}
		if (start.x == end.x) {
			e.show = '|';
		} else if (start.y == end.y) {
			e.show = '-';
		} else if (start.y > end.y) {
			e.show = '/';
		} else if (start.y < end.y) {
			e.show = '\\';
		}


		edgeList[i] = e;

	}

	for (i = 0; i < nodeSize; i++) {
		Node n = nodeList[i];

		n.x = width / maxNodeX * n.x;
		n.y = height / maxNodeY * n.y;
		int len = strlen(n.name);
		n.x = n.x - len;
		if (n.x < 0) n.x += len;
		nodeList[i] = n;
	}
}

int main() {
	loadInfo();
	int res = -1;
	while (res != 0) {
		res = menu();
		if (res == 1) {
			print_road = 0;
			printMap();
		}
		if (res == 2) {
			printDec();
		}
		if (res == 3) {
			print_road = 1;
			searchRoad();
		}
		if (res == 4) {
			print_road = 2;
			sight();
		}
		if (res == 5) {
			printHelp();
		}
	}
	printf("感谢使用 再见.");
	return 0;
}
