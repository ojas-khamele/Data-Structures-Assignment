#include <stdio.h>
#include <malloc.h>

typedef struct Time
{
    int Hour;
    int Min;
    int Sec;
} Time;

typedef struct Flight_tag
{
    int flightID;
    Time departureTime;
    Time ETA;
    struct Flight_tag*next;
} Flight;

typedef struct Bucket_tag
{
    Flight *flights;
    int BucketID;
    Time beginningETA;
    Time endETA;
    struct Bucket_tag*next;
} Bucket;

Flight* create_node(int ID,Time departureTime, Time ETA) 
{
    Flight* nptr;
    nptr = (Flight*)malloc(sizeof(Flight));
    
    nptr->flightID = ID;
    nptr->next = NULL;

    nptr->departureTime.Hour = departureTime.Hour;
    nptr->departureTime.Min = departureTime.Min;
    nptr->departureTime.Sec = departureTime.Sec;

    nptr->ETA.Hour = ETA.Hour;
    nptr->ETA.Min = ETA.Min;
    nptr->ETA.Sec = ETA.Sec;

    return nptr; 
}

int compareTime(Time t1,Time t2)    //ret_val=1 indicates t1 < t2 and 0 indicates t1 > t2
{
    int ret_val=1;

    if(t1.Hour > t2.Hour) ret_val=0;
    else if(t1.Hour < t2.Hour) ret_val=1;

    else
    {
        if(t1.Min > t2.Min) ret_val=0;
        else if (t1.Min < t2.Min) ret_val=1;

        else
        {
            if(t1.Sec > t2.Sec) ret_val=0;
            else if (t1.Sec < t2.Sec) ret_val=1;
        }
    }

    return ret_val;
} 

Flight* insert_in_PlaneList(Flight* head, Flight* newNode)  //inserting in ascending order
{
    if(head==NULL) head=newNode;
    
    else if (compareTime(head->ETA,newNode->ETA)==0)    //Insert at start
    {
        newNode->next=head;
        head=newNode;
    }

    else    //General Case
    {
        Flight*ptr,*prev;
        ptr=head;
        
        while(ptr->next!=NULL && compareTime(ptr->ETA,newNode->ETA))
        {
            prev = ptr;
            ptr = ptr->next;
        }

        if(ptr->next==NULL)
        {
            if(compareTime(ptr->ETA,newNode->ETA))  //Insert at end
            {
                ptr->next = newNode;
                newNode->next = NULL;
            }

            else
            {
                newNode->next = ptr;
                prev->next = newNode; 
            } 
        }

        else
        {
            newNode->next = ptr;
            prev->next = newNode;
        }
    }

    return head;
}

void show_PlaneList(Flight*ptr)
{
    while(ptr!=NULL)
    {
        printf("Flight ID is %d\n",ptr->flightID);
        printf("Flight ETA = %d:%d:%d\n\n",ptr->ETA.Hour,ptr->ETA.Min,ptr->ETA.Sec);
        ptr=ptr->next;
    }
}

Bucket* InsertInBucketList(Bucket*head, Bucket*newBucket)
{
    if(head==NULL) head=newBucket;

    else if(head->BucketID > newBucket->BucketID)
    {
        newBucket->next=head;
        head=newBucket;
    }

    else
    {
        Bucket*ptr,*prev;
        ptr=head;

        while(ptr!=NULL && ptr->BucketID < newBucket->BucketID)
        {
            prev=ptr;
            ptr=ptr->next;
        }

        if(ptr==NULL)
        {
            prev->next=newBucket;
            newBucket->next=NULL;
        }

        else
        {
            newBucket->next=ptr;
            prev->next=newBucket;
        }
    }

    return head;
}

Bucket* createBucket(int ID)    //Assigning BucketID = ETA.hour
{
    Bucket*newBucket;

    newBucket=(Bucket*)malloc(sizeof(Bucket));
    
    newBucket->BucketID=ID;
    newBucket->flights=NULL;
    newBucket->next=NULL;
    
    return newBucket;
}

Bucket* InsertFlightsInBucket(Flight* lptr,Bucket* bHead)  
{
    Bucket* newBucket=NULL;

    newBucket=createBucket(lptr->ETA.Hour); 
    newBucket->beginningETA=lptr->ETA;
    newBucket->endETA=lptr->ETA;
    newBucket->flights=lptr;
    

    Flight*prev;

    while(lptr!=NULL)
    {
        
        if(newBucket->BucketID==lptr->ETA.Hour) newBucket->endETA=lptr->ETA;
        else
        {
            prev->next=NULL;
            bHead=InsertInBucketList(bHead,newBucket);
            newBucket=createBucket(lptr->ETA.Hour);
            newBucket->beginningETA=lptr->ETA;
            newBucket->endETA=lptr->ETA;
            newBucket->flights=lptr;
        }
        
        prev=lptr;
        lptr=lptr->next;
    }

    return bHead;
}

void showBucketList(Bucket*head)
{
    while(head!=NULL)
    {
        printf("Bucket Id is %d\n",head->BucketID);
        printf("Bucket begin ETA = %d:%d:%d\n",head->beginningETA.Hour,head->beginningETA.Min,head->beginningETA.Sec);
        printf("Bucket end ETA = %d:%d:%d\n\n",head->endETA.Hour,head->endETA.Min,head->endETA.Sec);
        head=head->next;
    }
}

int main()
{
    int tempFlightID;

    Time tempDepartureTime, tempETA;
    FILE *fptr = fopen ("Data.csv", "r");

    Flight *plane_lptr; //header to list of planes 
    plane_lptr=NULL;

    Flight*planeNode;  //temp pointer to link nodes in list

    char line[100];
    char header[256];

    while (fgets(line, 100, fptr) != NULL)
    {
        sscanf (line, "%d,%d,%d,%d,%d,%d,%d", &tempFlightID, &(tempDepartureTime.Hour), &(tempDepartureTime.Min), &(tempDepartureTime.Sec), &(tempETA.Hour), &(tempETA.Min), &(tempETA.Sec));
        //ID,DH,DM,DS,EH,EM,ES

        planeNode = create_node(tempFlightID,tempDepartureTime,tempETA);
        plane_lptr = insert_in_PlaneList(plane_lptr,planeNode);
    }

    Bucket*ActiveBuckets=NULL;
    ActiveBuckets=InsertFlightsInBucket(plane_lptr,ActiveBuckets);

    showBucketList(ActiveBuckets);
    // show_PlaneList(plane_lptr);
    return 0;
}