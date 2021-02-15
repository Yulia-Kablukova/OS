#include <sys/types.h>
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
	char currentLine[BUFSIZ];
	Node* currentNode = head;
	
	printf("Enter lines of text. For exit, enter '.' at the beginning of a line\n");
	
	while (fgets(currentLine, sizeof(currentLine), stdin) != NULL) {
  
		if (currentLine[0] == '.')
			break;
      
		if ((strlen(currentLine) == sizeof(currentLine) - 1) && currentLine[sizeof(currentLine) - 2] != '\n') {
			printf("Too long!\n");
      break;
		}
    
    currentNode->next = newElement(currentLine);
    currentNode = currentNode->next;
	}
  
	for (Node* i = head->next; i != NULL; i = i->next) {
		fputs(i->line, stdout);
	}
  
	Node* j = NULL;
	Node* i = head;
	while (i != NULL) {
		j = i->next;
		free(i);
		i = j;
	}
}
