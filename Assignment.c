#include <stdio.h>
#include <malloc.h>
#include <windows.h>

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

int compareTime(Time t1,Time t2)    //ret_val=1 indicates t1 <= t2 and 0 indicates t1 > t2
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

void showPlaneList(Flight*ptr)
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
    newBucket->beginningETA.Hour=lptr->ETA.Hour;
    newBucket->endETA.Hour=lptr->ETA.Hour;
    newBucket->beginningETA.Min=0;
    newBucket->endETA.Min=59;
    newBucket->flights=lptr;

    Flight*prev;

    while(lptr!=NULL)
    {
        
        if(newBucket->BucketID!=lptr->ETA.Hour)
        {
            prev->next=NULL;
            bHead=InsertInBucketList(bHead,newBucket);
            newBucket=createBucket(lptr->ETA.Hour);
            newBucket->beginningETA.Hour=lptr->ETA.Hour;
            newBucket->endETA.Hour=lptr->ETA.Hour;
            newBucket->beginningETA.Min=0;
            newBucket->endETA.Min=59;
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
        printf("Bucket end ETA = %d:%d:%d\n",head->endETA.Hour,head->endETA.Min,head->endETA.Sec);
        printf("This Bucket contains following flightIDs :\n");
        Flight*temp;
        temp=head->flights;
        
        while(temp!=NULL)
        {
            printf("%d ",temp->flightID);
            temp=temp->next;
        }
        printf("\n\n");
        head=head->next;
    }
}

void insertFlight(Bucket*head)
{
    int ID;
    Time deptTime,ETA;
    printf("Enter details for the flight to be added :\n");
    
    printf("Flight ID : ");
    scanf("%d",&ID);

    printf("Departure time : \nhr = ");
    scanf("%d",&deptTime.Hour);
    printf("min = ");
    scanf("%d",&deptTime.Min);
    printf("sec = ");
    scanf("%d",&deptTime.Sec);

    printf("Expected Arrival time : \nhr = ");
    scanf("%d",&ETA.Hour);
    printf("min = ");
    scanf("%d",&ETA.Min);
    printf("sec = ");
    scanf("%d",&ETA.Sec);

    Flight* newNode;
    newNode = create_node(ID,deptTime,ETA);
    
    Bucket*bptr=head;
    while(bptr!=NULL && bptr->BucketID!=ETA.Hour) bptr=bptr->next;

    if(bptr==NULL)
    {
        bptr=createBucket(ID);
        bptr->flights = newNode;
        bptr->BucketID = newNode->ETA.Hour;
        bptr->beginningETA = newNode->ETA;
        bptr->endETA = newNode->ETA;
        head=InsertInBucketList(head,bptr);
    }
    else
    {
        Flight*ptr,*prev;
        ptr=bptr->flights;

        while(ptr!=NULL && compareTime(ptr->ETA,newNode->ETA))
        {
            prev=ptr;
            ptr=ptr->next;
        }
        if(ptr==NULL) prev->next=newNode;
        else
        {
            newNode->next=ptr->next;
            prev->next=newNode;
        }
    }
}

void removeBucketFromList(Bucket*head,Bucket*emptyNode)
{
    if(head==emptyNode) head=head->next;

    else
    {
        while(head!=NULL && head->next!=emptyNode)head=head->next;

        if(head==NULL) printf("Something went wrong\n");
        else head->next=head->next->next;
    }
    free(emptyNode);
}

void cancelFlight(Bucket*bHead)
{
    Bucket*head=bHead;
    int ID;

    printf("Enter the ID of Flight that is to be cancelled : ");
    scanf("%d",&ID);

    int found=0;

    while(!found && head!=NULL)
    {
        Flight*ptr,*prev;
        ptr=head->flights;
        prev=ptr;

        while(ptr!=NULL && !found)
        {
            if(ptr->flightID==ID)
            {
                found=1;
                printf("\nFlight that was expected to arrive at %d:%d:%d is CANCELLED\n",ptr->ETA.Hour,ptr->ETA.Min,ptr->ETA.Sec);
                if(ptr==head->flights) head->flights=head->flights->next;
                else prev->next = ptr->next;
                free(ptr);
                if(head->flights==NULL) removeBucketFromList(bHead,head);
            }
            prev=ptr;
            ptr=ptr->next;
        }
        head=head->next;
    } 
    if(!found) printf("\nNo such Flight\n");
}

