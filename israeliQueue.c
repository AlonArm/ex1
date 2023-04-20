#include <stdio.h>      
#include <stdlib.h>     
#include "IsraeliQueue.h"

//creating the nodes of people in our list with the relevant values
typedef struct person {
  int friendsPassed;
  int enemyHeldBack;
struct person* next;
} person;

struct funcNode{
  FriendshipFunction func;
  struct funcNode *next;
};

struct IsraeliQueue_t
{
  struct person* head;
  struct funcNode* funcList;
  //int friendshipCounter; //for realloc and free
  //FriendshipFunction* friendshipFunctions;
  ComparisonFunction compare;
  int friendship_th;
  int rivalry_th;

};
IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendFunctions, ComparisonFunction compFunction, int friendThres, int rivalTres){
  IsraeliQueue queue = (IsraeliQueue)malloc(sizeof(IsraeliQueue));
  //queue->friendshipFunctions = friendFunctions;
  queue->compare = compFunction;
  queue->friendship_th = friendThres;
  queue->rivalry_th = rivalTres;
  if(friendFunctions[0] != NULL){
    queue->funcList = (struct funcNode*)(malloc(sizeof(struct funcNode)));
    queue->funcList->func = friendFunctions[0];
    struct funcNode *temp = queue->funcList;
    for(int i = 1 ; friendFunctions[i] != NULL ; i++){
      temp->next = (struct funcNode*)(malloc(sizeof(struct funcNode)));
      temp->next->func = friendFunctions[i];
      temp = temp->next;
    }
  }
  return queue;
}
IsraeliQueue IsraeliQueueClone(IsraeliQueue q); //mine!!
{
  if(q==null) //check for nullity
  {
    return null;
  }
IsraeliQueue clone = (IsraeliQueue)malloc(sizeof(IsraeliQueue)); ;
clone->rivalry_th=q->rivalry_th;
clone->friendship_th=q->friendship_th;

    //void (*newFunc)(int) = (void (*)(int)) malloc(sizeof(myFuncCopy)); example of creating  new func

if(){}//check if head is not null for q the loop to see what the result is

ComparisonFunction cloneCompare 
//need to deal with friendship function see that it is not trivial because you need to create a new one
return clone;

}
void IsraeliQueueDestroy(IsraeliQueue queue){
  struct person* p = queue->head;
  while(p != NULL){
    struct person* temp = p;
    p = p->next;
    free(temp);
  }
  struct funcNode* f = queue->funcList;
  while(f != NULL){
    struct funcNode* temp = f;
    f = f->next;
    free(temp);
  }
}
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queue, int threshold){
    queue->friendship_th = threshold;
    if(queue->friendship_th == threshold) return ISRAELIQUEUE_SUCCESS;
    return ISRAELI_QUEUE_ERROR;
}
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue queue, int threshold){
    queue->rivalry_th = threshold;
    if(queue->rivalry_th == threshold) return ISRAELIQUEUE_SUCCESS;
    return ISRAELI_QUEUE_ERROR;
}



