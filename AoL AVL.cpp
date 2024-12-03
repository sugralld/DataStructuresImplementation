#include<stdio.h>
#include<stdlib.h>

// Shafalda Bilqis Sceishar - 2602186775

int choice; // integer untuk menginput menu
int flag; // integer untuk menandakan apakah data ditemukan atau tidak (deletion)

struct data
{
	int value;
	struct data *left, *right;
	int height;
}*root = NULL;

// function untuk mengambil height (validasi tree)
int height(struct data *root)
{
	if(root == NULL) return 0;
	return root->height;
}

// function untuk mendapatkan height maksimal
int max(int a, int b)
{
	return (a > b) ? a:b;
}

// function untuk mendapat selisih subtree kiri dan kanan (balance atau tidak)
int get_balance(struct data *root)
{
	if(root == NULL) return 0;
	return (height(root->left) - height(root->right));
}

// function untuk menginput value ke dalam node
struct data *new_node(struct data *root, int value)
{
	struct data *latest = (struct data*)malloc(sizeof(struct data));
	latest->value = value;
	latest->height = 1;
	latest->left = latest->right = NULL;
	return latest;
}

// memutar tree / subtree ke arah kiri
struct data *left_rotate(struct data *root)
{
	struct data *x = root->right;
	struct data *sub_x = x->left;
	x->left = root;
	root->right = sub_x;
	
	// update height root & x
	root->height = 1 + max(height(root->left),height(root->right));
	x->height = 1 + max(height(x->left),height(x->right));
	
	return x;
}

// memutar tree / subtree ke arah kanan
struct data *right_rotate(struct data *root)
{
	struct data *x = root->left;
	struct data *sub_x = x->right;
	x->right = root;
	root->left = sub_x;
	
	// update height root & x
	root->height = 1 + max(height(root->left),height(root->right));
	x->height = 1 + max(height(x->left),height(x->right));
	
	return x;
}

// insert data ke dalam tree
struct data *insert(struct data *root, int value)
{
	// jika data masih kosong, langsung masukkan valuenya
	if(root == NULL) return new_node(root,value);
	
	// jika value data yang ingin dimasukkan lebih besar dari current node
	if(value > root->value)
	{
		// pindah ke subtree kanan
		root->right = insert(root->right,value);
	}
	
	// jika value data yang ingin dimasukkan lebih kecil dari current node
	else if(value < root->value)
	{
		// pindah ke subtree kiri
		root->left = insert(root->left,value);
	}
	
	// jika data sudah ada dalam tree
	else return root;
	
	// menghitung height
	root->height = 1 + max(height(root->left),height(root->right));
	
	int balance = get_balance(root);
	
	// jika tree belum balance
	if(balance > 1)
	{
		// left left case
		if(value < root->left->value)
		{
			return right_rotate(root);
		}
		
		// left right case
		else if(value > root->left->value)
		{
			root->left = left_rotate(root->left);
			return right_rotate(root);
		}
	}
	else if(balance < -1)
	{
		// right left case
		if(value < root->right->value)
		{
			root->right = right_rotate(root->right);
			return left_rotate(root);
		}
		
		// right right case
		else if(value > root->right->value)
		{
			return left_rotate(root);
		}
	}
	
	return root;
};

// output tree dalam format preorder
void pre_order(struct data *root)
{
	if(root != NULL)
	{
		printf(" %d",root->value);
		pre_order(root->left);
		pre_order(root->right);
	}
}

// output tree dalam format inorder
void in_order(struct data *root)
{
	if(root != NULL)
	{
		in_order(root->left);
		printf(" %d",root->value);
		in_order(root->right);
	}
}

// output tree dalam format postorder
void post_order(struct data *root)
{
	if(root != NULL)
	{
		post_order(root->left);
		post_order(root->right);
		printf(" %d",root->value);
	}
}

// mencari node dengan value terbesar pada subtree kiri
struct data *predecessor(struct data *root)
{
	struct data *curr = root;
	while(curr->right != NULL)
	{
		curr = curr->right;
	}
	return curr;
}

// mencari node dengan value terkecil pada subtree kanan
struct data *successor(struct data *root)
{
	struct data *curr = root;
	while(curr->left != NULL)
	{
		curr = curr->left;
	}
	return curr;
}

// untuk menghapus suatu data
struct data *del(struct data *root,int value)
{
	flag = 0; // inisiasi awal ada atau tidaknya data
	
	// jika data tidak ditemukan
	if(root == NULL)
	{
		flag = 1;
		return NULL;
	}
	
	// jika value data yang ingin dihapus lebih kecil dari current node
	if(value < root->value) root->left = del(root->left,value);
	
	// jika value data yang ingin dihapus lebih besar dari current node
	else if(value > root->value) root->right = del(root->right,value);
	
	// jika data berhasil ditemukan dalam tree
	else
	{
		// kondisi ini dilakukan jika data yang dihapus memiliki satu anak atau tidak punya anak
		if(root->left == NULL || root->right == NULL)
		{
			struct data *temp = NULL;
			if(root->left == NULL) temp = root->right; // node punya anak pada subtree kanan
			else temp = root->left; // node punya anak pada subtree kiri
			
			// jika node yang ingin dihapus tidak memiliki anak
			if(temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			
			// jika node yang ingin dihapus hanya memiliki 1 anak
			else
			{
				*root = *temp;
			}
			
			free(temp);
		}
		
		// jika node yang dihapus punya 2 anak
		else
		{
			struct data *temp = successor(root->right); // node yang ingin dihapus direplace dengan successornya
			root->value = temp->value;
			root->right = del(root->right,temp->value);
		}
	}
	
	if(root == NULL) return root;
	
	// menghitung height
	root->height = 1 + max(height(root->left),height(root->right));
	
	int balance = get_balance(root);
	
	// jika tree belum balance
	if(balance > 1)
	{
		// left left case
		if(get_balance(root->left)>=0)
		{
			return right_rotate(root);
		}
		
		// left right case
		else if(get_balance(root->left)<0)
		{
			root->left = left_rotate(root->left);
			return right_rotate(root);
		}
	}
	else if(balance < -1)
	{
		// right left case
		if(get_balance(root->right)>0)
		{
			root->right = right_rotate(root->right);
			return left_rotate(root);
		}
		
		// right right case
		else if(get_balance(root->right)<=0)
		{
			return left_rotate(root);
		}
	}
	return root;
}

void menu()
{
	system("cls");
	printf("1. Insertion\n2. Deletion\n3. Transversal\n4. Exit\n");
	do
	{
		printf("Choose: "); scanf("%d",&choice); fflush(stdin);
	}while(choice<1 || choice>4);
}

void menu_insert()
{
	int val;
	printf("Insert: ");
	scanf("%d",&val); fflush(stdin);
	root = insert(root,val);
}

void menu_delete()
{
	int val;
	printf("Delete: "); scanf("%d",&val); fflush(stdin);
	root = del(root,val);
	if(flag == 0) printf("Data found\nValue %d was deleted\n",val);
	else printf("Data not found\n");
	getchar();
}

void menu_tranversal()
{
	printf("Preorder:");
	pre_order(root);
	printf("\n");
	printf("Inorder:");
	in_order(root);
	printf("\n");
	printf("Postorder:");
	post_order(root);
	printf("\n");
	getchar();
}

int main()
{
	do
	{
		menu();
		switch(choice)
		{
			case 1:
				menu_insert();
				break;
			case 2:
				menu_delete();
				break;
			case 3:
				menu_tranversal();
				break;
		}
	}while(choice != 4);
	printf("thanks!!! have a great day =]\n");
	return 0;
}
