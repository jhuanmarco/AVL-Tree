#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int info;
	int fb;
	struct node *left;
	struct node *right;
} NODE;


char mainMenu(){ //Menu principal
	char option;
	printf("\nAVL Tree\n\nType:\n1 - Create AVL\n2 - Add Node\n3 - Search Node\n4 - Del Node\n5 - Destroy AVL\n6 - Print AVL\n0 - Exit\n");
	scanf(" %c", &option);
	printf("\n");
	
	return option;
}

int returnInfo(){ //Para coletar a info do Nó
	int info;
	printf("Type Node Info: ");
	scanf(" %d", &info);
	
	return info; 
}

int altura (NODE *r) { //Altura da arvore, usado para comparar e gerar o FB (fator balanceamento
   if (r == NULL) 
      return -1; 
   else {
      int he = altura (r->left);
      int hd = altura (r->right);
      if (he < hd) return hd + 1;
      else return he + 1;
   }
}

void attAltura(NODE *root){ //Atualiza o fb da arvore
	root->fb = altura(root->left) - altura(root->right);
}
	
NODE *createNode(int info){ //Aloca espaço para o novo nó
	NODE *newNode = malloc(sizeof(NODE));
	if(!newNode){
		 printf("Error: No Memory");
		 return NULL;
	}
	
	newNode->info = info;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->fb = 0;
	printf("Node Created, Info = %d\n", newNode->info);
	
	return newNode;
}

void RSE(NODE **p){ 	//Rotação simples a esquerda
	NODE *no;
	no = (*p)->right;
	(*p)->right = no->left;
	no->left = (*p);
	attAltura(*p);
	attAltura(no);
	(*p) = no;
}

void RSD(NODE **p){ 	//Rotação simples a direita
	NODE *no;
	no = (*p)->left;
	(*p)->left = no->right;
	no->right = (*p);
	attAltura(*p);
	attAltura(no);
	(*p) = no;
}



void createNewNode(NODE **root, int *i, int info){ //Adiciona um novo nó recursivamente e rearranja a arvore
	if(*i == 1){
		if(info >= (*root)->info){
			if((*root)->right == NULL){
				(*root)->right = createNode(info);
				*i = 0;
			} else {
				createNewNode(&(*root)->right, i, info);			
			}
		
		} else if(info < (*root)->info){
			if((*root)->left == NULL){
				(*root)->left = createNode(info);
				*i = 0;
			} else {
				createNewNode(&(*root)->left, i, info);		
			}
		}
	
	}
	
	if(*i == 0){
		attAltura(*root);
		printf(" \nInfo = %d Altura = %d\n", (*root)->info,(*root)->fb);
		if((*root)->fb == -2){
			if((*root)->right->fb < 0){ //RSE
				RSE(root);
			
			}else{ //RDE
				RSD(&((*root)->right));
				RSE(root);
			
			}
	
		} else if((*root)->fb == 2){
			if((*root)->left->fb > 0){ //RSD
				RSD(root);
			
			}else{ //RDD
				RSE(&((*root)->left));
				RSD(root);
			
			}
	
		
		}
	
	}
}

void addNode(NODE **root){ //chama funcao principal de adicao de nó
	int info = returnInfo();
	int i = 1;
	createNewNode(root, &i, info);
}

void postOrder(NODE *walk){ //caminhamentos abaixo
	if(walk){
		postOrder(walk->left);
		postOrder(walk->right);
		printf("%d, FB = ", walk->info);
		printf("%d\n", walk->fb);
	}

	return;
}

void preOrder(NODE *walk){
	if(walk){
		printf("%d, FB = ", walk->info);
		printf("%d\n", walk->fb);
		preOrder(walk->left);
		preOrder(walk->right);
	}

	return;	
}

void inOrder(NODE *walk){
	if(walk){
		inOrder(walk->left);
		printf("%d, FB = ", walk->info);	
		printf("%d\n", walk->fb);
		inOrder(walk->right);
	} 

	return;
}

void destroyAVL(NODE **root){
	*root = NULL;
	printf("AVL Successful Destroyed\n");
	return;
}

int searchMatchesNode(NODE *root, int i){ //Procura nós
	int matches = 0; 

	if(!root) return 0;
	
	if(root->info == i) matches++;

	if(i < root->info){
		matches += searchMatchesNode(root->left, i);
	} else {
		matches += searchMatchesNode(root->right, i);
	}

	return matches;
}

void searchNode(NODE *root){
	int infoSearch, matches;
	
	printf("Enter The Info to Search: ");
	scanf(" %d", &infoSearch);

	matches = searchMatchesNode(root, infoSearch);
	if(matches == 0) {
		printf("There's no Match for this Info\n");
		return;
	}

	printf("We Found %d Matche's for this Info\n", matches);
	return;
}

NODE *retorna_maior(NODE **no){
	NODE *aux;
	aux = *no;
	if (aux->right==NULL){
		*no=(*no)->left;
		return (aux);
	}else
		return (retorna_maior(&((*no)->right)));
}

void exclui(NODE **no, int x){
    	int alt;
	NODE *aux;
	int resultado;
	        		
    if ((*no)->info==x){ //Encontrou
		aux=*no;
		// Se não tiver filho na esquerda 
		if ((*no)->left==NULL)
			*no=(*no)->right; //Então o filho da direita substitui 
		else 
		    // Se não tem filho a direita 
			if ((*no)->right==NULL) 
			   *no=(*no)->left; // então o filho da esquerda substitui 
   			else{ // Senão possui dois filhos 
			   aux=retorna_maior(&((*no)->left)); // Busca o substituto 
			   (*no)->info=aux->info; 
		    }
		free(aux); 
		return;
	}else{
		if (x<(*no)->info)
		    return (exclui(&((*no)->left),x)); 
		else
			return (exclui(&((*no)->right),x));
	}
}

void deleteNode(NODE **root){
	int nodeInfo, matches;
	
	printf("Enter The Node Info to Delete: ");
	scanf(" %d", &nodeInfo);
	matches = searchMatchesNode(*root, nodeInfo);
	if(matches == 0) { //if node dont exist
		printf("There's no match for this info\n");
		return;
	}
	exclui(root, nodeInfo);
}

void main(){
	char menu;
	NODE *root = NULL;	
	int info;
	
	do {
		menu = mainMenu();
		switch(menu){
			case '1':
				if(root){
					printf("AVL Root Already Created\n\n");
					break;
				}
				info = returnInfo();
				root = createNode(info);
				
			break;
			case '2':
				if(!root){
					printf("AVL Not Created, Go to Opt.1\n\n");
					break;
				}
				
				addNode(&root);
			
			break;
			case '3':
				if(!root){
					printf("AVL Not Created, Go to Opt.1\n\n");
					break;
				}	

				searchNode(root);
			break;
			case '4':
				if(!root){
					printf("AVL Not Created, Go to Opt.1\n\n");
					break;
				}	

				deleteNode(&root);
			break;
			case '5':
				if(!root){
					printf("AVL Not Created, Go to Opt.1\n\n");
					break;
				}

				destroyAVL(&root);
			break;
			case '6':
				if(root){
					printf("Tree Traversals:\nPre Order:\n");
					preOrder(root);	
					printf("\nIn Order:\n");
					inOrder(root);
					printf("\nPost Order:\n");
					postOrder(root);
				} else {
					printf("AVL Not Created, Go to Opt.1\n\n");
				}

			break;
			default:
			
			break;
		}
		
		

	} while(menu != '0');
}	
