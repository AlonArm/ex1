#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include "IsraeliQueue.h"


typedef struct personQueue* personPtr; //for cleaner code
typedef struct funcNode* funcNodePtr;//for cleaner code

typedef struct personQueue 
{
  int friendsPassed;
  int enemyHeldBack;
  void* person;
  struct personQueue *next, *previous;
}personQueue;

funcNodePtr copyFunc(funcNodePtr); //this function appears later in the code and was added by me to make the code cleaner
personPtr copyPersonQueue(personPtr head); //this helper function will create a copy of every person inside the function that coies everything
int FuncListSize(funcNodePtr);
int compare(IsraeliQueue, void*, void*);
void destroyFuncList(funcNodePtr);

struct funcNode
{
  FriendshipFunction func;
  struct funcNode *next;
}funcNode;

struct IsraeliQueue_t
{
  personPtr head, tail;
  funcNodePtr funcList;
  ComparisonFunction compare;
  int friendship_th;
  int rivalry_th;
}IsraeliQueue_t;

IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendFunctions, ComparisonFunction compFunction, int friendThres, int rivalTres){
  IsraeliQueue queue = (IsraeliQueue)malloc(sizeof(IsraeliQueue_t)); //i think the size allocation here needs to be that of the struct itslef
  //queue->friendshipFunctions = friendFunctions;
  //our pointers inside the israeliqueue are not pointing to anything which is unddefined beahvior maybe we need to change this
  queue->compare = compFunction;
  queue->friendship_th = friendThres;
  queue->rivalry_th = rivalTres;
  queue->head=NULL; //so that there are no errors
  queue->tail = NULL;
  if(friendFunctions[0] != NULL)
  {
    queue->funcList = (funcNodePtr)(malloc(sizeof(struct funcNode)));
    queue->funcList->func = friendFunctions[0];
    funcNodePtr temp = queue->funcList;
    for(int i = 1 ; friendFunctions[i] != NULL ; i++)
    {
      temp->next = (funcNodePtr)(malloc(sizeof(struct funcNode)));
      temp->next->func = friendFunctions[i];
      temp = temp->next;
    }
  }
  return queue;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q) 
{
  if(q==NULL) //check for nullity if so then return null
  {
    return NULL;
  }
  IsraeliQueue clone = (IsraeliQueue)malloc(sizeof(IsraeliQueue_t)); 
  clone->rivalry_th=q->rivalry_th;
  clone->friendship_th=q->friendship_th;
  clone->compare=q->compare;
  clone->head= copyPersonQueue(q->head);
  clone->funcList=copyFunc(q->funcList);
   return clone;
}

funcNodePtr copyFunc(funcNodePtr head) //helper function for cloning linked list of func
{ //this is an addition to the api
  if(head==NULL)
  {
    return NULL;
  }
     funcNodePtr newNode= (funcNodePtr)malloc(sizeof(struct funcNode));
     newNode->func = head->func;
     newNode->next = copyFunc(head->next);
     return newNode;
}

personPtr copyPersonQueue(personPtr head) //helper function for cloning queue of person data
{ //this is an addition to the api
  if(head==NULL)
  {
    return NULL;
  }
    personPtr person= (personPtr)malloc(sizeof(struct personQueue));
    person->enemyHeldBack = head->enemyHeldBack;
    person->friendsPassed= head->friendsPassed;
    person->person = head->person;
    person->next=copyPersonQueue(head->next);
    if(person->next != NULL){
      person->next->previous = person;
    }
    return person;
}

void IsraeliQueueDestroy(IsraeliQueue queue){
  personPtr p = queue->head;
  while(p != NULL)
  {
    personPtr temp = p;
    p = p->next;
    free(temp->person);
    free(temp);
  }
  destroyFuncList(queue->funcList);
  free(queue);
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queue, int threshold){
    queue->friendship_th = threshold;
    if(queue->friendship_th == threshold) 
    {
      return ISRAELIQUEUE_SUCCESS;
    }
    return ISRAELI_QUEUE_ERROR;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue queue, int threshold){
    queue->rivalry_th = threshold;
    if(queue->rivalry_th == threshold)
    {
      return ISRAELIQUEUE_SUCCESS;
    }
    return ISRAELI_QUEUE_ERROR;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void * person) //need to figure out together the return types
{
  if(queue==NULL||person==NULL){
    return ISRAELIQUEUE_BAD_PARAM;
  }
  personPtr newPerson = (personPtr)malloc(sizeof(struct personQueue));
  newPerson->person = person;
  newPerson->enemyHeldBack = 0;
  newPerson->friendsPassed = 0;
  if(!newPerson){
    return ISRAELIQUEUE_ALLOC_FAILED;
  }
  personPtr tmpPerson = queue->head;
  personPtr friend = NULL;

  while(tmpPerson->next!=NULL){
    if(friend==NULL) //if there is no current friend check if current node is friend that i can pass
    {
      if(tmpPerson->friendsPassed < FRIEND_QUOTA && compare(queue, tmpPerson->person, person) == 1){
        friend = tmpPerson;
      }
    }
    else //check if there is an enemy behind
    {
      if(tmpPerson->enemyHeldBack < RIVAL_QUOTA && compare(queue, tmpPerson->person, person) == -1){
        friend = NULL;
        tmpPerson->enemyHeldBack++;
      }
    }
    tmpPerson= tmpPerson->next;
  }
  newPerson->previous = tmpPerson;
  if(friend == NULL){
    newPerson->next = NULL;
    tmpPerson->next = newPerson;
    queue->tail = newPerson;
  }
  else{
    newPerson->next = tmpPerson->next;
    tmpPerson->next = newPerson;
    tmpPerson->friendsPassed++;
  }
return ISRAELIQUEUE_SUCCESS;
}
//receives two objects in the queue and checks if they are friends or enemies
//returns -1 if they are enemies, 1 if they are friends, 0 if neither
int compare(IsraeliQueue queue, void* person1, void* person2){
  if(queue->funcList == NULL || person1 == NULL || person2 == NULL){
    return 0;
  }
  funcNodePtr temp = queue->funcList;
  int fCounter = 0;
  while(temp != NULL){
    int fLevel = (*(temp->func))(person1, person2);
    if(fLevel > queue->friendship_th) return 1;
    fCounter += fLevel;
    temp = temp->next;
  }
  if(fCounter < queue->rivalry_th * FuncListSize(queue->funcList)) return -1;
  return 0;
}

