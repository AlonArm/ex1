#include <stdio.h>      
#include <stdlib.h>     
#include "IsraeliQueue.h"


typedef struct person *personPtr; //for cleaner code
typedef struct funcNode *funcNodePtr;//for cleaner code

typedef struct person 
{
  int friendsPassed;
  int enemyHeldBack;
struct person *next;
}person;

funcNodePtr copyFunc(funcNodePtr); //this function appears later in the code and was added by me to make the code cleaner
personPtr copyPersonList(personPtr head); //this helper function will create a copy of every person inside the function that coies everything

struct funcNode
{
  FriendshipFunction func;
  struct funcNode *next;
}FuncNode;

struct IsraeliQueue_t
{
  personPtr head;
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
  if(friendFunctions[0] != NULL)
  {
    queue->funcList = (funcNodePtr)(malloc(sizeof(FuncNode)));
    queue->funcList->func = friendFunctions[0];
    funcNodePtr temp = queue->funcList;
    for(int i = 1 ; friendFunctions[i] != NULL ; i++)
    {
      temp->next = (funcNodePtr)(malloc(sizeof(FuncNode)));
      temp->next->func = friendFunctions[i];
      temp = temp->next;
    }
  }
  return queue;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q) //mine!!
{
  if(q==NULL) //check for nullity if so then return null
  {
    return NULL;
  }
  IsraeliQueue clone = (IsraeliQueue)malloc(sizeof(IsraeliQueue_t)); 
  clone->rivalry_th=q->rivalry_th;
  clone->friendship_th=q->friendship_th;
  clone->compare=q->compare;
  //need to add the pointer to the person and the persons info
  clone->funcList=copyFunc(q->funcList);
   return clone;
}

funcNodePtr copyFunc(funcNodePtr head) //helper function for cloning linked list of func
{ //this is an addition to the api
  if(head==NULL)
  {
    return NULL;
  }
     funcNodePtr newNode= (funcNodePtr)malloc(sizeof(FuncNode));
     newNode->func = head->func;
     newNode->next = copyFunc(head->next);
     return newNode;
}
personPtr copyPerson(personPtr head) //helper function for cloning queue of person data
{ //this is an addition to the api
  if(head==NULL)
  {
    return NULL;
  }
     personPtr person= (personPtr)malloc(sizeof(person));
     person->enemyHeldBack = head->enemyHeldBack;
      person->friendsPassed= head->friendsPassed;
    person->next=copyPerson(head->next);
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
  funcNodePtr f = queue->funcList;
  while(f != NULL)
  {
    funcNodePtr temp = f;
    f = f->next;
    free(temp);
  }
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


