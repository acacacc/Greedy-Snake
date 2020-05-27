#include < iostream >
#include <algorithm>
#include <graphics.h>
#include <time.h>
using namespace std;
//地图大小为640,480，界面为1000,480；
//蛇一个单位的大小为20*20;
struct snake {
	int x, y;
	snake *next;
};

struct fnode {
	int x, y;
	bool statu;
}food[7];

struct bnode {
	int x, y;
	bool statu;
}boom[8];

struct record {
	char name[20];
	int score;
};

snake *head;
int score, Time, level, color;

void my_sort(record *rec, int len) {
	int i, j;
	record tmp;
	for (i = 0; i < len - 1; i++)
		for (j = 0; j < len; j++) {
			if (rec[j].score < rec[j + 1].score) {
				tmp = rec[j];
				rec[j] = rec[j + 1];
				rec[j + 1] = tmp;
			}
		}
}

void set_data_into_file(record *rec)
{
	int i, cnt = 0;
	int ret;
	FILE *fp = fopen("record.txt", "w");
	if (!fp) {
		fprintf(stderr, "open file error!\n");
		return;
	}
	for (i = 0; i < 5; i++) {
		ret = fprintf(fp, "%s %d\n", rec[i].name, rec[i].score);
	}
	fclose(fp);
}

int get_data_from_file(record *rec)
{
	int cnt = 0;
	int ret;
	FILE *fp = fopen("record.txt", "r");
	if (!fp) {
		fprintf(stderr, "open file error!\n");
		return -1;
	}
	for (int i = 0; i < 5; i++) {
		ret = fscanf(fp, "%s %d\n", rec[i].name, &rec[i].score);
		if (ret == EOF)
			break;
		cnt++;
	}
	fclose(fp);
	return cnt;
}

bool check_fb(int xx, int yy) {
	snake *pre = head;
	while (pre) {
		if (pre->x == xx && pre->y == yy)
			return 0;
		pre = pre->next;
	}
	for (int i = 1; i < 7; i++) {
		if (xx == food[i].x && yy == food[i].y)
			return 0;
	}
	for (int i = 1; i < 8; i++) {
		if (xx == boom[i].x && yy == boom[i].y)
			return 0;
	}
	return 1;
}

void init_snake() {//初始化
	head = new snake;
	head->x = 120;
	head->y = 220;
	snake *temp = new snake;
	temp = head;
	for (int i = 1; i < 4; i++) {
		snake *p = new snake;
		p->x = 120;
		p->y = 220 + 20 * i;
		p->next = NULL;
		temp->next = p;
		temp = p;
	}
}

void init_boom() {
	for (int i = 1; i < 8; i++) {
		randomize();
		int xx = random(640);
		int yy = random(480);
		while (!check_fb(xx, yy) || xx % 20 != 0 || yy % 20 != 0) {
			xx = random(640);
			yy = random(480);
		}
		boom[i].statu = 1;
		boom[i].x = xx; boom[i].y = yy;
	}
}

void init_food() {
	for (int i = 1; i < 7; i++) {
		randomize();
		int xx = random(640);
		int yy = random(480);
		while (!check_fb(xx, yy) || xx % 20 != 0 || yy % 20 != 0) {
			xx = random(640);
			yy = random(480);
		}
		food[i].statu = 1;
		food[i].x = xx; food[i].y = yy;
	}
}

int eat_boom() {
	for (int i = 1; i < 8; i++) {
		if (head->x == boom[i].x && head->y == boom[i].y) {
			boom[i].statu = 0;
			return i;
		}
	}
	return 0;
}

int eat_food() {
	for (int i = 1; i < 7; i++) {
		if (head->x == food[i].x && head->y == food[i].y) {
			food[i].statu = 0;
			return i;
		}
	}
	return 0;
}

void create_boom(int n) {
	randomize();
	int xx = random(640);
	int yy = random(480);
	while (!check_fb(xx, yy) || xx % 20 != 0 || yy % 20 != 0) {
		xx = random(640);
		yy = random(480);
	}
	boom[n].statu = 1;
	boom[n].x = xx; boom[n].y = yy;
}

void create_food(int n) {//创建食物
	randomize();
	int xx = random(640);
	int yy = random(480);
	while (!check_fb(xx, yy) || xx % 20 != 0 || yy % 20 != 0) {
		xx = random(640);
		yy = random(480);
	}
	food[n].statu = 1;
	food[n].x = xx; food[n].y = yy;
}

