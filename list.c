 #include "list.h"
 /**
  *  File Name: list.c
  *  Author: Rehan Nagoor Mohideen
  *  Student ID: 1100592
  * Purpose: Contains functions for CIS2520 assignment 3
  */

struct Performance *newPerformance() {
    struct Performance *new_performance = malloc(sizeof(struct Performance));//allocate memory to the structure
    if (new_performance == NULL) { //For ERROR handling
        fprintf(stderr, "ERROR: not enough space for mallocof newPerformance\n");
        exit(0);
    }
    //initialize values to 0
    new_performance->reads = 0;
    new_performance->writes = 0;
    new_performance->mallocs = 0;
    new_performance->frees = 0;

    return new_performance;
}

void push(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width){
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL) { //For ERROR handling
        fprintf(stderr, "ERROR: not enough space to malloc a new Node\n");
        exit(0);
    }

    newNode->data = malloc(width);
    if (newNode->data == NULL) { //For ERROR handling
        fprintf(stderr, "ERROR: not enough space to malloc data in newNode\n");
        exit(0);
    }

    memcpy(newNode->data, src, width);//copy the data to the node
    //linik the list and assign it to node head
    newNode->next = *list_ptr;
    *list_ptr = newNode;

    performance->mallocs++;
    performance->writes++;
}

void readHead( struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width ){
    if (list_ptr == NULL || *list_ptr == NULL) {//If list is empty
        fprintf(stderr, "ERROR: list is empty.\n");
        exit(0);
    }
    memcpy(dest, (*list_ptr)->data, width);
    
    performance->reads++;
}

void pop( struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width ) {
    if (list_ptr == NULL || *list_ptr == NULL) {//If list is empty
        fprintf(stderr, "ERROR: list is empty.\n");
        exit(0);
    }
    memcpy(dest, (*list_ptr)->data, width);//capoy the data to be deleted
    struct Node *oldHead = *list_ptr;//store the location of the data to be deleted
    *list_ptr = (*list_ptr)->next;//increment the head of the list to the next Node
    free(oldHead->data);
    free(oldHead);

    performance->frees++;
    performance->reads++;
}

struct Node **next( struct Performance *performance, struct Node **list_ptr){
    if (list_ptr == NULL || *list_ptr == NULL) {//If list is empty
        fprintf(stderr, "ERROR: list is empty.\n");
        exit(0);
    }

    performance->reads++;
    return &((*list_ptr)->next);//return the address of the next node
}

int isEmpty( struct Performance *performance, struct Node **list_ptr){
    if (list_ptr == NULL || *list_ptr == NULL) {//If list is empty
        return 1;
    }
    return 0;
}


void freeList( struct Performance *performance, struct Node **list_ptr) {
    int buffer;
    while (!isEmpty(performance, list_ptr)) {//pops all list elements until list is empty
        pop(performance, list_ptr, &buffer, 0);
    }
}

void readItem( struct Performance *performance, struct Node **list_ptr, unsigned int index, void *dest, unsigned int width) {
    struct Node **temp_ptr = list_ptr;
    int i;

    for (i = 0; i < index; i++) {//loop till the index of the required item
        temp_ptr = next(performance, temp_ptr);
    }
    readHead(performance, temp_ptr, dest, width);
}

void appendItem( struct Performance *performance, struct Node**list_ptr, void *src, unsigned int width) {
    struct Node **temp_ptr = list_ptr;

    while (!isEmpty(performance, temp_ptr)) {//loop till the list is empty to add the new item at the end
        temp_ptr = next(performance, temp_ptr);
    }
    push(performance, temp_ptr, src, width);
}

void insertItem(struct Performance *performance, struct Node **list_ptr, unsigned int index, void *src, unsigned int width) {
    struct Node **temp_ptr = list_ptr;
    int i;
    for (i = 0; i < index; i++) {//loop till the index to push the required item at the required position
        temp_ptr = next(performance, temp_ptr);
    }
    push(performance, temp_ptr, src, width);
}

void prependItem(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width) {
    insertItem(performance, list_ptr, 0, src, width);//insert item at index of 0 or first element
}

void deleteItem(struct Performance *performance, struct Node **list_ptr, unsigned int index ) {
    struct Node **temp_ptr = list_ptr;
    int i;

    for (i = 0; i < index; i++) {//loop till the required index to delete the item
        temp_ptr = next(performance, temp_ptr);
    }
    pop(performance, temp_ptr, &i, 0);//using i as a buffer variable since copying no data
}

int findItem( struct Performance *performance, struct Node **list_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width) {
    int i = 0;
    struct Node **temp_ptr = list_ptr;
    void *buffer = malloc(width);//to get the values from the list and compare
    int compareVal = -1;

    if (isEmpty(performance, temp_ptr)==0) {//check if the list is not empty to read the value and compare first value
        readHead(performance, temp_ptr, buffer, width);
        compareVal = (compar(buffer, target)!=0);
    }
    
    while ( (compareVal!=0) && (isEmpty(performance, temp_ptr)==0) ) {//loop till value is found or the list is empty (item not in list)
        //increment index and go to the next list item to perform the operations
        i++;
        temp_ptr = next(performance, temp_ptr);

        if (isEmpty(performance, temp_ptr)==0) {//check if the list is not empty to read the value and compare if found
            readHead(performance, temp_ptr, buffer, width);
            compareVal = (compar(buffer, target)!=0);
        }
    }
    
    if (compar(buffer, target) == 0) {//if the value is found return the index position of the value
        free(buffer);
        return i;
    }
    //otherwise return -1 to signify that the value is not found
    free(buffer);
    return -1;    
}