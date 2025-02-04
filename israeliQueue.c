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
  ComparisonFunction compareFunc;
  int friendship_th;
  int rivalry_th;
}IsraeliQueue_t;

IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendFunctions, ComparisonFunction compFunction, int friendThres, int rivalTres){
  IsraeliQueue queue = (IsraeliQueue)malloc(sizeof(IsraeliQueue_t)); 
    if(queue==NULL) //checking for NULLITY
    {
        return NULL;
    }
  queue->compareFunc = compFunction;
  queue->friendship_th = friendThres;
  queue->rivalry_th = rivalTres;
  queue->head=NULL; //so that there are no errors
  queue->tail = NULL;
  if(friendFunctions != NULL && friendFunctions[0] != NULL)
  {
    queue->funcList = (funcNodePtr)(malloc(sizeof(struct funcNode)));
    if(queue->funcList==NULL) //freeing unused data if malloc fails
    {
        IsraeliQueueDestroy(queue);
        return NULL;
    }
    queue->funcList->func = friendFunctions[0];
    funcNodePtr temp = queue->funcList;
    for(int i = 1 ; friendFunctions[i] != NULL ; i++)
    {
      temp->next = (funcNodePtr)(malloc(sizeof(struct funcNode)));
      if(temp->next==NULL) //freeing unused data if malloc fails
      {
          IsraeliQueueDestroy(queue);
          return NULL;
      }
      temp->next->func = friendFunctions[i];
      temp->next->next = NULL;
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
  if(clone==NULL)
  {
      return NULL;
  }
  clone->rivalry_th=q->rivalry_th;
  clone->friendship_th=q->friendship_th;
  clone->compareFunc=q->compareFunc;
  clone->head= copyPersonQueue(q->head);
  clone->funcList=copyFunc(q->funcList);
  return clone;
}

funcNodePtr copyFunc(funcNodePtr head) //helper function for cloning linked list of func
{ //this is an addition to the api
  if(head==NULL){
    return NULL;
  }
     funcNodePtr newNode= (funcNodePtr)malloc(sizeof(struct funcNode));
     if(newNode==NULL)
     {
         return NULL;
     }
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
    if(person==NULL)
    {
        return NULL;
    }
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
  if(newPerson==NULL)
  {
      return ISRAELIQUEUE_ALLOC_FAILED;
  }
  newPerson->person = person;
  newPerson->enemyHeldBack = 0;
  newPerson->friendsPassed = 0;
  if(!newPerson){
    return ISRAELIQUEUE_ALLOC_FAILED;
  }
  if(queue->head == NULL){
    queue->head = newPerson;
    queue->tail = newPerson;
    newPerson->next = NULL;
    newPerson->previous = NULL;
    return ISRAELIQUEUE_SUCCESS;
  }
  personPtr tmpPerson = queue->head;
  personPtr friend = NULL;
  while(tmpPerson->next!=NULL){
    if(friend==NULL) //if there is no current friend check if current node is friend that i can pass
    {
      if(tmpPerson->friendsPassed < FRIEND_QUOTA && compare(queue, person, tmpPerson->person) == 1){
        friend = tmpPerson;
      }
    }
    else //check if there is an enemy behind
    {
      if(tmpPerson->enemyHeldBack < RIVAL_QUOTA && compare(queue, person, tmpPerson->person) == -1){
        friend = NULL;
        tmpPerson->enemyHeldBack = tmpPerson->enemyHeldBack + 1;
      }
    }
    tmpPerson= tmpPerson->next;
  }
  newPerson->previous = tmpPerson;
  if(friend == NULL){
    newPerson->next = NULL;
    newPerson->previous = tmpPerson;
    tmpPerson->next = newPerson;
    queue->tail = newPerson;
  }
  else{
    newPerson->next = friend->next;
    newPerson->next->previous = newPerson;
    newPerson->previous = friend;
    friend->next = newPerson;
    friend->friendsPassed++;
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
  int i = 0;
  while(temp != NULL){
    int fLevel = (*(temp->func))(person1, person2);
    if(fLevel >= queue->friendship_th) return 1;
    fCounter += fLevel;
    temp = temp->next;
    i++;
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
    if((queue->compareFunc)(temp->person, person)){
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
  personPtr temp = queue->head;
  void* data = temp->person;
  queue->head=temp->next;
  if(queue->head == NULL){
    queue->tail = NULL;
  }else{
    queue->head->previous = NULL;
  }
  free(temp);
  return data;
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
  if(queue == NULL) {
      return ISRAELIQUEUE_BAD_PARAM;
  }
  personPtr advancingPerson = queue->tail;
  int i = 0;
  while(advancingPerson != queue->head){
    personPtr iterator = queue->head;
    personPtr friend = NULL;
    int j = 0;
    while(iterator != advancingPerson){
      j++;
      int compareResult = compare(queue, advancingPerson->person, iterator->person);
      if(friend == NULL && compareResult == 1 && iterator->friendsPassed < FRIEND_QUOTA){
        friend = iterator;
      }
      else if(friend != NULL && compareResult == -1 && iterator->enemyHeldBack < RIVAL_QUOTA){
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
    i++;
  }
  return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueue IsraeliQueueMerge(IsraeliQueue* qarr,ComparisonFunction func){
  if(qarr == NULL || qarr[0] == NULL || func == NULL){
      return NULL;
  }
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
      if(addedFunc==NULL)
      {
          return NULL;
      }
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
  newQueue->compareFunc = func;
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
