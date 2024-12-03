#include<stdio.h>
#include<stdlib.h>

// Shafalda Bilqis Sceishar - 2602186775

enum colour {RED,BLACK};

struct data
{
	int value;
	enum colour color;
	struct data *left,*right,*parent;
}*root = NULL;

// function untuk menginput value ke dalam node
struct data *newnode(int value)
{
	struct data *temp = (struct data*)malloc(sizeof(struct data));
	temp->value = value;
	temp->color = RED;
	temp->parent = temp->left = temp->right = NULL;
	return temp;
}

// memutar tree / subtree ke arah kiri
void left_rotate(struct data **root,struct data *temp)
{
	struct data *right_child = temp->right;
	temp->right = right_child->left;
	if(right_child->left != NULL)
	{
		right_child->left->parent = temp;
	}
	right_child->parent = temp->parent;
	if(temp->parent == NULL) *root = right_child;
	else if(temp == temp->parent->left) temp->parent->left = right_child;
	else temp->parent->right = right_child;
	right_child->left = temp;
	temp->parent = right_child;
	
}

// memutar tree / subtree ke arah kanan
void right_rotate(struct data **root,struct data *temp)
{
	struct data *left_child = temp->left;
	temp->left = left_child->right;
	if(left_child->right != NULL)
	{
		left_child->right->parent = temp;
	}
	left_child->parent = temp->parent;
	if(temp->parent == NULL) *root = left_child;
	else if(temp == temp->parent->left) temp->parent->left = left_child;
	else temp->parent->right = left_child;
	left_child->right = temp;
	temp->parent = left_child;
}

// function jika data yang dimasukkan setelahnya melanggar aturan red black tree
void fix_up(struct data **root,struct data *temp)
{
	// jika value yang dimasukkan bukan root dan parent berwarna merah
	while(temp!=*root && temp->parent->color==RED)
	{
		// parent adalah left child dari grand parent newnode
		if(temp->parent == temp->parent->parent->left)
		{
			struct data *uncle = temp->parent->parent->right; // saudara kanan parent = uncle
			
			if(uncle!=NULL && uncle->color == RED) // jika uncle berwarna merah
			{
				temp->parent->color = uncle->color = BLACK;
				temp->parent->parent->color = RED;
				temp = temp->parent->parent;
			}
			else // jika uncle masih NULL atau berwarna hitam
			{
				if(temp == temp->parent->right)
				{
					temp = temp->parent;
					left_rotate(root,temp);
				}
				temp->parent->color = BLACK;
				temp->parent->parent->color = RED;
				right_rotate(root,temp->parent->parent);
			}
		}
		// parent adalah right child dari grand parent newnode
		else
		{
			struct data *uncle = temp->parent->parent->left; // saudara kiri parent = uncle
			if(uncle!=NULL && uncle->color == RED) // jika uncle berwarna merah
			{
				temp->parent->color = uncle->color = BLACK;
				temp->parent->parent->color = RED;
				temp = temp->parent->parent;
			}
			else // jika uncle masih NULL atau berwarna hitam
			{
				if(temp == temp->parent->left)
				{
					temp = temp->parent;
					right_rotate(root,temp);
				}
				temp->parent->color = BLACK;
				temp->parent->parent->color = RED;
				left_rotate(root,temp->parent->parent);
			}
		}
	}
	// warna root akan selalu hitam
	(*root)->color = BLACK;
}

// insert data ke dalam tree
struct data *insert(struct data **root, int value)
{
	struct data *temp = newnode(value);
	struct data *parent = NULL;
	struct data *curr = *root;
	
	// memposisikan newnode ke dalam tree
	while(curr != NULL)
	{
		parent = curr;
		if(temp->value < curr->value) curr = curr->left;
		else if(temp->value > curr->value) curr = curr->right;
		else // jika value newnode sudah ada dalam tree
		{
			free(temp);
			return *root;
		}
	}
	
	temp->parent = parent;
	
	if(parent == NULL) *root = temp;
	else if(temp->value < parent->value) parent->left = temp;
	else if(temp->value > parent->value) parent->right = temp;
	
	fix_up(root,temp);
}

// function untuk mengeluarkan output format inorder
void inorder(struct data *root)
{
	if(root != NULL)
	{
		inorder(root->left);
		printf("%d ",root->value);
		inorder(root->right);
	}
}

int main()
{
	// inisiasi value
	int val[] = {41,22,5,51,48,29,18,21,45,3};
	
	// memasukkan value ke dalam red black tree
	for(int i=0; i<10; i++)
	{
		insert(&root,val[i]); 
	}
	
	// output
	printf("Inorder Traversal of Created Tree:\n");
	inorder(root);
	
	return 0;
}
