### 辽宁理工学院滨海校区导航系统

1. 数据结构的定义与其意义

   为方便数据的存储，项目定义了两类数据结构，分别为`Edge`与`Node`，分别表示景点地图中的景点与路径。下面分别为两种结构的结构体源代码。

   1. 边结构`Edge`

   ```c
   typedef struct { 
       int start_index, end_index;
       int dis;
       char show;
   } Edge;
   ```

   对于每条路径(边)，项目中定义了他有一个起始节点`start_index`与结束节点`end_index`,并且定义了这条边的长度`dis`，最后的`show`参数表示当前路径应该以什么样的字符绘制在地图上。

   2. 点结构`Node`

   ```c
   typedef struct {
       int x, y;
       char name[20];
       char describe[100];
   } Node;
   ```

   对于每个景点，项目中定义了它有一对坐标 `x` 与`y`，并且有一个名字`name`与描述`describe`。

2. 全局变量的定义与其意义

   |    类型    |      变量名      | 初始值 |          意义          |
   | :--------: | :--------------: | :----: | :--------------------: |
   | `#define`  |     `height`     |  `20`  |    打印的地图的高度    |
   | `#define`  |     `width`      | `100`  |    打印的地图的宽度    |
   |  `Edge*`   |    `edgeList`    | `NULL` |        路的列表        |
   |   `int`    |    `edgeSize`    |  `0`   |        路的数量        |
   |  `Node*`   |    `nodeList`    | `NULL` |       景点的列表       |
   |   `int`    |    `nodeSize`    |  `0`   |       景点的数量       |
   |   `int`    |    `maxNodeX`    |  `-1`  |  景点坐标中最大的X值   |
   |   `int`    |    `maxNodeY`    |  `-1`  |  景点坐标中最大的Y值   |
   | `int[100]` |      `road`      | `{0}`  | 寻路时存储路的中间数组 |
   |   `int`    |   `road_index`   |  `0`   |     road数组的长度     |
   |   `int`    |   `print_road`   |  `0`   |     打印地图的模式     |
   | `int[100]` |    `all_road`    | `{0}`  | 寻路时存储路的结果数组 |
   |   `int`    | `all_road_index` |  `0`   |     all_road的长度     |
   |   `int`    |   `now_print`    |  `0`   | 当前应该打印的路的下标 |

3. 数据读取函数

   数据读取函数的源代码如下

   ```c
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
   ```

   该函数从文件`info.txt`中读取学校各个建筑物的位置信息以及道路信息，其中`info.txt`文件的内容如下所示。

   > 14 22
   > 0 0 B栋 宿舍B
   > 0 3 C栋 宿舍C
   > 0 6 小体育场 小球场
   > 3 0 A栋 周景鑫住的宿舍A
   > 3 3 智能楼 一栋楼
   > 3 6 工程训练中心 一栋带机房的楼
   > 5 0 食堂 吃饭地方
   > 5 3 机电楼 另一栋楼
   > 5 6 科技实验楼 不常去的楼
   > 7 6 明德楼 有物理实验等等的楼
   > 8 0 大体育场 大球场
   > 8 3 图书馆 装修未半而中道崩殂的楼
   > 8 8 大门 门口
   > 9 6 知行楼 老师办公室
   > 0 1 90
   > 0 3 120
   > 1 2 90
   > 1 4 120
   > 2 5 120
   > 3 6 60
   > 3 4 90
   > 4 7 60
   > 4 5 60
   > 5 8 80
   > 6 10 120
   > 6 11 150
   > 6 7 90
   > 7 9 120
   > 8 9 184
   > 9 11 100
   > 9 12 90
   > 9 13 80
   > 10 11 60
   > 11 13 100
   > 11 12 150
   > 12 13 90

   由于GitHub中会将数据压缩至一行显示，如需拷贝数据可以移步CSDN（文末链接）或直接clone本项目，下图为部分数据。

   ![info样例](/images/info样例.jpg)

   其中第一行的两个数字`m`,`n`, 分别代表了地图中有`m`个景点以及`n`条路。

   接下来`m`行，其中每一行都由空格分割成四部分，第一、第二部分表示此景点的`x`,`y`坐标，第三部分为该景点的名称，第四部分为该景点的描述，对于任意一行而言，我们都令该景点的`id`为描写该景点的行数`i-1`。

   接下来`n`行，每一行都由空格分割成三部分，其中第一、第二部分为路的起点与终点的`id`，第三部分为这条路的长度。

   通过这个函数，程序将`info.txt`文件中的数据读取到程序中。

4. 菜单函数

   ```c
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
   ```

   菜单函数负责将菜单打印到屏幕上，并获取用户的输入。只有用户的输入为合法(即1-5)时，函数才进行返回，否则函数将持续提示用户进行输入。

