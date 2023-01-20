#define _CRT_SECURE_NO_WARNIGNS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Structuri

typedef struct node {
	struct node *leftNode; 
	int data; 
	int height; 
	int isBalanced;
	struct node *rightNode; 
} Node;

unsigned short int isTreeBalanced = 1;
unsigned short int iterator = 0;

// Functii
Node* createNode(int data) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	
	newNode->data = data; 
	newNode->isBalanced = 0; 
	newNode->height = 1;
	newNode->leftNode = NULL; 
	newNode->rightNode = NULL; 
	
	return newNode; 
}

int getHeight(Node* root) {
	return (root) ? root->height : 0;
}

Node* insert(Node* root, int data) {
	if (root == NULL) {
		root = createNode(data);
	}
	else if (root->data >= data) {
		root->leftNode = insert(root->leftNode, data);
	}
	else {
		root->rightNode = insert(root->rightNode, data);
	}

	root->height = 1 + max(getHeight(root->leftNode), getHeight(root->rightNode));

	return root;
}

unsigned short int search(Node* root, int data) {
	if (root == NULL) {
		return 0;
	}
	else if (root->data == data) {
		return 1;
	}
	else if (root->data >= data) {
		return search(root->leftNode, data);
	}
	else {
		return search(root->rightNode, data);
	}
}

void inorder(Node* root) {
	if (root == NULL) {
		return;
	}

	inorder(root->leftNode);
	printf("%d ", root->data);
	inorder(root->rightNode);
}

void preorder(Node* root) {
	if (root == NULL) {
		return;
	}

	printf("%d ", root->data);
	preorder(root->leftNode);
	preorder(root->rightNode);
}

void postorder(Node* root) {
	if (root == NULL) {
		return;
	}

	postorder(root->leftNode);
	postorder(root->rightNode);
	printf("%d ", root->data);
}

Node* findMaxNode(Node* root) {
	while (root->rightNode != NULL) {
		root = root->rightNode;
	}

	return root;
}

Node* deleteNode(Node* root, int data) {
	if (root == NULL) {
		return root; 
	}
	else if (data < root->data) {
		root->leftNode = deleteNode(root->leftNode, data);
	}
	else if (data > root->data) {
		root->rightNode = deleteNode(root->rightNode, data);
	}
	else { // when the node is found
		// the node has no child
		if (root->leftNode == NULL && root->rightNode == NULL) {
			free(root);
			root = NULL; 
		}
		// the node has one child
		else if (root->rightNode == NULL) {
			Node* temp = root; 
			root = root->leftNode;
			free(temp);
			temp = NULL; 
		}

		else if (root->leftNode == NULL) {
			Node* temp = root; 
			root = root->rightNode; 
			free(temp);
			temp = NULL;
		}
		// the node had 2 children
		else {
			Node *temp = findMaxNode(root->leftNode);
			root->data = temp->data;
			root->leftNode = deleteNode(root->leftNode, temp->data);
		}
	}

	return root;
}

unsigned short int isBST(Node* root, int minVal, int maxVal) {
	if (root == NULL) {
		return 1;
	}

	if (root->data >= minVal && root->data <= maxVal
		&& isBST(root->leftNode, minVal, root->data)
		&& isBST(root->rightNode, root->data, maxVal)) {
		return 1;
	}
	else {
		return 0;
	}
}

unsigned short int checkIfBST(Node* root) {

	return isBST(root, INT_MIN, INT_MAX);
}

void setBalance(Node* root) {
	if (root) {
		setBalance(root->leftNode);
		root->isBalanced = getHeight(root->leftNode) - getHeight(root->rightNode);
		setBalance(root->rightNode);
	}
}

void getEvenNumbers(Node *root, int *evenNr) {
	if (root) {
		getEvenNumbers(root->leftNode, evenNr); 

		if (root->data % 2 == 0) {
			evenNr[iterator] = root->data;
			iterator++;
		}

		getEvenNumbers(root->rightNode, evenNr);
	}
}

void checkIsTreeBalanced(Node* root) {
	if (root) {
		setBalance(root);

		checkIsTreeBalanced(root->leftNode); 

		if (root->isBalanced > 1 || root->isBalanced < -1) {
			isTreeBalanced = 0;
		}

		checkIsTreeBalanced(root->rightNode);
	}
}

int main() {
	Node *root = NULL; 
	int evenNr[20];
	
	root = insert(root, 10);
	root = insert(root, 5);
	root = insert(root, 3);
	root = insert(root, 7);
	root = insert(root, 8);
	root = insert(root, 15);
	root = insert(root, 12);
	root = insert(root, 18);

	printf("Rezultat cautare 18: %hu\n", search(root, 18));
	printf("Rezultat cautare 21: %hu\n", search(root, 21));

	preorder(root);
	printf("\n");
	inorder(root);
	printf("\n");
	postorder(root);
	printf("\n");

	root = deleteNode(root, 10);
	inorder(root);
	printf("\n");

	if (checkIfBST(root)) {
		printf("The BT is a BST\n");
	}
	else {
		printf("The BT is not a BST\n");
	}
	
	checkIsTreeBalanced(root);
	printf((isTreeBalanced) ? "Tree is balanced.\n" : "Tree is not balanced.\n");

	getEvenNumbers(root, evenNr);
	for (unsigned short int k = 0; k < iterator; k++) {
		root = deleteNode(root, evenNr[k]);
	};

	inorder(root);

	//(Node*)malloc(sizeof(Node));
}