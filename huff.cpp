#include<iostream>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#pragma warning(disable:4996)
#define N 100
#define M 2*N-1
typedef char * HuffmanCode[2 * M];//haffman����
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
	cout << "�γ̱����" << yc << endl;
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
	cout << "�γ������" << jieya << endl;
	out << "�γ������" << jieya << endl;
}
typedef struct
{
	int weight;//Ȩֵ
	int parent;//���ڽڵ�
	int LChild;//���ӽڵ�
	int RChild;//���ӽڵ�
}HTNode, Huffman[M + 1];//huffman��
typedef struct Node
{
	int weight; //Ҷ�ӽ���Ȩֵ
	char c; //Ҷ�ӽ��
	int num; //Ҷ�ӽ��Ķ�������ĳ���
}WNode, WeightNode[N];
/***����Ҷ�ӽ����ַ���Ȩֵ***/
void CreateWeight(char ch[], int *s, WeightNode CW, int *p)
{
	int i, j, k;
	int tag;
	*p = 0;//Ҷ�ӽڵ����
		   //ͳ���ַ����ָ���,����CW
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
					CW[*p].weight++;//Ȩֵ�ۼ�
		}
	}
	*s = i;//�ַ�������
}
/********����HuffmanTree********/
void CreateHuffmanTree(Huffman ht, WeightNode w, int n)
{
	int i, j;
	int s1, s2;
	//��ʼ����������
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
		s1 = j; //�ҵ���һ��˫��Ϊ��Ľ��
		for (; j <= i - 1; j++)
			if (!ht[j].parent)
				s1 = ht[s1].weight>ht[j].weight ? j : s1;
		ht[s1].parent = i;
		ht[i].LChild = s1;
		for (j = 1; j <= i - 1; j++)
			if (!ht[j].parent)
				break;
		s2 = j; //�ҵ��ڶ���˫��Ϊ��Ľ��
		for (; j <= i - 1; j++)
			if (!ht[j].parent)
				s2 = ht[s2].weight>ht[j].weight ? j : s2;
		ht[s2].parent = i;
		ht[i].RChild = s2;
		ht[i].weight = ht[s1].weight + ht[s2].weight;//Ȩֵ�ۼ�
	}
}
/***********Ҷ�ӽ��ı���***********/
void CrtHuffmanNodeCode(Huffman ht, char ch[], HuffmanCode h, WeightNode 	weight, int m, int n)
{
	int i, c, p, start;
	char *cd;
	cd = (char *)malloc(n * sizeof(char));
	cd[n - 1] = '\0';//ĩβ��
	for (i = 1; i <= n; i++)
	{
		start = n - 1; //cd��ÿ�δ�ĩβ��ʼ
		c = i;
		p = ht[i].parent;//p��n+1��n-1
		while (p) //�ظ��׷������,ֱ��Ϊ
		{
			start--;//������ǰ��ֵ
			if (ht[p].LChild == c)//��������ͬ,��
				cd[start] = '0';
			else//������
				cd[start] = '1';
			c = p;
			p = ht[p].parent;
		}
		weight[i].num = n - start; //��������ĳ���(����ĩβ)
		h[i] = (char *)malloc((n - start) * sizeof(char));
		strcpy(h[i], &cd[start]);//���������ַ���������ָ������h��
	}
	free(cd);//�ͷ�cd�ڴ�
	system("pause");
}
/*********�����ַ��ı���*********/
void CrtHuffmanCode(char ch[], HuffmanCode h, HuffmanCode hc, WeightNode 	weight, int n, int m)
{
	int i, k;
	for (i = 0; i<m; i++)
	{
		for (k = 1; k <= n; k++) /*��weight[k].c�в�����ch[i]��ȵ��±�K*/
			if (ch[i] == weight[k].c)
				break;
		hc[i] = (char *)malloc((weight[k].num) * sizeof(char));
		strcpy(hc[i], h[k]); //���������Ʊ���
	}
}
/*****����*****/
void TrsHuffmanTree(Huffman ht, WeightNode w, HuffmanCode hc, int n, int m)
{
	int i = 0, j, p;
	while (i<m)
	{
		p = 2 * n - 1;//�Ӹ��׽ڵ����±���ֱ��Ҷ�ӽڵ�
		for (j = 0; hc[i][j] != '\0'; j++)
		{
			if (hc[i][j] == '0')
				p = ht[p].LChild;
			else
				p = ht[p].RChild;
		}
		printf("%c", w[p].c); /*��ӡԭ��Ϣ*/
		out << w[p].c;
		i++;
	}
	out << endl;
}
/*****�ͷ�huffman�����ڴ�*****/
void FreeHuffmanCode(HuffmanCode h, HuffmanCode hc, int n, int m)
{
	int i;
	for (i = 1; i <= n; i++)//�ͷ�Ҷ�ӽ��ı���
		free(h[i]);
	for (i = 0; i<m; i++) //�ͷ����н��ı���
		free(hc[i]);
}
int n; /*nΪҶ�ӽ��ĸ���*/
int m; /*mΪ�ַ���ch[]�ĳ���*/
Huffman ht; /*Huffman������*/
HuffmanCode h, hc; /*h���Ҷ�ӽ��ı��룬hc ������н��ı���*/
WeightNode weight; /*���Ҷ�ӽ�����Ϣ*/
void huffmanbm(char *ch)
{
	n = 0;
	int i;
	m = 0;
	printf("\t***HuffmanCoding***\n");
	CreateWeight(ch, &m, weight, &n); /*����Ҷ�ӽ����Ϣ��mΪ�ַ���ch[]�ĳ���
									  */
	printf("***WeightInformation***\n Node ");
	for (i = 1; i <= n; i++) //���Ҷ�ӽ����ַ���Ȩֵ
		printf("%c ", weight[i].c);
	printf("\nWeight ");
	for (i = 1; i <= n; i++)
		printf("%d ", weight[i].weight);
	CreateHuffmanTree(ht, weight, n); /*����Huffman��*/
	printf("\n***HuffamnTreeInformation***\n");
	printf("\ti\tweight\tparent\tLChild\tRChild\n");
	for (i = 1; i <= 2 * n - 1; i++) //��ӡHuffman������Ϣ
		printf("\t%d\t%d\t%d\t%d\t%d\n", i, ht[i].weight, ht[i].parent, ht[i].LChild, ht[i].RChild);
	CrtHuffmanNodeCode(ht, ch, h, weight, m, n); /*Ҷ�ӽ��ı���*/
	printf(" ***NodeCode***\n"); /*��ӡҶ�ӽ��ı���*/
	for (i = 1; i <= n; i++)
	{
		printf("\t%c:", weight[i].c);
		printf("%s\n", h[i]);
	}
	CrtHuffmanCode(ch, h, hc, weight, n, m); /*�����ַ��ı���*/
	printf("Huffman�����"); /*��ӡ�ַ����ı���*/
	for (i = 0; i < m; i++)
	{
		printf("%s", hc[i]);
		strcpy(&y[i], hc[i]);
	}
	system("pause");
}
void huffmanyima()
{
	printf("huffman�����");
	TrsHuffmanTree(ht, weight, hc, n, m); /*����*/
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
		out << "�γ̱����" << x << endl;
		huffmanbm(x);
		out << "Huffman�����" << y << endl;
		out << "huffman�����";
		huffmanyima();
		youchengyima(x);
	}
	in.close();
	out.close();
	return 0;
}