int IsraeliQueueSize(IsraeliQueue queue)
{
  int num =0;
  personPtr temp = queue->head;

  while(temp!=NULL)
  {
    num++;
    temp=temp->next;
  }
  return num;
}
int FuncListSize(funcNodePtr funcList)//void function to get back size of list
{
  int num =0;
  funcNodePtr temp = funcList;

  while(temp!=NULL)
  {
    num++;
    temp=temp->next;
  }
  return num;
}

bool IsraeliQueueContains(IsraeliQueue queue, void * person)
{
  if(person==NULL||queue==NULL){
    return false;
  }
  
  personPtr temp = queue->head;
   
  while(temp!=NULL){
    if(temp->person==person){
      return true;
    }
    temp=temp->next;
  }
  return false;
}

void* IsraeliQueueDequeue(IsraeliQueue queue)
{
  if(queue == NULL || queue->head==NULL){
    return NULL;
  }
  personPtr first = queue->head;
  queue->head=queue->head->next;
  if(queue->head == NULL){
    queue->tail = NULL;
  }else{
  queue->head->previous = NULL;
  }
  return first;
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction newFunc)// need to add checks that the function works correctly
{
  funcNodePtr newPointer = (funcNodePtr)(malloc(sizeof(struct funcNode)));
    if(newPointer==NULL)
    {
      return ISRAELIQUEUE_ALLOC_FAILED;
    }

  if(queue==NULL||newFunc==NULL)
  {
    return ISRAELIQUEUE_BAD_PARAM;
  }
  
  funcNodePtr temp = queue->funcList;
  while(temp->next != NULL)
  {
    temp = temp->next;
  }
  
    newPointer->func = newFunc;
    temp->next = newPointer;
    newPointer->next = NULL;

    if(temp->next==newPointer)
    {
      return ISRAELIQUEUE_SUCCESS;
    }
    return ISRAELI_QUEUE_ERROR;
}

IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue queue){
  if(queue == NULL) return ISRAELIQUEUE_BAD_PARAM;
  personPtr advancingPerson = queue->tail;
  while(advancingPerson != queue->head){
    personPtr iterator = queue->head;
    personPtr friend = NULL;
    while(iterator != advancingPerson){
      int compareResult = compare(queue, iterator->person, advancingPerson->person);
      if(friend == NULL && compareResult == 1 && iterator->friendsPassed < FRIEND_QUOTA){
        friend = iterator;
      }else if(friend != NULL && compareResult == -1 && iterator->enemyHeldBack < RIVAL_QUOTA){
        friend = NULL;
        iterator->enemyHeldBack++;
      }
      iterator = iterator->next;
    }
    if(friend != NULL){
      personPtr temp = advancingPerson->previous;
      advancingPerson->next->previous = advancingPerson->previous;
      advancingPerson->previous->next = advancingPerson->next;
      advancingPerson->previous = friend;
      advancingPerson->next = friend->next;
      friend->next = advancingPerson;
      friend->friendsPassed++;
      advancingPerson = temp;
    }
    else{
      advancingPerson = advancingPerson->previous;
    }
  }
  return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueue IsraeliQueueMerge(IsraeliQueue* qarr,ComparisonFunction func){
  if(qarr == NULL || qarr[0] == NULL || func == NULL) return NULL;
  int newFThreshold = 0;
  int newRThreshold = 1;
  int qarr_size = 0;
  funcNodePtr newFuncList = NULL;
  for(int i = 0 ; qarr[i] != NULL ; i++){
    qarr_size++;
    newFThreshold += qarr[i]->friendship_th;
    newRThreshold *= qarr[i]->rivalry_th;
    funcNodePtr temp = qarr[i]->funcList;
    while(temp != NULL){
      funcNodePtr addedFunc = (funcNodePtr)malloc(sizeof(struct funcNode));
      addedFunc->func = temp->func;
      addedFunc->next = newFuncList;
      newFuncList = addedFunc;
      temp = temp->next;
    }
  }
  IsraeliQueue newQueue = (IsraeliQueue)malloc(sizeof(struct IsraeliQueue_t));
  if(newQueue == NULL){
    destroyFuncList(newFuncList);
    return NULL;
  }
  newQueue->compare = func;
  newQueue->friendship_th = newFThreshold / qarr_size;
  newQueue->rivalry_th = (int)(ceil(pow(newRThreshold, 1.0/qarr_size)));
  newQueue->funcList = newFuncList;
  newQueue->head = NULL;
  bool allEmpty = false;
  while(!allEmpty){
    allEmpty = true;
    for(int i = 0 ; qarr[i] != NULL ; i++){
      if(IsraeliQueueSize(qarr[i]) > 0){
        allEmpty = false;
        void* person = IsraeliQueueDequeue(qarr[i]);
        IsraeliQueueEnqueue(newQueue, person);
      }
    }
  }
  return newQueue;
}
void destroyFuncList(funcNodePtr funcList){
  if(funcList->next != NULL){
    destroyFuncList(funcList->next);
  }
  free(funcList);
}