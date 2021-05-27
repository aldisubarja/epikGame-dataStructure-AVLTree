#include <stdio.h>
#include <stdlib.h>
#include<string.h>

struct data{
	int value;
	char name[25];
	char genre[20];
	double price;
	int height;
	struct data *left, *right;
};

struct data *newData(int value,char name[],char genre[],double price){
	struct data *temp = (struct data*)malloc(sizeof(struct data));
	temp->value = value;
	strcpy(temp->name,name);
	strcpy(temp->genre,genre);
	temp->price = price; 
	temp->height = 1;
	temp->left = temp->right = NULL;
	return temp;
}

int getheight(struct data *root){
	if(!root) return 0;
	return root->height;
}

int max(int a, int b){
	if(a>b){
        return a;
    } 
    else{
        return b;
    } 
}

int getBalanceFactor(struct data *root){
	if(!root) return 0;
	return getheight(root->left) - getheight(root->right);
}

struct data *rightRotate(struct data *y){
	struct data *x = y->left;
	struct data *bwh = x->right;
	
	x->right = y;
	y->left = bwh;
	
	y->height = max(getheight(y->left), getheight(y->right)) + 1;
	x->height = max(getheight(x->left), getheight(x->right)) + 1;
	return x;
}

struct data *leftRotate(struct data *x){
	struct data *y = x->right;
	struct data *bwh = x->left;
	
	y->left = x;
	x->right = bwh;
	
	x->height = max(getheight(x->left), getheight(x->right)) + 1;
	y->height = max(getheight(y->left), getheight(y->right)) + 1;
	return y;
}

struct data *rebalance(struct data *root){
	root->height = max(getheight(root->left), getheight(root->right)) + 1;
	int currBalance = getBalanceFactor(root);
	
	if(currBalance > 1 && getBalanceFactor(root->left) >= 0) return rightRotate(root);
	if(currBalance > 1 && getBalanceFactor(root->left) < 0 ){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	
	if(currBalance < -1 && getBalanceFactor(root->right) <= 0) return leftRotate(root);
	if(currBalance < -1 && getBalanceFactor(root->right) > 0 ){
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}	
	return root;
	
}

struct data *insert(struct data *root, int value, char name[], char genre[], double price){
	if(!root)return newData(value,name,genre,price);
	else if(root->value > value) root->left = insert(root->left, value,name,genre,price);
	else if(root->value < value) root->right = insert(root->right, value,name,genre,price);
	else return root;
	
	return rebalance(root);
}

struct data *succar(struct data *root){
	while(root && root->left) root = root->left;
	return root;
}

struct data *deletedata(struct data *root, int value){
	if(!root)return root;
	else if(root->value > value) root->left = deletedata(root->left, value);
	else if(root->value < value) root->right = deletedata(root->right, value);
	else {
		if(!root->left && !root->right){
			free(root);
			root = NULL;
		}
		else if(root->left && !root->right){
			struct data *del = root->left;
			*root = *(root->left);
			free(del);
		}
		else if(!root->left && root->right){
			struct data *del = root->right;
			*root = *(root->right);
			free(del);
		}
		else{
			struct data *suc = succar(root->right);
			root->value = suc->value;
			root->right = deletedata(root->right, root->value);
		}
	}
	if(!root) return 0;
	return rebalance(root);
}

struct data *popAll(struct data *root){
	if(root != NULL){
		popAll(root->left);
		popAll(root->right);
		free(root);
	}
	return NULL;
}

int find(struct data *root,int value){
	if(!root) return 0;
	else if(root->value==value) return 1;
	else if(root->value > value){
		return find(root->left,value);
	}
	else if(root->value < value){
		return find(root->right,value);
	}
	else return 0;
}

int count(struct data *root,char genre[]){
	int ada=0;
	if(root){
		count(root->left,genre);
		if(strcmp(root->genre,genre)==0) ada+=1;
		count(root->right,genre);
	}
	return ada;
}


void inorder(struct data *root){
	if(root){
		inorder(root->left);
		printf("|%10d|%20s|%10s|%10.2f\n", root->value,root->name,root->genre,root->price);
		inorder(root->right);
	}
}

int main(){
	struct data *root=NULL;
	root=insert(root,3,"Warframe","Action",13.29);
	root=insert(root,77,"GTA 5","Action",50.00);
	root=insert(root,88,"Team Fortress 4","Action",6.79);
	root=insert(root,2,"Hades","RPG",16.16);
	root=insert(root,95,"Fallout 7","RPG",50.99);
	root=insert(root,10,"Last oasis","Survival",17.09);
	int choose;
	do{
		printf("|%10s|%20s|%10s|%10s\n","Game ID","Game Name","Genre","Game Price");
		inorder(root);
		printf("1. Add new Pre-Paid Game\n");
		printf("2. Delete Pre-Paid Game\n");
		printf("3. Game Genres Summary\n");
		printf("4. Quit\n");
		printf("Enter your choice : ");
		scanf("%d",&choose); getchar();
		if(choose==1){
			int error;
			int value;
			char name[25];
			char genre[20];
			double price;
			do{
				error=0;
				printf("Game ID[1-99]: ");
				scanf("%d",&value); getchar();
				error = find(root,value);
			}while(value<1||value>99||error>0);
			do{
				printf("Game Name[5-20 Characters]: ");
				scanf("%[^\n]",name); getchar();
			}while(strlen(name)<5||strlen(name)>20);
			do{
				error=0;
				printf("Genre[Action/RPG/Survival]: ");
				scanf("%s",genre); getchar();
				if(strcmp(genre,"Action")!=0) error+=1;
				if(strcmp(genre,"RPG")!=0) error+=1;
				if(strcmp(genre,"Survival")!=0) error+=1;
			}while(error==3);
			do{
				printf("Game Price[$5.00 - $1000.00]: $");
				scanf("%lf",&price);
			}while(price<5||price>1000);
			root = insert(root,value,name,genre,price);
		}else if(choose==2){
			if(root==NULL){
				printf("List game kosong");
			}else{
				int ketemu;
				int value;
				printf("|%10s|%20s|%10s|%10s\n","Game ID","Game Name","Genre","Game Price");
				inorder(root);
				do{
					ketemu=0;
					printf("Game ID[1-99]: ");
					scanf("%d",&value); getchar();
					ketemu = find(root,value);
				}while(value<1||value>99||ketemu==0);
				root = deletedata(root,value);
			}
		}else if(choose==3){
			int rpg=count(root,"RPG");
			int survival=count(root,"Survival");
			int action=count(root,"Action");
			printf("Genre\n");
			printf("========\n");
			printf("Action : %d\n",action);
			printf("RPG : %d\n",rpg);
			printf("Survival %d\n",survival);
			
		}else if(choose==4){
			root=popAll(root);
		}
	}while(choose!=4);
	
	return 0;
}
