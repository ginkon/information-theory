#include<iostream>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#pragma warning(disable:4996)
#define N 100
#define M 2*N-1
typedef char * HuffmanCode[2 * M];//haffman编码
using namespace std;
char x[100];
char y[100];
fstream in("in5.txt");
fstream out("out5.txt");
void youchengbianma(char a[100])
{
	char yc[100];
	int i = 0, j = 0, jishu = 1;
	yc[0] = a[0];
	for (i = 0; a[i] != '\0'; i++)
	{
		if (a[i] == a[i + 1])
			jishu++;
		else
		{
			yc[j + 1] = jishu + 48;
			j = j + 2;
			yc[j] = a[i + 1];
			jishu = 1;
		}
	}
	yc[j] = '\0';
	cout << "游程编码后：" << yc << endl;
	strcpy_s(x, yc);
}
void youchengyima(char a[100])
{
	char bz = 0, jieya[100], j, jishu;
	for (int i = 0; a[i] != '\0'; i = i + 2)
	{
		jieya[bz] = a[i];
		for (j = bz, jishu = 1; jishu <= a[i + 1] - 48; jishu++, j++)
			jieya[j] = a[i];
		bz = j;
	}
	jieya[j] = '\0';
	cout << "游程译码后：" << jieya << endl;
	out << "游程译码后：" << jieya << endl;
}
typedef struct
{
	int weight;//权值
	int parent;//父节节点
	int LChild;//左子节点
	int RChild;//右子节点
}HTNode, Huffman[M + 1];//huffman树
typedef struct Node
{
	int weight; //叶子结点的权值
	char c; //叶子结点
	int num; //叶子结点的二进制码的长度
}WNode, WeightNode[N];
/***产生叶子结点的字符和权值***/
void CreateWeight(char ch[], int *s, WeightNode CW, int *p)
{
	int i, j, k;
	int tag;
	*p = 0;//叶子节点个数
		   //统计字符出现个数,放入CW
	for (i = 0; ch[i] != '\0'; i++)
	{
		tag = 1;
		for (j = 0; j<i; j++)
			if (ch[j] == ch[i])
			{
				tag = 0;
				break;
			}
		if (tag)
		{
			CW[++*p].c = ch[i];
			CW[*p].weight = 1;
			for (k = i + 1; ch[k] != '\0'; k++)
				if (ch[i] == ch[k])
					CW[*p].weight++;//权值累加
		}
	}
	*s = i;//字符串长度
}
/********创建HuffmanTree********/
void CreateHuffmanTree(Huffman ht, WeightNode w, int n)
{
	int i, j;
	int s1, s2;
	//初始化哈夫曼树
	for (i = 1; i <= n; i++)
	{
		ht[i].weight = w[i].weight;
		ht[i].parent = 0;
		ht[i].LChild = 0;
		ht[i].RChild = 0;
	}
	for (i = n + 1; i <= 2 * n - 1; i++)
	{
		ht[i].weight = 0;
		ht[i].parent = 0;
		ht[i].LChild = 0;
		ht[i].RChild = 0;
	}
	for (i = n + 1; i <= 2 * n - 1; i++)
	{
		for (j = 1; j <= i - 1; j++)
			if (!ht[j].parent)
				break;
		s1 = j; //找到第一个双亲为零的结点
		for (; j <= i - 1; j++)
			if (!ht[j].parent)
				s1 = ht[s1].weight>ht[j].weight ? j : s1;
		ht[s1].parent = i;
		ht[i].LChild = s1;
		for (j = 1; j <= i - 1; j++)
			if (!ht[j].parent)
				break;
		s2 = j; //找到第二个双亲为零的结点
		for (; j <= i - 1; j++)
			if (!ht[j].parent)
				s2 = ht[s2].weight>ht[j].weight ? j : s2;
		ht[s2].parent = i;
		ht[i].RChild = s2;
		ht[i].weight = ht[s1].weight + ht[s2].weight;//权值累加
	}
}
/***********叶子结点的编码***********/
void CrtHuffmanNodeCode(Huffman ht, char ch[], HuffmanCode h, WeightNode 	weight, int m, int n)
{
	int i, c, p, start;
	char *cd;
	cd = (char *)malloc(n * sizeof(char));
	cd[n - 1] = '\0';//末尾置
	for (i = 1; i <= n; i++)
	{
		start = n - 1; //cd串每次从末尾开始
		c = i;
		p = ht[i].parent;//p在n+1至n-1
		while (p) //沿父亲方向遍历,直到为
		{
			start--;//依次向前置值
			if (ht[p].LChild == c)//与左子相同,置
				cd[start] = '0';
			else//否则置
				cd[start] = '1';
			c = p;
			p = ht[p].parent;
		}
		weight[i].num = n - start; //二进制码的长度(包含末尾)
		h[i] = (char *)malloc((n - start) * sizeof(char));
		strcpy(h[i], &cd[start]);//将二进制字符串拷贝到指针数组h中
	}
	free(cd);//释放cd内存
	system("pause");
}
/*********所有字符的编码*********/
void CrtHuffmanCode(char ch[], HuffmanCode h, HuffmanCode hc, WeightNode 	weight, int n, int m)
{
	int i, k;
	for (i = 0; i<m; i++)
	{
		for (k = 1; k <= n; k++) /*从weight[k].c中查找与ch[i]相等的下标K*/
			if (ch[i] == weight[k].c)
				break;
		hc[i] = (char *)malloc((weight[k].num) * sizeof(char));
		strcpy(hc[i], h[k]); //拷贝二进制编码
	}
}
/*****解码*****/
void TrsHuffmanTree(Huffman ht, WeightNode w, HuffmanCode hc, int n, int m)
{
	int i = 0, j, p;
	while (i<m)
	{
		p = 2 * n - 1;//从父亲节点向下遍历直到叶子节点
		for (j = 0; hc[i][j] != '\0'; j++)
		{
			if (hc[i][j] == '0')
				p = ht[p].LChild;
			else
				p = ht[p].RChild;
		}
		printf("%c", w[p].c); /*打印原信息*/
		out << w[p].c;
		i++;
	}
	out << endl;
}
/*****释放huffman编码内存*****/
void FreeHuffmanCode(HuffmanCode h, HuffmanCode hc, int n, int m)
{
	int i;
	for (i = 1; i <= n; i++)//释放叶子结点的编码
		free(h[i]);
	for (i = 0; i<m; i++) //释放所有结点的编码
		free(hc[i]);
}
int n; /*n为叶子结点的个数*/
int m; /*m为字符串ch[]的长度*/
Huffman ht; /*Huffman二叉数*/
HuffmanCode h, hc; /*h存放叶子结点的编码，hc 存放所有结点的编码*/
WeightNode weight; /*存放叶子结点的信息*/
void huffmanbm(char *ch)
{
	n = 0;
	int i;
	m = 0;
	printf("\t***HuffmanCoding***\n");
	CreateWeight(ch, &m, weight, &n); /*产生叶子结点信息，m为字符串ch[]的长度
									  */
	printf("***WeightInformation***\n Node ");
	for (i = 1; i <= n; i++) //输出叶子结点的字符与权值
		printf("%c ", weight[i].c);
	printf("\nWeight ");
	for (i = 1; i <= n; i++)
		printf("%d ", weight[i].weight);
	CreateHuffmanTree(ht, weight, n); /*产生Huffman树*/
	printf("\n***HuffamnTreeInformation***\n");
	printf("\ti\tweight\tparent\tLChild\tRChild\n");
	for (i = 1; i <= 2 * n - 1; i++) //打印Huffman树的信息
		printf("\t%d\t%d\t%d\t%d\t%d\n", i, ht[i].weight, ht[i].parent, ht[i].LChild, ht[i].RChild);
	CrtHuffmanNodeCode(ht, ch, h, weight, m, n); /*叶子结点的编码*/
	printf(" ***NodeCode***\n"); /*打印叶子结点的编码*/
	for (i = 1; i <= n; i++)
	{
		printf("\t%c:", weight[i].c);
		printf("%s\n", h[i]);
	}
	CrtHuffmanCode(ch, h, hc, weight, n, m); /*所有字符的编码*/
	printf("Huffman编码后"); /*打印字符串的编码*/
	for (i = 0; i < m; i++)
	{
		printf("%s", hc[i]);
		strcpy(&y[i], hc[i]);
	}
	system("pause");
}
void huffmanyima()
{
	printf("huffman译码后：");
	TrsHuffmanTree(ht, weight, hc, n, m); /*解码*/
	FreeHuffmanCode(h, hc, n, m);
	system("pause");
}
int main()
{
	char s[100];
	if (!in.is_open())
	{
		cout << "Error opening file"; exit(1);
	}
	while (!in.eof()) {
		in.getline(s, 100);
		cout << s << endl;
		youchengbianma(s);
		out << "游程编码后：" << x << endl;
		huffmanbm(x);
		out << "Huffman编码后：" << y << endl;
		out << "huffman译码后：";
		huffmanyima();
		youchengyima(x);
	}
	in.close();
	out.close();
	return 0;
}
