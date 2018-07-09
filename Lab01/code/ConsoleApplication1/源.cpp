#include<iostream>  
#include<cmath>  
#include<stdio.h>  
#include<algorithm>  
#include<string.h>  
using namespace std;
int pos, n, Size; ///����λ��,��������,��С�ָ��С  
struct List
{
	int id;///���з������  
	int sad;///���з����׵�ַ  
	int rom;///�ռ�  
	int state;///״̬��0Ϊ�գ�1Ϊδ����2Ϊ����  
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
	printf("|       ������    ����ʼַ    ������С    ����״̬       |\n");
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
	printf("������Ҫ�ͷ�ռ�ÿռ��������ţ�\n");
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
			if (L[j].sad>T[l].sad + T[l].rom)///ʵ���ϴ���T[l].sad�Ϳ���  
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
	cout << "�ڴ������ϣ�" << endl;
}
void NF()///ѭ���״���Ӧ�㷨  
{
	cout << "ѡ�����'ѭ���״���Ӧ�㷨'�����ڴ����\n" << endl;
	int tmp;
	pos = 1;///��ʼ�ӵ�һ����������  
	while (1)
	{
		cout << "      *******************************************" << endl;
		cout << "      1: �����ڴ�             2: �����ڴ�        " << endl;
		cout << endl;
		cout << "      3: �鿴���з�����       0: �˳�       " << endl;
		cout << "      *******************************************" << endl;
		cout << "���������Ĳ��� ��";
		cin >> tmp;
		int k = 0;
		if (tmp == 1)
		{
			k++;
			printf("�������%d����ҵռ�ÿռ��С��\n", k);
			cin >> T[k].rom;
			T[k].id = k;
			int num = 0;
			for (int i = pos;; i++)
			{
				num++;
				if (num>n)
				{
					printf("��ҵ�����ڴ�ռ���󣬿��з�����������Ҫ���ڴ����ʧ��!\n");
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
						printf("�ڴ����ɹ���\n��ҵ����ռ�Ϊ%d\n��ʼ��ַΪ%d\n", T[k].rom, T[k].sad);
						break;
					}
					else
					{
						L[i].sad = L[i].sad + Size;
						L[i].rom = L[i].rom - Size;
						L[i].state = 2;
						T[k].sad = L[i].sad - Size;
						printf("�ڴ����ɹ���\n��ҵ����ռ�Ϊ%d\n��ʼ��ַΪ%d\n", L[i].rom, T[k].sad);
						break;
					}
				}
				else if (L[i].state = 0 && L[i].rom - T[k].rom == Size)
				{
					L[i].state = 2;
					T[k].sad = L[i].sad;
					printf("�ڴ����ɹ���\n��ҵ����ռ�Ϊ%d\n��ʼ��ַΪ%d\n", T[k].rom, T[k].sad);
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
			printf("�����������������룡\n");
			continue;
		}
	}
}
void BF()///�����Ӧ�㷨  
{
	cout << "ѡ�����'�����Ӧ�㷨'�����ڴ����\n" << endl;
	sort(L + 1, L + n + 1, cmp);
	int tmp;
	while (1)
	{
		cout << "      *******************************************" << endl;
		cout << "      1: �����ڴ�             2: �����ڴ�        " << endl;
		cout << endl;
		cout << "      3: �鿴���з�����       0: �˳�       " << endl;
		cout << "      *******************************************" << endl;
		cout << "���������Ĳ��� ��";
		cin >> tmp;
		int k = 0;
		if (tmp == 1)
		{
			k++;
			printf("�������%d����ҵռ�ÿռ��С��\n", k);
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
						printf("�ڴ����ɹ���\n��ҵ����ռ�Ϊ%d\n��ʼ��ַΪ%d\n", T[k].rom, T[k].sad);
						break;
					}
					else
					{
						L[i].sad = L[i].sad + Size;
						L[i].rom = L[i].rom - Size;
						L[i].state = 2;
						T[k].sad = L[i].sad - Size;
						printf("�ڴ����ɹ���\n��ҵ����ռ�Ϊ%d\n��ʼ��ַΪ%d\n", L[i].rom, T[k].sad);
						break;
					}
				}
				else if (L[i].state = 0 && L[i].rom - T[k].rom == Size)
				{
					L[i].state = 2;
					T[k].sad = L[i].sad;
					printf("�ڴ����ɹ���\n��ҵ����ռ�Ϊ%d\n��ʼ��ַΪ%d\n", T[k].rom, T[k].sad);
					break;
				}
			}
			if (i>n)
			{
				printf("��ҵ�����ڴ�ռ���󣬿��з�����������Ҫ���ڴ����ʧ��!\n");
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
			printf("�����������������룡\n");
			continue;
		}
	}
}
void init()
{
loop1:
	pos = 1;
	printf("��������з��������������\n");
	cin >> n;
	printf("������ÿ�����з����ķ�����С������ʼַ\n");
	for (int i = 1; i <= n; i++)
	{
		printf("�������%d����������Ϣ��\n", i);
		cin >> L[i].rom >> L[i].sad;
		L[i].id = i;
		L[i].state = 0;
	}
	printf("������ϣ���ǰ���з�����״̬Ϊ��\n");
	print();
	printf("�����벻���и��ʣ��ռ�Ĵ�С��\n");
	cin >> Size;
loop2:
	printf("ѡ���ڴ������㷨��1.ѭ���״���Ӧ�㷨  2.�����Ӧ�㷨  3.���±༭���з�����  4.�˳� \n");
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
		printf("������������������!\n");
		goto loop2;
	}
}

int main()
{
	init();
	return 0;
}