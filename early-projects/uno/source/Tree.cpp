#include "Tree.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

Tree::Tree()
{

}
Tree::~Tree()
{

}

//创建树
CBTree *Tree::Create_tree()
{
	//创建一棵树
	CBTree *tree = (CBTree*)malloc(sizeof(CBTree) / sizeof(char));
	if (tree == NULL)
	{
		errno = MALLOC_ERROR;
		return NULL;
	}
	//创建头节点
	tree->head = (CBNode *)malloc(sizeof(CBNode) / sizeof(char));
	if (tree->head == NULL)
	{
		errno = MALLOC_ERROR;
		free(tree);
		return NULL;
	}
	tree->head->child = NULL;   //头节点的孩子指针为空，此时树为空
	tree->head->brother = NULL;   //头节点没有兄弟
	tree->len = 0;     //树节点个数为0（不算头节点）

	head = tree->head;
	return tree;
}


int Tree::Insert_Node(CBTree *tree, ElemType data,  int layer, int count)
{
	if (tree == NULL || count < 0)
	{
		errno = ERROR;
		return FALSE;
	}
	//创建树节点
	CBNode* node = (CBNode*)malloc(sizeof(CBNode) / sizeof(char));
	if (node == NULL)
	{
		errno = MALLOC_ERROR;
		return FALSE;
	}
	//初始化树节点，该节点的孩子和右兄弟都为空
	node->data = data;
	node->child = NULL;
	node->brother = NULL;
	if (count == 0)    //如果步数为0，则是插入的根节点
	{
		tree->head->child = node;
		tree->len++;  //插入成功，树节点+1
		return TRUE;
	}
	else
	{
		CBNode* tmp = tree->head;//指针指向头节点，开始找父节点位置
		for (int i = 0;i < layer;i++)
		{
			tmp = tmp->child;
			for (int j = 0;j < count;j++)
				tmp = tmp->brother;
		}
		//tmp指向的是要插入节点的父节点，tmp为空时，就越界
		if (tmp == NULL)
		{
			errno = OVER_ERROR;
			printf("插入越界\n");
			return FALSE;
		}
		if (tmp->child == NULL)
		{
			tmp->child = node;//插入节点作为该父节点的首孩子
		}
		else
		{
			if (node->WinValue >= tmp->child->WinValue)
			{
				CBNode* tmp2 = tmp->child;
				tmp->child = node;
				tmp->child->brother = tmp2;
			}
			else
			{
				tmp = tmp->child;
				if (node->WinValue < tmp->WinValue)
				{
					tmp = tmp->brother;
				}
				CBNode* tmp2 = tmp->brother;
				tmp->brother = node;
				tmp->brother->brother = tmp2;
			}
		}
		tree->len++;  //插入成功，树节点+1
	}
	return TRUE;
}


//递归打印
void Tree::r_display(CBNode *node, int gap)
{
	if (node == NULL)
	{
		return;
	}
	//打印距离第一个节点的距离
	int i;
	for (i = 0; i < gap;i++)
	{
		printf("-");
	}
	printf("%d %d\n", node->data.cColor,node->data.cNum);  //打印节点自己
	CBNode * child = node->child;  //该节点的第一个孩子
	//打印该节点的首孩子
	r_display(child, gap + 4);
	CBNode * brother = node->brother; //该节点的右兄弟
	//打印该节点的右兄弟
	r_display(brother, gap);
}

//打印树
void Tree::Display(CBTree *tree)
{
	if (tree == NULL)
	{
		errno = ERROR;
		return;
	}
	r_display(tree->head->child, 0);
}

//递归删除
void Tree::r_delete(CBTree* tree, CBNode* node)
{
	//该节点为空或既没有孩子也没有兄弟，就结束
	if (node == NULL || node->child == NULL && node->brother == NULL)
	{
		return;
	}
	CBNode* p = node->child;  //指向第一个孩子
	r_delete(tree, p);   //删除孩子的孩子
	if (p != NULL)
	{
		tree->len--;    //删除成功，树节点-1
	}
	free(p);             //删除孩子
	p = node->brother;   //指向右兄弟
	r_delete(tree, p);   //删除右兄弟的孩子
	if (p != NULL)
	{
		tree->len--;
	}
	free(p);           //删除右兄弟
}

