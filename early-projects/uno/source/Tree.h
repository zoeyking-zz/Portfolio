#pragma once
#include"Card.h"

#define CHILD 1		//只出一张牌，最终只要一个孩子
#define BROTHER 0
#define MAX_TREE_NUM 900        //定义树的最大结点数目

//树专属卡牌信息结构体
struct TreeCardInfo
{
	CARDCOLOR cColor;
	CARDTYPE cType;
	//CARDSCORE cScore;
	int weight;
};

//typedef struct TreeCardInfo	 ElemType;          //定义结点中的数据类型
typedef struct CardInfo	 ElemType;          //定义结点中的数据类型

//定义一个孩子-兄弟存储结构表示树
typedef struct CBNode
{
	ElemType data;                      //结点中存放的数据
	int WinValue = 25;				//胜率 最后要不要选择这个孩子的依据
	//CBNode *parent;
	CBNode *child;                  //指向从左边起，第一个孩子结点，没有孩子的结点指向NULL
	CBNode *brother;                //指向第一个孩子的右边的一个兄弟结点的指针，如果含有多个兄弟结点，依次指过去（类似单链表）
}CBNode;

typedef struct CBTree
{
	CBNode *head;             //指向头节点的指针
	int len;                  //当前树中结点数目
}CBTree;



class Tree
{
public:

	Tree();
	~Tree();

	CBNode *head;
	CBTree *cbTree;
	CBTree *Create_tree();												//创建树
	//插入结点
	int Insert_Node(CBTree *tree, ElemType data, int layer, int count);
	//在layer层第count个结点插入													
	//flag表示是作为前一个节点的的首孩子还是兄弟
	
	//选出第deep层胜率最低的子节点（该层最后的结点）
	CBNode *ChiceWorstChild(int deep);										
	void Display(CBTree *tree);											//打印树
	void r_display(CBNode *node, int gap);								//递归打印
	int Delete(CBTree *tree, int pos, int count, ElemType *x);			//删除节点
																		// pos 代表要删除结点怎么走，首孩子为1，兄弟为0，如01为先首孩子，后兄弟
																		//count表示步数，从头节点（根节点的前一个节点）开始走。
	void r_delete(CBTree* tree, CBNode* node);							//递归删除
	int Tree_Clear(CBTree* tree);										// 清空树中所有的节点
	void Tree_Destroy(CBTree* tree);// 树的销毁 
	int Tree_Get(CBTree *tree, int pos, int count, ElemType *x);		//求指定位置树节点的值
																		// pos 代表要删除结点怎么走，首孩子为1，兄弟为0，如01为先首孩子，后兄弟
																		//count表示步数，从头节点（根节点的前一个节点）开始走。
	CBNode* Tree_Root(CBTree* tree);	//获取树根节点的地址
	int Tree_Count(CBTree* tree);	//求树节点个数
	int Tree_Height(CBTree* tree);	//求树的高度
	int r_height(CBNode *node, int gap);//递归求高度
	int Tree_Degree(CBTree* tree);	//求树的度
	int r_degree(CBNode* node, int deg);//递归求度
	
};

