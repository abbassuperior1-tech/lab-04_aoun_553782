#include <iostream>
#include <string>
using namespace std;

// Each node is one song
class node{
public:
    int id;
    string name;
    int minutes;
    int seconds;
    node *next;   // next song
    node *prev;   // previous song
};

class Playlist{
public:
    node *head=NULL;      // first song
    node *tail=NULL;      // last song
    node *current=NULL;   // song that is playing now
    node *loc=NULL;       // used by search (found node or NULL)
    int length=0;         // total songs

    // free all nodes when program ends
    ~Playlist(){
        node *temp;
        while(head!=NULL){
            temp=head;
            head=head->next;
            delete temp;
        }
    }

    // list is empty if head is NULL
    bool isEmpty(){
        return head==NULL;
    }

    // print one song like  ID: 1 | Name | 3:05
    void PrintSong(node *n){
        cout<<"ID: "<<n->id<<" | "<<n->name<<" | "<<n->minutes<<":";
        if(n->seconds<10)
            cout<<"0";          // print 05 instead of 5
        cout<<n->seconds<<endl;
    }

    // search a song by id, loc will point to it (NULL if not found)
    void Search(int id){
        loc=head;
        while(loc!=NULL && loc->id!=id)
            loc=loc->next;
    }

    // Add Song: insert at the end
    void AddSong(int id, string name, int m, int s){
        Search(id);
        if(loc!=NULL){
            cout<<"This ID already exists."<<endl;
            return;
        }
        // make a new node and fill it
        node *newnode = new node();
        newnode->id = id;
        newnode->name = name;
        newnode->minutes = m;
        newnode->seconds = s;
        newnode->next = NULL;
        newnode->prev = NULL;

        if(isEmpty()){
            // first song, so head, tail and current are all this song
            head = newnode;
            tail = newnode;
            current = newnode;
        }
        else{
            newnode->prev = tail;   // new song points back to old last song
            tail->next = newnode;   // old last song points to new song
            tail = newnode;         // new song is the last now
        }
        length++;
        cout<<"Song added."<<endl;
    }

    // Delete Song by id
    void DeleteSong(int id){
        Search(id);
        if(loc==NULL){
            cout<<"Song not found."<<endl;
            return;
        }
        // if we delete the playing song, move current to its neighbour
        if(loc==current){
            if(loc->next!=NULL)
                current=loc->next;
            else
                current=loc->prev;   // NULL if it was the only song
        }
        // connect the song before loc with the song after loc
        if(loc->prev!=NULL)
            loc->prev->next=loc->next;
        else
            head=loc->next;          // loc was the first song
        if(loc->next!=NULL)
            loc->next->prev=loc->prev;
        else
            tail=loc->prev;          // loc was the last song
        delete loc;
        loc=NULL;
        length--;
        cout<<"Song deleted."<<endl;
    }

    // Display from first to last
    void DisplayForward(){
        if(isEmpty()){
            cout<<"Playlist is empty."<<endl;
            return;
        }
        node *temp=head;
        while(temp!=NULL){
            PrintSong(temp);
            temp=temp->next;
        }
    }

    // Display from last to first
    void DisplayBackward(){
        if(isEmpty()){
            cout<<"Playlist is empty."<<endl;
            return;
        }
        node *temp=tail;
        while(temp!=NULL){
            PrintSong(temp);
            temp=temp->prev;
        }
    }

    // Search a song and show its details
    void SearchSong(int id){
        Search(id);
        if(loc==NULL)
            cout<<"Song not found."<<endl;
        else{
            cout<<"Song found: ";
            PrintSong(loc);
        }
    }

    // show the song that is playing
    void NowPlaying(){
        if(current==NULL)
            cout<<"No song is playing."<<endl;
        else{
            cout<<"Now playing: ";
            PrintSong(current);
        }
    }

    // Play next song
    void PlayNext(){
        if(isEmpty()){
            cout<<"Playlist is empty."<<endl;
            return;
        }
        if(current->next==NULL){
            cout<<"This is the last song."<<endl;
            return;
        }
        current=current->next;
        NowPlaying();
    }

    // Play previous song
    void PlayPrevious(){
        if(isEmpty()){
            cout<<"Playlist is empty."<<endl;
            return;
        }
        if(current->prev==NULL){
            cout<<"This is the first song."<<endl;
            return;
        }
        current=current->prev;
        NowPlaying();
    }

    // Reverse the list by swapping next and prev of every node
    void Reverse(){
        if(isEmpty()){
            cout<<"Playlist is empty."<<endl;
            return;
        }
        node *cur=head;
        node *temp=NULL;
        while(cur!=NULL){
            temp=cur->prev;       // swap prev and next
            cur->prev=cur->next;
            cur->next=temp;
            cur=cur->prev;        // old next is now in prev, so go there
        }
        // swap head and tail
        temp=head;
        head=tail;
        tail=temp;
        cout<<"Playlist reversed."<<endl;
    }
};

void ShowMenu(){
    cout<<"\n===== PLAYLIST MENU ====="<<endl;
    cout<<"1. Add Song"<<endl;
    cout<<"2. Delete Song"<<endl;
    cout<<"3. Display Forward"<<endl;
    cout<<"4. Display Backward"<<endl;
    cout<<"5. Search Song"<<endl;
    cout<<"6. Play Next"<<endl;
    cout<<"7. Play Previous"<<endl;
    cout<<"8. Show Current Song"<<endl;
    cout<<"9. Reverse Playlist"<<endl;
    cout<<"0. Exit"<<endl;
    cout<<"Enter choice: ";
}

int main(){
    Playlist p;
    int choice=-1;

    while(choice!=0){
        ShowMenu();
        if(!(cin>>choice)){       // user typed something that is not a number
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid input."<<endl;
            choice=-1;
            continue;
        }

        if(choice==1){
            int id, m, s;
            string name;
            cout<<"Song ID: ";
            cin>>id;
            cin.ignore(1000,'\n');      // remove the extra newline
            cout<<"Song Name: ";
            getline(cin,name);          // getline so the name can have spaces
            cout<<"Minutes: ";
            cin>>m;
            cout<<"Seconds: ";
            cin>>s;
            if(m<0 || s<0 || s>59)
                cout<<"Invalid duration."<<endl;
            else
                p.AddSong(id,name,m,s);
        }
        else if(choice==2){
            int id;
            cout<<"Enter ID to delete: ";
            cin>>id;
            p.DeleteSong(id);
        }
        else if(choice==3)
            p.DisplayForward();
        else if(choice==4)
            p.DisplayBackward();
        else if(choice==5){
            int id;
            cout<<"Enter ID to search: ";
            cin>>id;
            p.SearchSong(id);
        }
        else if(choice==6)
            p.PlayNext();
        else if(choice==7)
            p.PlayPrevious();
        else if(choice==8)
            p.NowPlaying();
        else if(choice==9)
            p.Reverse();
        else if(choice==0)
            cout<<"Goodbye!"<<endl;
        else
            cout<<"Invalid choice."<<endl;
    }
    return 0;
}