//删除节点
// pos 代表要删除结点怎么走，首孩子为1，兄弟为0，如01为先首孩子，后兄弟
//count表示步数，从头节点（根节点的前一个节点）开始走。
int Tree::Delete(CBTree *tree, int pos, int count, ElemType *x)
{
	//不可以删头节点
	if (tree == NULL || x == NULL || count <= 0 || pos <= 0)
	{
		errno = ERROR;
		return FALSE;
	}
	//头节点没有孩子时，树为空
	if (tree->head->child == NULL)
	{
		errno = TREE_EMPTY;
		printf("此树为空\n");
		return FALSE;
	}
	//tmp指向头节点，开始找节点位置。
	CBNode *tmp = tree->head;
	int way;
	while (count > 1 && tmp != NULL) //先找到要删除的节点的前一个节点
	{
		way = pos & 1;
		pos = pos >> 1;
		if (way == CHILD)
		{
			tmp = tmp->child;
		}
		else
		{
			tmp = tmp->brother;
		}
		count--;
	}
	//此时tmp指向要删除节点的前一个，所以为空时，删除越界
	if (tmp == NULL)
	{
		errno = OVER_ERROR;
		printf("删除越界\n");
		return FALSE;
	}
	way = pos & 1;   //判断最后一步怎么走，若最后一步走向空，删除越界
	if (way == CHILD)
	{
		if (tmp->child == NULL)
		{
			errno = OVER_ERROR;
			printf("删除越界\n");
			return FALSE;
		}
	}
	else
	{
		if (tmp->brother == NULL)
		{
			errno = OVER_ERROR;
			printf("删除越界\n");
			return FALSE;
		}
	}
	//找到节点，开始删除
	if (way == CHILD)
	{
		CBNode* p = tmp->child;
		*x = p->data;
		tmp->child = p->brother;  //保留要删除节点的所有右兄弟
		p->brother = NULL;    //断开该节点与右兄弟的联系

		r_delete(tree, p);   //删除该节点的所有孩子
		free(p);  //删除该节点
		tree->len--;  //删除该节点成功，树节点个数-1
	}
	else
	{
		CBNode* p = tmp->brother;
		*x = p->data;
		tmp->brother = p->brother;
		p->brother = NULL;

		r_delete(tree, p);
		free(p);
		tree->len--;
	}
}

//求指定位置树节点的值
//pos代表要删除结点怎么走，首孩子为1，兄弟为0，如01为先首孩子，后兄弟
//count表示步数，从头节点（根节点的前一个节点）开始走。
int Tree::Tree_Get(CBTree *tree, int pos, int count, ElemType *x)
{
	//不可以查看头节点
	if (tree == NULL || x == NULL || count <= 0 || pos <= 0)
	{
		errno = ERROR;
		return FALSE;
	}
	//指针指向头节点，开始找节点位置
	CBNode *tmp = tree->head;
	int way;
	while (count > 0 && tmp != NULL)
	{
		way = pos & 1;
		pos = pos >> 1;
		if (way == CHILD)
		{
			tmp = tmp->child;
		}
		else
		{
			tmp = tmp->brother;
		}
		count--;
	}
	//tmp指向为空，查找的节点不存在，代表越界
	if (tmp == NULL)
	{
		errno = OVER_ERROR;
		printf("查看越界\n");
		return FALSE;
	}
	*x = tmp->data;
	return TRUE;

}

// 清空树中所有的节点
int Tree::Tree_Clear(CBTree* tree)
{
	if (tree == NULL)
	{
		errno = ERROR;
		return FALSE;
	}
	ElemType x;
	return Delete(tree, 1, 1, &x);   //删掉根节点
}

// 树的销毁 
void Tree::Tree_Destroy(CBTree* tree)
{
	if (tree == NULL)
	{
		errno = ERROR;
		return;
	}
	Tree_Clear(tree);   //清空树
	free(tree->head);   //释放掉头节点
	free(tree);   //释放掉树
}

//获取树根节点的地址
CBNode* Tree::Tree_Root(CBTree* tree)
{
	if (tree == NULL)
	{
		errno = ERROR;
		//return  ;
	}
	return tree->head->child;
}

//求树节点个数
int Tree::Tree_Count(CBTree* tree)
{
	if (tree == NULL)
	{
		errno = ERROR;
		//return ;
	}
	return tree->len;
}

//递归求高度
int Tree::r_height(CBNode *node, int gap)
{
	if (node == NULL)
	{
		return gap;
	}

	CBNode * child = node->child;  //该节点的第一个孩子
	int ret = r_height(child, gap + 1);   //该节点不为空，高度+1，返回该节点高度
	CBNode * brother = node->brother;   //该节点的右兄弟
	r_height(brother, gap);       //兄弟不增加高度
	return ret;  //返回该节点高度
}

//求树的高度
int Tree::Tree_Height(CBTree* tree)
{
	if (tree == NULL)
	{
		return FALSE;
	}
	int ret = r_height(tree->head->child, 0);
	return ret;
}

//递归求度
int Tree::r_degree(CBNode* node, int deg)
{
	int i = 0;
	if (node == NULL)
	{
		return deg;
	}
	CBNode* p = node->child;
	if (p != NULL)    //如果该节点的孩子不为空，初始度为1
	{
		i++;
	}
	int degtmp = r_degree(p, i);  //返回度
	p = node->brother;
	if (p != NULL)  //如果右兄弟不为空，度+1
	{
		deg++;
	}
	int tmpdeg = r_degree(p, deg);  //返回度
	//比较此处由孩子节点返回的度和兄弟节点返回的度，谁大就作为此处的度
	return (tmpdeg > degtmp ? tmpdeg : degtmp);
}

//求树的度
int Tree::Tree_Degree(CBTree* tree)
{
	if (tree == NULL || tree->head == NULL || tree->head->child == NULL)
	{
		return FALSE;
	}
	int deg = r_degree(tree->head->child, 0);
	return deg;
}