void checkStatus(Bucket*head)
{
    int ID;

    printf("Enter the ID of Flight for checking status : ");
    scanf("%d",&ID);

    int found=0;

    while(!found && head!=NULL)
    {
        Flight*ptr;
        ptr=head->flights;

        while(ptr!=NULL && !found)
        {
            if(ptr->flightID==ID)
            {
                found=1;
                printf("\nFlight is expected to arrive at %d:%d:%d\n",ptr->ETA.Hour,ptr->ETA.Min,ptr->ETA.Sec);
            }
            ptr=ptr->next;
        }
        head=head->next;
    } 
    if(!found) printf("\nNo such Flight\n");
}

void showMenu(Bucket*ActiveBuckets)
{   
    int option;
    do
    {
        printf("1 - Insert a Flight \n2 - Cancel Flight \n3 - Check Status \nPress any other key to exit\n\n");
        printf("Select option : ");
        scanf("%d",&option);

        switch(option)
        {
            case 1 : insertFlight(ActiveBuckets); 
            break;

            case 2 : cancelFlight(ActiveBuckets);
            break;

            case 3 : checkStatus(ActiveBuckets);
            break;
        }
        Sleep(2000);
        showBucketList(ActiveBuckets);
    }
    while(option>0 && option<4);
}

void rearrangeBuckets(Bucket*firstBucket,int mins, int secs)
{
    Flight* allFlights;
    Bucket* bptr;
    
    bptr=firstBucket;

    if(bptr!=NULL)
    {
        // Emptying All Buckets And linking the list of flights in each bucket to form a sorted common list of all flights
        allFlights = bptr->flights;
        while(bptr!=NULL)
        {
            while(bptr->flights!=NULL && bptr->flights->next!=NULL) bptr->flights=bptr->flights->next;
            

            if(bptr->next!=NULL) bptr->flights->next = bptr->next->flights;
            else bptr->flights->next=NULL;
            
            bptr=bptr->next;
        }

        showPlaneList(allFlights); //To check if all flights are linked or not...
        printf("List of planes created \n");
        
        //Adding Flights to Buckets with modified timings
        bptr=firstBucket;

        int hr=allFlights->ETA.Hour;
        if (allFlights->ETA.Min > mins) hr--;

        Bucket*prevBucket;
        Flight*prevFlight;

        while(allFlights!=NULL)
        {
            printf("This goes infinite\n");
            if(bptr==NULL)
            {
                bptr=createBucket(123);
                prevBucket->next=bptr;
            }
            
            bptr->beginningETA.Hour=hr;
            bptr->endETA.Hour=hr;
            bptr->beginningETA.Min=bptr->beginningETA.Min + mins;
            bptr->endETA.Min=bptr->endETA.Min + mins;
            bptr->flights = allFlights;

            if(bptr->endETA.Min >= 60)
            {
                bptr->endETA.Hour=(bptr->endETA.Hour + 1) % 24;
                bptr->endETA.Min=bptr->endETA.Min - 60;
            }
            
            while(allFlights!=NULL && compareTime(bptr->beginningETA,allFlights->departureTime) && compareTime(allFlights->departureTime,bptr->endETA))
            {    
                prevFlight=allFlights;
                allFlights=allFlights->next;
            }

            hr++;
            prevFlight->next=NULL;
            prevBucket=bptr;
            bptr=bptr->next;
        }     
    }
    showBucketList(firstBucket);
}

int main()
{
    int tempFlightID;

    Time tempDepartureTime, tempETA;
    FILE *fptr = fopen ("FlightData.csv", "r");

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
    
    showMenu(ActiveBuckets);
    //rearrangeBuckets(ActiveBuckets,30,0);
       
    return 0;
}