5. 打印地图函数

   ```c
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
   			for (_i = min(start.x, end.x);_i < max(end.x, start.x); _i++) {
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
   			for (_i = min(start.y, end.y);_i < max(end.y, start.y); _i++) {
   				int _x = start.x + strlen(start.name) / 2;
   				if (map[_i][_x] == empty) {
   					if (print_road && in_road(i)) {
   						map[_i][_x] = '*';
   					} else {
   						map[_i][_x] = '|';
   					}
   				} else if (map[_i][_x] == '-') {
   					map[_i][_x] = '+';
   				}
   			}
   		}
   		if (e.show == '\\') {
   			float det = 0;
   			int _i;
   			for (_i = min(start.y, end.y);_i <= max(end.y, start.y);_i++) {
   				int _x = start.x + 0.5 * strlen(start.name);
   				if (map[_i][(int)(_x + det)] == empty) {
   					if (print_road && in_road(i)) {
   						map[_i][(int)(_x + det)] = '*';
   					} else {
   						map[_i][(int)(_x + det)] = '\\';
   					}
   				}
   				det += 1.0 * (max(end.x, start.x) - min(end.x, start.x) - 0.3 * strlen(end.name))
   				       / (max(end.y, start.y) - min(end.y, start.y));
   			}
   		}
   		if (e.show == '/') {
   			float det = 0;
   			int _i;
   			for (_i = min(start.y, end.y);_i <= max(end.y, start.y);_i++) {
   				int _x = end.x + 0.5 * strlen(end.name);
   
   				if (map[_i][(int)(_x + det)] == empty) {
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
   ```

   函数中首先初始化一个名为`map`的二维数组，首先遍历所有景点，将所有景点的名字写入`map`中，然后遍历所有的路径，对每个路径按照其`show`属性分别使用不同的绘图方式将其绘制到二维数组  中，最后将`map`数组打印到控制台上，即完成了地图的打印。需要注意的是，此时的`print_road=0`，说明此时打印地图的方式为正常打印。

6. 打印描述函数

   ```c
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
   ```

   此函数主要由一个循环组成，遍历所有的景点并将他们与所对应的描述进行打印。  

7. 寻路函数 **重要！！！**

   ```c
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
   ```

   此函数首先接受用户输入的两个地点，然后调用深度优先搜索函数，对两个地点进行搜索，并将最后的结果打印出来。其中本函数使用了多个辅助函数，分别为`copyArray`，`_dfsRoad`，`printVisit`其中它们的代码分别如下。

   ```c
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
   ```

   ```c
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
   	if (visit[start] != -1)return 0;
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
   ```

   ```c
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
   ```

   其中`_dfsRoad`函数的各个参数以及意义如下表所示。

   | 参数类型 |  参数名  |   意义   |
   | :------: | :------: | :------: |
   |  `int`   | `start`  | 开始节点 |
   |  `int`   |  `end`   | 结束节点 |
   |  `int`   | `depth`  |   深度   |
   |  `int*`  | `visit`  | 访问数组 |
   |  `int*`  |  `res`   | 当前结果 |
   |  `int*`  |  `dis`   | 最小距离 |
   |  `int`   | `nowDis` | 当前距离 |

8. 帮助函数

   ```c
   void printHelp() {
   	printf("1. 显示地图将显示我校地图简图\n2. 建筑信息将显示建筑的信息\n");
   	printf("3. 我要寻路需要你输入起点和终点, 系统自动选择最短路径\n");
   	printf("4. 赛博游校需要你输入起点和终点, 系统将选择可逛景点最多且距离最短的路线\n");
   }
   ```

9. 游校函数

   ```c
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
   			now_print = i;
   			getchar();
   	}
   	print_road = 0;
   	printMap();
   }
   ```

   其中`_dfsSight`函数的各个参数意义几乎同`_dfsRoad`，仅将最短距离改为最长距离。

10. main函数

    ```c
    int main() {
    	loadInfo();
    	int res = -1;
    	while (res != 0) {
    		res = menu();
    		if (res == 1) {
    			printMap();
    		}
    		if (res == 2) {
    			printDec();
    		}
    		if (res == 3) {
    			searchRoad();
    		}
    		if (res == 4) {
    			sight();
    		}
    		if (res == 5) {
    			printHelp();
    		}
    	}
    	printf("感谢使用 再见.");
    	return 0;
    }
    ```

11. 详细效果

    打印地图

    ![打印地图](images\打印地图.jpg)

    显示建筑信息

    ![显示建筑信息](/images/显示建筑信息.jpg)

    寻路模块

    ![寻路](/images/寻路.jpg)

    通过上图我们可知，从大门到C栋的最短距离为：大门->明德楼->机电楼->智能楼->C栋，其最短距离为390，系统将以星号标注路线。

    赛博游校模块

    ![赛博游校模块](/images/赛博游校.jpg)

    如上图，经过系统推荐，从大门到科技实验楼的游览路线为：大门->明德楼->知行楼->图书馆->大体育场->食堂->机电楼->智能楼->A栋->B栋->C栋->小体育场->工程训练中心->科技实验楼，且系统将动态演示游览路径，以星号标注游览路线。

    系统帮助

    ![系统帮助](/images/系统帮助.jpg)

12. 代码链接

    代码的详细介绍已上传至csdn，链接：[CSDN](https://blog.csdn.net/m0_72416569/article/details/144856074)

    开发环境`小熊猫Dev-C++ 6.7.5`

13. TODO

    代码中关于打印地图部分解释的不到位

14. 联系作者：绿泡泡`zjxyyds0307`