void draw() {//刷新界面
	cleardevice();
	PIMAGE img = newimage();
	getimage(img, "pi8.jpg");
	putimage(0, 0, img);
	getimage(img, "pi3.jpg");
	putimage(640, 0, img);
	snake *pre = head;
	while (pre) {
		setfillcolor(color);
		fillellipse(pre->x + 10, pre->y + 10, 11, 11);
		pre = pre->next;
	}
	setfillcolor(BLACK);
	pre = head->next;
	if (head->x == pre->x + 20) {
		fillellipse(head->x + 15, head->y + 4, 2, 2);
		fillellipse(head->x + 15, head->y + 16, 2, 2);
	}
	else if (head->x == pre->x - 20) {
		fillellipse(head->x + 5, head->y + 16, 2, 2);
		fillellipse(head->x + 5, head->y + 4, 2, 2);
	}
	else if (head->y == pre->y - 20) {
		fillellipse(head->x + 4, head->y + 5, 2, 2);
		fillellipse(head->x + 16, head->y + 5, 2, 2);
	}
	else {
		fillellipse(head->x + 4, head->y + 15, 2, 2);
		fillellipse(head->x + 16, head->y + 15, 2, 2);
	}
	setcolor(BLACK);
	setfont(20, 0, "幼圆");
	outtextxy(650, 50, "Game Level:");
	char s[5];
	sprintf(s, "%d", level);
	outtextxy(780, 50, s);
	outtextxy(650, 90, "Your score:");
	score = max(score, 0);
	sprintf(s, "%d", score);
	outtextxy(780, 90, s);
	char s1[] = "W S A D分别控制上下左右";
	char s2[] = "空格键暂停，方向键继续游戏，esc键结束游戏";
	char s3[] = ", 速度减慢    . 速度加快";
	char s4[] = "等级越高，得到的分数越高！";
	char s5[] = "——获得1分";
	char s6[] = "——获得2分";
	char s7[] = "——获得3分";
	char s8[] = "——减少2分";
	setfont(15, 0, "幼圆");
	outtextxy(650, 130, s1);
	outtextxy(650, 150, s2);
	outtextxy(650, 170, s3);
	outtextxy(650, 190, s4);
	getimage(img, "f1.jpg");
	putimage(650, 220, img);
	for (int i = 1; i < 4; i++)
		putimage(food[i].x, food[i].y, img);
	outtextxy(680, 220, s5);
	getimage(img, "f2.jpg");
	putimage(650, 250, img);
	for (int i = 4; i < 6; i++)
		putimage(food[i].x, food[i].y, img);
	outtextxy(680, 250, s6);
	getimage(img, "f3.jpg");
	putimage(650, 280, img);
	putimage(food[6].x, food[6].y, img);
	outtextxy(680, 280, s7);
	getimage(img, "boom.jpg");
	putimage(650, 310, img);
	for (int i = 1; i < 8; i++) {
		putimage(boom[i].x, boom[i].y, img);
	}
	outtextxy(680, 310, s8);
}

void end_game(record temp) {//游戏结束
	char s1[] = "If you want to continue playing, press the space bar or esc to exit...";
	char s2[] = "Your score:";
	char s3[5];
	char s4[] = "RATING:";
	score = max(score, 0);
	sprintf(s3, "%d", score);
	cleardevice();
	setcolor(BLACK);
	setfont(30, 25, "仿宋");
	PIMAGE img = newimage();
	getimage(img, "pi9.jpg");
	putimage(0, 0, img);
	outtextxy(150, 60, s2);
	outtextxy(470, 60, s3);
	setfont(20, 0, "仿宋");
	outtextxy(150, 130, s1);
	setcolor(RED);
	outtextxy(150, 170, s4);
	record rec[10];
	int num = get_data_from_file(rec);
	temp.score = score;
	if (temp.score > rec[num - 1].score)
		rec[num - 1] = temp;
	my_sort(rec, num);
	int q = 0, p = 0;
	for (int i = 0; i < 5; i++) {
		outtextxy(150, 200 + q, rec[i].name);
		q += 30;
		char sc[5];
		sprintf(sc, "%d", rec[i].score);
		outtextxy(250, 200 + p, sc);
		p += 30;
	}
	set_data_into_file(rec);
}

