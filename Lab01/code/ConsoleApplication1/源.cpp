#include<iostream>  
#include<cmath>  
#include<stdio.h>  
#include<algorithm>  
#include<string.h>  
using namespace std;
int pos, n, Size; ///查找位置,分区数量,最小分割大小  
struct List
{
	int id;///空闲分区编号  
	int sad;///空闲分区首地址  
	int rom;///空间  
	int state;///状态，0为空，1为未满，2为满；  
} L[2000];
struct Task
{
	int id;
	int rom;
	int sad;
} T[2000];
bool cmp(List a, List b)
{
	return a.rom<b.rom;
}
void print()
{
	int i;
	printf("|---------------------------------------------------------------|\n");
	printf("|       分区号    分区始址    分区大小    分区状态       |\n");
	printf("|---------------------------------------------------------------|\n");
	for (i = 1; i <= n; i++)
	{
		printf("|        %3d        %3d         %3d         %3d          |\n",
			L[i].id, L[i].sad, L[i].rom, L[i].state);
		printf("|---------------------------------------------------------------|\n");
	}

}
void recycle()
{
	printf("请输入要释放占用空间任务的序号：\n");
	int l;
	int f1 = 0, f2 = 0, f3 = 0;
	int p1, p2;
	cin >> l;
	for (int i = 1; i <= n; i++)
	{
		if (T[l].sad == L[i].sad + L[i].rom)
		{
			f1 = 1;
			p1 = i;
		}
		if (T[l].sad + T[l].rom == L[i].sad)
		{
			f2 = 1;
			p2 = i;
		}
	}
	//cout<<f1<<" "<<f2<<endl;  
	if (f1 == 1 && f2 == 0)
	{
		L[p1].rom = L[p1].rom + T[l].rom;
	}
	else if (f1 == 0 && f2 == 1)
	{
		L[p2].rom = L[p2].rom + T[l].rom;
		L[p2].sad = T[l].sad;
	}
	else if (f1 == 1 && f2 == 1)
	{
		L[p1].rom = L[p1].rom + T[l].rom + L[p2].rom;
		for (int j = p1 + 1; j <= n; j++)
			L[j].id--;
		n--;
	}
	else if (f1 == 0 && f2 == 0)
	{
		int temp;
		for (int j = 1; j <= n; j++)
		{
			if (L[j].sad>T[l].sad + T[l].rom)///实际上大于T[l].sad就可以  
			{
				temp = j;
			}
		}
		for (int j = temp; j <= n; j++)
			T[j].id++;
		n++;
		L[temp].id = temp;
		L[temp].rom = T[l].rom;
		L[temp].sad = T[l].sad;
		L[temp].state = 0;
	}
	cout << "内存回收完毕！" << endl;
}
void NF()///循环首次适应算法  
{
	cout << "选择采用'循环首次适应算法'进行内存分配\n" << endl;
	int tmp;
	pos = 1;///开始从第一个分区查找  
	while (1)
	{
		cout << "      *******************************************" << endl;
		cout << "      1: 分配内存             2: 回收内存        " << endl;
		cout << endl;
		cout << "      3: 查看空闲分区表       0: 退出       " << endl;
		cout << "      *******************************************" << endl;
		cout << "请输入您的操作 ：";
		cin >> tmp;
		int k = 0;
		if (tmp == 1)
		{
			k++;
			printf("请输入第%d个作业占用空间大小：\n", k);
			cin >> T[k].rom;
			T[k].id = k;
			int num = 0;
			for (int i = pos;; i++)
			{
				num++;
				if (num>n)
				{
					printf("作业请求内存空间过大，空闲分区表不能满足要求，内存分配失败!\n");
					break;
				}
				if (i>n)
				{
					i = 1;
					continue;
				}
				if ((L[i].state == 0 || L[i].state == 1) && L[i].rom >= T[i].rom)
				{
					if (L[i].rom - T[k].rom>Size)
					{
						L[i].sad = L[i].sad + T[k].rom;
						L[i].rom = L[i].rom - T[k].rom;
						L[i].state = 1;
						T[k].sad = L[i].sad - T[k].rom;
						printf("内存分配成功！\n作业申请空间为%d\n起始地址为%d\n", T[k].rom, T[k].sad);
						break;
					}
					else
					{
						L[i].sad = L[i].sad + Size;
						L[i].rom = L[i].rom - Size;
						L[i].state = 2;
						T[k].sad = L[i].sad - Size;
						printf("内存分配成功！\n作业申请空间为%d\n起始地址为%d\n", L[i].rom, T[k].sad);
						break;
					}
				}
				else if (L[i].state = 0 && L[i].rom - T[k].rom == Size)
				{
					L[i].state = 2;
					T[k].sad = L[i].sad;
					printf("内存分配成功！\n作业申请空间为%d\n起始地址为%d\n", T[k].rom, T[k].sad);
					break;
				}
			}
		}
		else if (tmp == 2)
		{
			recycle();
		}
		else if (tmp == 3)
			print();
		else if (tmp == 0)
			return;
		else
		{
			printf("输入有误，请重新输入！\n");
			continue;
		}
	}
}
void BF()///最佳适应算法  
{
	cout << "选择采用'最佳适应算法'进行内存分配\n" << endl;
	sort(L + 1, L + n + 1, cmp);
	int tmp;
	while (1)
	{
		cout << "      *******************************************" << endl;
		cout << "      1: 分配内存             2: 回收内存        " << endl;
		cout << endl;
		cout << "      3: 查看空闲分区表       0: 退出       " << endl;
		cout << "      *******************************************" << endl;
		cout << "请输入您的操作 ：";
		cin >> tmp;
		int k = 0;
		if (tmp == 1)
		{
			k++;
			printf("请输入第%d个作业占用空间大小：\n", k);
			cin >> T[k].rom;
			T[k].id = k;
			int i;
			for (i = 1; i <= n; i++)
			{
				if (i>n)
				{
					i = 1;
					continue;
				}
				if ((L[i].state == 0 || L[i].state == 1) && L[i].rom >= T[i].rom)
				{
					if (L[i].rom - T[k].rom>Size)
					{
						L[i].sad = L[i].sad + T[k].rom;
						L[i].rom = L[i].rom - T[k].rom;
						L[i].state = 1;
						T[k].sad = L[i].sad - T[k].rom;
						printf("内存分配成功！\n作业申请空间为%d\n起始地址为%d\n", T[k].rom, T[k].sad);
						break;
					}
					else
					{
						L[i].sad = L[i].sad + Size;
						L[i].rom = L[i].rom - Size;
						L[i].state = 2;
						T[k].sad = L[i].sad - Size;
						printf("内存分配成功！\n作业申请空间为%d\n起始地址为%d\n", L[i].rom, T[k].sad);
						break;
					}
				}
				else if (L[i].state = 0 && L[i].rom - T[k].rom == Size)
				{
					L[i].state = 2;
					T[k].sad = L[i].sad;
					printf("内存分配成功！\n作业申请空间为%d\n起始地址为%d\n", T[k].rom, T[k].sad);
					break;
				}
			}
			if (i>n)
			{
				printf("作业请求内存空间过大，空闲分区表不能满足要求，内存分配失败!\n");
				break;
			}
		}
		else if (tmp == 2)
		{
			recycle();
			sort(L + 1, L + n + 1, cmp);
		}
		else if (tmp == 3)
			print();
		else if (tmp == 0)
			return;
		else
		{
			printf("输入有误，请重新输入！\n");
			continue;
		}
	}
}
void init()
{
loop1:
	pos = 1;
	printf("请输入空闲分区表分区数量：\n");
	cin >> n;
	printf("请输入每个空闲分区的分区大小，分区始址\n");
	for (int i = 1; i <= n; i++)
	{
		printf("请输入第%d个分区的信息：\n", i);
		cin >> L[i].rom >> L[i].sad;
		L[i].id = i;
		L[i].state = 0;
	}
	printf("输入完毕，当前空闲分区表状态为：\n");
	print();
	printf("请输入不再切割的剩余空间的大小：\n");
	cin >> Size;
loop2:
	printf("选择内存分配的算法：1.循环首次适应算法  2.最佳适应算法  3.重新编辑空闲分区表  4.退出 \n");
	int tmp;
	cin >> tmp;
	if (tmp == 1)NF();
	else if (tmp == 2)BF();
	else if (tmp == 3)
	{
		goto loop1;
	}
	else if (tmp == 4) return;
	else
	{
		printf("输入有误，请重新输入!\n");
		goto loop2;
	}
}

int main()
{
	init();
	return 0;
}