#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 5
#define M 4

int flag = 0;

typedef struct cell *direction;
struct cell{
	char value;
	int visit;
	direction north,south,west,east;
};

void freelabyrinth(direction head){
	direction temp, temphead;
	int i,j;
	for(i=0;i<N;i++){
		temphead = head;
		head = head->south;
		for(j=0;j<M;j++){
			temp = temphead;
			temphead = temphead->east;
			free(temp);
		}
	}
}

void printlabyrinth(direction head){
	int i,j;
	direction temp,temp2;
	temp = temp2 = head;

	for(i=0;i<N;i++){
		for(j=0;j<M;j++){
			printf("%c ",temp->value);
			temp = temp->east;
		}
		printf("\n");
		temp=temp2->south;
		temp2=temp2->south;
	}
	printf("\n");
	return;
}

void diasxish(direction super,direction head){
	if(super->value == 'F'){
		super->value = '@';
		flag = 1;
		return;
	}
	if(super->value == '-')
		super->value = '*';
	printf("--------------------------------------------------------------\n\n");
	printlabyrinth(head);
	if(super->west->value != 'W' && super->west->visit == 0){
		super->west->visit = 1;
		diasxish(super->west, head);
	}
	if(super->north->value != 'W' && super->north->visit == 0){
		super->north->visit = 1;
		diasxish(super->north, head);
	}
	if(super->south->value != 'W' && super->south->visit == 0){
		super->south->visit = 1;
		diasxish(super->south, head);
	}
	if(super->east->value != 'W' && super->east->visit == 0){
		super->east->visit = 1;
		diasxish(super->east, head);
	}
	if(super->west->value == '@' || super->north->value == '@' || super->south->value == '@' || super->east->value == '@')
		super->value='@';
	else
		super->value='-';
}

void main(void){
	FILE *fp;
	char pinakas[N][M];
	int i, j, ch;
	direction head, temp, temphead, temp2, super;
	head = temphead = NULL;

	fp = fopen("labyrinth.txt","r");
	if(fp == NULL){
		printf("Error while opening file.\n");
		exit(-1);
	}
	i = j = 0;
	while( (ch  = fgetc(fp)) != EOF){
		if( (char)ch == '\n' || (char)ch == '\0' )
			continue;
		if( (i == N-1) && (j == M-1) ){
			pinakas[i][j] = (char)ch;
			continue;
		}
		else
			pinakas[i][j] = (char)ch;
		if(j == M-1){
			i++;
			j=0;
		}
		else
			j++;
	}
	fclose(fp);
	for(i=0;i<N;i++){
		for(j=0;j<M;j++){
			direction new = (direction)malloc(sizeof( *new));
			//isws fygei to apo katw
			if(i == 0)
				new->north = NULL;
			else if(i == N-1)
				new->south = NULL;
			if(j == M-1)
				new->east = NULL;
			if( pinakas[i][j] == 'S')
				super = new;
			if(i == 0 && j == 0){
				head = temp =  new;
				new->visit = 0;
				new->value = pinakas[i][j];
				new->north = NULL;
				new->west = NULL;
			}
			else{
				temp->east = new;
				new->visit = 0;
				new->value = pinakas[i][j];
				new->west = temp;
				temp=new;
			}
			if(temphead != NULL){
				if(j == 0){
					temp = new;
					new->visit = 0;
					new->value = pinakas[i][j];
				}
				else{
					temp->east = new;
					new->west = temp;
					temp = new;
					new->visit = 0;
					new->value = pinakas[i][j];
				}
				temp2->south = new;
				new->north = temp2;
				temp2 = temp2->east;
			}
		}
		if(i == 0)
			temphead = temp2 = head;
		else{
				temphead = temp2 = temphead->south;
		}
	}
	diasxish(super, head);
	if(flag == 1){
		printf("WIN\n");
		printf("|----------------|---------------|\n");
		printlabyrinth(head);
	}
	else
		printf("There is no solution!\n");
	
	freelabyrinth(head);
	return;
}
