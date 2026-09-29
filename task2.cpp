#include <iostream>
using namespace std;

// Each node is one person
class node{
public:
    int id;
    node *next;
};

class Josephus{
public:
    node *list=NULL;   // points to the last person (list->next is the first person)
    int length=0;      // people left in the circle
    node *loc;         // the person who is going to be eliminated
    node *ploc;        // the person just before loc

    // Create Circle: make N people (1, 2, ..., N) and join them in a circle
    void CreateCircle(int n){
        for(int i=1;i<=n;i++){
            node *newnode = new node();
            newnode->id = i;
            if(list==NULL){
                newnode->next = newnode;    // first person points to himself
                list = newnode;
            }
            else{
                newnode->next = list->next; // new person points to the first person
                list->next = newnode;       // old last person points to new person
                list = newnode;             // new person is the last now
            }
            length++;
        }
    }

    // print all people in the circle (just to check the circle)
    void PrintCircle(){
        if(list==NULL){
            cout<<"Circle is empty."<<endl;
            return;
        }
        node *temp=list->next;      // start from first person
        do{
            cout<<temp->id<<" ";
            temp=temp->next;
        } while(temp!=list->next);  // stop when we are back at the first person
        cout<<endl;
    }

    // Elimination Process + Display Eliminated Order
    void Eliminate(int k){
        ploc=list;          // person before person 1 is the last person
        loc=list->next;     // start counting from person 1

        cout<<"Eliminated order: ";
        while(length>1){
            // move k-1 steps, then loc is the k-th person
            for(int i=1;i<k;i++){
                ploc=loc;
                loc=loc->next;
            }
            cout<<loc->id<<" ";

            // remove loc from the circle
            ploc->next=loc->next;
            if(loc==list)       // if we removed the last node, move list back
                list=ploc;

            node *temp=loc;
            loc=ploc->next;     // next counting starts from the person after loc
            delete temp;
            length--;
        }
        cout<<endl;
    }

    // Display Survivor: only one person is left, list points to him
    void DisplaySurvivor(){
        cout<<"Survivor: "<<list->id<<endl;
    }

    // delete the last person so there is no memory leak
    void DestroyList(){
        if(list!=NULL){
            delete list;
            list=NULL;
            length=0;
        }
    }
};

int main(){
    int n, k;
    cout<<"Enter number of people (N): ";
    cin>>n;
    cout<<"Enter step count (k): ";
    cin>>k;

    if(n<1 || k<1){
        cout<<"N and k must be 1 or more."<<endl;
        return 0;
    }

    Josephus j;
    j.CreateCircle(n);
    cout<<"Circle: ";
    j.PrintCircle();

    j.Eliminate(k);
    j.DisplaySurvivor();
    j.DestroyList();
    return 0;
}