void move_snake(record t, char s[]) {//移动蛇
	snake *pre = new snake;
	snake *temp = new snake;
	pre = head->next;
	int frox = head->x, froy = head->y;
	if (s[0] == 's') {
		while (pre) {
			temp->x = pre->x, temp->y = pre->y;
			pre->x = frox, pre->y = froy;
			pre = pre->next;
			frox = temp->x, froy = temp->y;
		}
		head->y -= 20;
	}
	else if (s[0] == 'x') {
		while (pre) {
			temp->x = pre->x, temp->y = pre->y;
			pre->x = frox, pre->y = froy;
			pre = pre->next;
			frox = temp->x, froy = temp->y;
		}
		head->y += 20;
	}
	else if (s[0] == 'z') {
		while (pre) {
			temp->x = pre->x, temp->y = pre->y;
			pre->x = frox, pre->y = froy;
			pre = pre->next;
			frox = temp->x, froy = temp->y;
		}
		head->x -= 20;
	}
	else if (s[0] == 'y') {
		while (pre) {
			temp->x = pre->x, temp->y = pre->y;
			pre->x = frox, pre->y = froy;
			pre = pre->next;
			frox = temp->x, froy = temp->y;
		}
		head->x += 20;
	}
	int ef = eat_food();
	int eb = eat_boom();
	if (ef) {
		if (ef >= 1 && ef < 4) {
			score++;
			snake *now = new snake;
			now->x = frox, now->y = froy;
			now->next = NULL;
			pre = head;
			while (pre->next) {
				pre = pre->next;
			}
			pre->next = now;
		}
		else if (ef >= 4 && ef < 6) {
			score += 2;
			pre = head;
			while (pre->next->next) {
				pre = pre->next;
			}
			if (pre->x == pre->next->x + 20) {//y
				pre = pre->next;
				snake *t1 = new snake;
				snake *t2 = new snake;
				t1->x = pre->x - 20; t1->y = pre->y;
				t2->x = t1->x - 20; t2->y = pre->y;
				t2->next = NULL;
				t1->next = t2;
				pre->next = t1;
			}
			else if (head->x == pre->x - 20) {//z
				pre = pre->next;
				snake *t1 = new snake;
				snake *t2 = new snake;
				t1->x = pre->x + 20; t1->y = pre->y;
				t2->x = t1->x + 20; t2->y = pre->y;
				t2->next = NULL;
				t1->next = t2;
				pre->next = t1;
			}
			else if (head->y == pre->y - 20) {//s
				pre = pre->next;
				snake *t1 = new snake;
				snake *t2 = new snake;
				t1->x = pre->x; t1->y = pre->y + 20;
				t2->x = t1->x; t2->y = pre->y + 20;
				t2->next = NULL;
				t1->next = t2;
				pre->next = t1;
			}
			else {//x
				pre = pre->next;
				snake *t1 = new snake;
				snake *t2 = new snake;
				t1->x = pre->x; t1->y = pre->y - 20;
				t2->x = t1->x; t2->y = pre->y - 20;
				t2->next = NULL;
				t1->next = t2;
				pre->next = t1;
			}
		}
		else {
			score += 3;
			pre = head;
			while (pre->next->next) {
				pre = pre->next;
			}
			if (pre->x == pre->next->x + 20) {//y
				pre = pre->next;
				for (int i = 0; i < 3; i++) {
					snake *now = new snake;
					now->x = pre->x - 20; now->y = pre->y;
					now->next = NULL;
					pre->next = now;
					pre = now;
				}
			}
			else if (pre->x == pre->next->x - 20) {//z
				for (int i = 0; i < 3; i++) {
					snake *now = new snake;
					now->x = pre->x + 20; now->y = pre->y;
					now->next = NULL;
					pre->next = now;
					pre = now;
				}
			}
			else if (pre->y == pre->next->y - 20) {//s
				for (int i = 0; i < 3; i++) {
					snake *now = new snake;
					now->x = pre->x; now->y = pre->y + 20;
					now->next = NULL;
					pre->next = now;
					pre = now;
				}
			}
			else {//x
				pre = pre->next;
				for (int i = 0; i < 3; i++) {
					snake *now = new snake;
					now->x = pre->x; now->y = pre->y - 20;
					now->next = NULL;
					pre->next = now;
					pre = now;
				}
			}
		}
		if (score % 10 == 0 && score && Time > 50) {
			Time -= 50;
			level++;
		}
		create_food(ef);
	}
	else if (eb) {
		score -= 2;
		if (score <= 0) {
			end_game(t);
			return;
		}
		snake *now = new snake;
		now = head;
		while (now->next->next->next) {
			now = now->next;
		}
		delete(now->next->next);
		delete(now->next);
		now->next = NULL;
		create_boom(eb);
	}
	draw();
}

