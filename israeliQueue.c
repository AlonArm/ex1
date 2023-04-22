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
personPtr copyPersonQueue(personPtr head); //this helper function will create a copy of every person inside the function that coies everything
int FuncListSize(IsraeliQueue queue);

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
     funcNodePtr newNode= (funcNodePtr)malloc(sizeof(FuncNode));
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
     personPtr person= (personPtr)malloc(sizeof(person));
     person->enemyHeldBack = head->enemyHeldBack;
      person->friendsPassed= head->friendsPassed;
    person->next=copyPersonQueue(head->next);
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

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void * person) //need to figure out together the return types
{

    if(queue==NULL||person==NULL)
    {
      return ISRAELIQUEUE_BAD_PARAM;
    }

  
       personPtr newPerson = (personPtr) person;
       funcNodePtr tempFunc = queue->funcList;
       personPtr tmpPerson = queue->head;
       personPtr friend = NULL;
       personPtr helperPointer=NULL;
       double average = 0;
       int sizeOfFuncList = FuncListSize(queue);
       bool flag = true;

       while(tmpPerson->next!=NULL)
      {

        if(friend==NULL) //if there is no current friend check if current node is friend that i can pass
        {
        while(tempFunc!=NULL&&flag)
        {
          if(tempFunc->func(newPerson,tmpPerson)>queue->friendship_th&&tmpPerson->friendsPassed<5)
          {
            friend = tmpPerson;
            flag=false;
          }
          tempFunc= tempFunc->next;
        }
        }
            else //check if there is an enemy behind
            {
            while(tempFunc!=NULL&&flag)
            {
              if(tempFunc->func(newPerson,tmpPerson)>queue->friendship_th)
              {
                flag=false;
              }
              average = (tempFunc->func(tmpPerson,newPerson)/(double)sizeOfFuncList);

            }
            if(flag&&average<queue->rivalry_th&&tmpPerson->enemyHeldBack<3)
            {
                tmpPerson->enemyHeldBack++;
                friend=NULL;
            }
             }

           tempFunc =queue->funcList;
           tmpPerson= tmpPerson->next;
           flag=true;
           average=0;

        }

            if(friend!=NULL)
            {
              helperPointer=friend->next;
              friend->next = newPerson;
              newPerson->next=helperPointer;
            }
            else
            {
              tmpPerson->next=newPerson;
              newPerson->next=NULL;
            }
      
return ISRAELIQUEUE_SUCCESS;

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
int FuncListSize(IsraeliQueue queue)//void function to get back size of list
{
  int num =0;
  funcNodePtr temp = queue->funcList;

  while(temp!=NULL)
  {
    num++;
    temp=temp->next;
  }
  return num;
}

bool IsraeliQueueContains(IsraeliQueue queue, void * person)
{
  if(person==NULL||queue==NULL)
  {
    return false;
  }
  
   personPtr newPerson = (personPtr) person;
   personPtr temp = queue->head;
   
   while(temp!=NULL)
   {
     if(temp==person)
     {
       return true;
     }
     temp=temp->next;

   }
   return false;
}

void* IsraeliQueueDequeue(IsraeliQueue queue)
{
  if(queue->head==NULL)
  {
    return NULL;
  }
  personPtr temp = queue->head;
  queue->head=queue->head->next;
  free(temp);
  return queue->head;
}
