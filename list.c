/*****************************************************************************************************************************
 * COEN 12 43492                                                                                                             *
 * Dr. Yuhong Liu                                                                                                            *
 *                                                                                                                           *
 * list.c                                                                                                                    *
 * Kayleigh Vu                                                                                                               *
 * Term Project: Create a doubly linked-list with each node containing a circuluar array to run maze.c, radix.c, and qsort.c *
 *****************************************************************************************************************************/
 
 #include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"

#define ARRAY_SIZE 10

typedef struct node{
    int aCount;
    int first;
    int size;
    int **data;
    struct node *next;
    struct node *prev;
}NODE;

typedef struct list{
    int nCount;
    struct node *head;
    struct node *tail;
}LIST;

//function to create a node and initial all variables
NODE *createNode(void){
    struct node *np;
    np = malloc(sizeof(NODE));
    np->aCount = 0;
    np->first = 0;
    np->size = ARRAY_SIZE; //initializing node pointer to be able to change the size of arrays in different nodes 

    np->data = malloc(sizeof(void *) * ARRAY_SIZE);
    np->next = NULL;
    np->prev = NULL;

    return np;
}

/*
 * Function: createList
 *
 * Big O(1)
 * Description: returns a pointer to a new created list
 *
*/
LIST *createList(void){
    LIST *lp = malloc(sizeof(LIST));
    assert(lp != NULL);
    lp->nCount = 1;

    lp->head = createNode();
    lp->tail = lp->head;

    return lp;
}

/*
 * Function: destroyList
 *
 * Big O(n)
 * Description: deallocates memory when removing nodes pointed by list pointer
 *
*/
void destroyList(LIST *lp){
    assert(lp != NULL);

    NODE *pDel, *pHolder;
    pDel = lp->head;

    while (pDel != NULL){
        pHolder = pDel->next;
        free(pDel->data);
        free(pDel);
        pDel = pHolder;
    }

    free(lp);
}

/*
 * Function: numItems
 *
 * Big O(1)
 * Description: return the total number of items in the list
 *
 */
int numItems(LIST *lp){
    assert(lp != NULL);
    int num = 0;

    NODE *np = lp->head;
    while(np != NULL){
        num += np->aCount;
        np = np->next;
    }

    return num;
}

/* 
 * Function: addFirst
 *
 * Big O(n)
 * Description: adds a new node to the front of the linked list if the node that head points to is full, then make the array inside the node
 *
 */
void addFirst(LIST *lp, void *item){
    assert(lp != NULL);

    NODE *pHead = malloc(sizeof(NODE));

    if (lp->head->aCount == lp->head->size){

        pHead->data = malloc(sizeof(void*)*(2*lp->head->size));
        pHead->size = lp->head->size * 2;
        pHead->aCount = 0;
        pHead->first = 2 * (lp->head->size) - 1;

        pHead->prev = NULL;
        pHead->next = lp->head->next;
        lp->head = lp->head->prev;


        lp->head = pHead;
        pHead->data[pHead->first] = item;
        lp->nCount++;

    }

    else{
        lp->head->first--;
        if(lp->head->first < 0) //if first is less than 0, add the size of the array and set it 
            lp->head->first += lp->head->size;
        lp->head->data[lp->head->first] = item;
    }

    lp->head->aCount++;
}

/*
 * Function: addLast
 *
 * Big O(n)
 * Description: adds a new node to the end of the linked list, after the node that tail pointer points to if the tail pointer node's array is full
 *
 */
 void addLast(LIST *lp, void *item){
    assert(lp != NULL);
    NODE *pTail = malloc(sizeof(NODE));

    if(lp->tail->aCount == lp->tail->size){

        pTail-> data = malloc(sizeof(void*) * (2 * lp->tail->size));
        pTail-> first = 0;
        pTail->aCount = 0;
        pTail->size = lp->tail->size * 2;

        pTail->prev = lp->tail;
        pTail->next = NULL;

        lp->tail->next = pTail;
        lp->tail = lp->tail->next;
        lp->tail->data[0] = item;

        lp->nCount++;

    }
    else
        lp->tail->data[(lp->tail->first + lp->tail->aCount) % lp->tail->size] = item;

    lp->tail->aCount++;
}

/*
 * Function: removeFirst
 *
 * Big O(n)
 * Description: removes the first in the list and returns the data
 *
 */
void *removeFirst(LIST *lp){
    NODE *pCur = lp->head;
    assert(lp != NULL && lp->nCount > 0);

    if (lp->head->aCount == 0){
        lp->head = lp->head->next;
        lp->head->prev = NULL;
        free(pCur->data);
        free(pCur);
        lp->nCount--;
    }
    void *pCurData = lp->head->data[lp->head->first];

    lp->head->data[lp->head->first] = NULL;
    lp->head->first = (lp->head->first + 1) % lp->head->size;
    lp->head->aCount--;
    return pCurData;

}

/*
 * Function: removeLast
 *
 * Big O(n)
 * Description: removes the last node in the list and returns pointed by the lp pointer
 *
 */
void *removeLast(LIST *lp){
    assert(lp != NULL && lp->nCount > 0);

    if(lp->tail->aCount == 0){

        NODE *p = lp->tail;
        lp->tail = lp->tail->prev;
        free(p);
        lp->nCount--;

    }

    NODE *pTail = lp->tail;
    void *pData = pTail->data[(pTail->first + pTail->aCount - 1) % lp->tail->size];
    pTail->data[(pTail->first + pTail->aCount - 1) % lp->tail->size] = NULL;
    pTail->aCount--;
    return pData;

}

/************************
void *getFirst(LIST *lp){
    assert(lp != NULL);
    return(lp->head->data[lp->head->first]);
}

void *getLast(LIST *lp){
    assert(lp != NULL);
    return(lp->tail->data[(lp->tail->first + lp->tail->aCount - 1) % lp->head->size]);
}************************/

/*
 * Function: getItem
 *
 * Big O(n)
 * Description: returns the item that is positioned at index in list
 * 
 */
void *getItem(LIST *lp, int index){
    assert(lp != NULL);

    NODE *temp = lp->head;
    while(temp->aCount <= index){

        index -= temp->aCount;
        temp = temp->next;
    }

    return temp->data[index];
}

/*
 * Function: setItem
 *
 * Big O(n)
 * Descrtiption: sets the item into the index of data
 *
 */
void setItem(LIST *lp, int index, void *item){
    assert(lp != NULL);

    NODE *temp = lp->head;
    while(temp->aCount <= index){

        index -= temp->aCount;
        temp = temp->next;
    }
    temp->data[index] = item;
}

 