bool snake_crash() {//判断是否碰撞
	snake *pre = head->next;
	if (head->x >= 640 || head->x < 0 || head->y >= 480 || head->y < 0)
		return 0;
	while (pre) {
		if (head->x == pre->x && head->y == pre->y)
			return 0;
		pre = pre->next;
	}
	return 1;
}

void auto_move(record temp) {//自动移动
	snake *pre = head->next;
	if (head->x == pre->x + 20)
		move_snake(temp, "y");
	else if (head->x == pre->x - 20)
		move_snake(temp, "z");
	else if (head->y == pre->y - 20)
		move_snake(temp, "s");
	else
		move_snake(temp, "x");
}

void begin(record temp) {//开始游戏
	init_snake();
	init_boom();
	init_food();
	bool flag = 1;
	MUSIC mus;
	mus.OpenFile("m.mp3");
	for (; is_run(); delay_fps(60)) {
		if (mus.GetPlayStatus() == MUSIC_MODE_STOP) {
			mus.Play(0);
		}
		delay(Time);
		if (kbhit()) {
			int dir = getch();
			switch (dir)
			{
			case 'w': move_snake(temp, "s"); break;//72
			case 's': move_snake(temp, "x"); break;//80
			case 'a': move_snake(temp, "z"); break;//75
			case 'd': move_snake(temp, "y"); break;//77
			case ',': Time += 50; if (Time > 300)Time = 300; level--; if (level < 1)level = 1; auto_move(temp); break;
			case '.': Time -= 50; if (Time < 50) Time = 50; level++; if (level > 6)level = 6; auto_move(temp); break;
			case 32:while (dir == ' ')dir = getch(); break;
			case key_esc:flag = 0; break;
			default:break;
			}
		}
		else
			auto_move(temp);
		if (!snake_crash() || score == 768 || !flag || score <= 0) {
			end_game(temp);
			break;
		}
	}
	mus.Close();
}

bool welcome() {//开始界面 420-610  445
	PIMAGE img = newimage();
	getimage(img, "pi1.jpg");
	putimage(0, 0, img);
	setbkmode(TRANSPARENT);
	bool flag = 0;
	mouse_msg msg = { 0 };
	for (; is_run(); delay_fps(60)) {
		while (mousemsg()) {
			msg = getmouse();
		}
		if (msg.is_down() && msg.x >= 420 && msg.x <= 610 && msg.y >= 400 && msg.y <= 445) {
			flag = 1;
			break;
		}
	}
	if (flag)
		return 1;
	return 0;
}

void select_color() {
	bool flag = 0;
	cleardevice();
	setbkcolor(WHITE);
	char s[] = "请点击下图任意位置，为蛇选择一种颜色";
	setcolor(BLACK);
	setfont(20, 0, "宋体");
	outtextxy(100, 10, s);
	PIMAGE img = newimage();
	getimage(img, "rgb.jpg");
	putimage(200, 100, img);
	mouse_msg msg = { 0 };
	color_t t;
	for (; is_run(); delay_fps(60)) {
		while (mousemsg()) {
			msg = getmouse();
			if (msg.is_down()) {
				flag = 1;
				t = getpixel(msg.x, msg.y);
				break;
			}
		}
		if (flag) {
			color = t;
			break;
		}
	}
}

int main() {
	int k = 0;
	while (k != key_esc) {
		score = 3; Time = 300, level = 1;
		record temp;
		initgraph(1000, 480);
		if (welcome()) {
			inputbox_getline("贪吃蛇", "请输入你的名字，回车结束", temp.name, sizeof(temp.name) / sizeof(*temp.name));
			select_color();
			begin(temp);
		}
		while (k != ' ' && k != key_esc)
			k = getch();
		k = 0;
		closegraph();
	}
	return 0;
}