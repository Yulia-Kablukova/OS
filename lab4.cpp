#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node {
	char* line;
	Node* next;
}Node;

Node* newElement(char* currentLine) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->next = NULL;
	newNode->line = (char*)malloc(strlen(currentLine) + 1);
	strcpy(newNode->line, currentLine);
	return newNode;
}

int main() {
	Node* head = (Node*)malloc(sizeof(Node));
	head->line = NULL;
	head->next = NULL;
	char currentLine[BUFSIZ];
	Node* currentNode = head;
	printf("Enter lines of text. For exit, enter '.' at the beginning of a line\n");
	int flag = 0;
	while (fgets(currentLine, sizeof(currentLine), stdin) != NULL) {
		if (currentLine[0] == '.')
			break;
		if ((strlen(currentLine) == sizeof(currentLine) - 1) && currentLine[sizeof(currentLine) - 2] != '\n' && flag == 0) {
			printf("Too long!\n");
			flag = 1;
		}
		else if (flag == 0) {
			currentNode->next = newElement(currentLine);
			currentNode = currentNode->next;
		}

		if (currentLine[strlen(currentLine) - 1] == '\n')
			flag = 0;
	}
	for (Node* i = head->next; i != NULL; i = i->next) {
		fputs(i->line, stdout);
	}
	Node* j = NULL;
	Node* i = head->next;
	while (i != NULL) {
		j = i->next;
		free(i->line);
		free(i);
		i = j;
	}
	free(head);
}
