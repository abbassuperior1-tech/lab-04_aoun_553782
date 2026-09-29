#include <iostream>
#include <string>
using namespace std;

// Each node stores one bit (0 or 1)
class node{
public:
    int bit;
    node *next;
    node *prev;
};

// A binary number stored in a doubly linked list.
// head = left most bit (MSB), tail = right most bit (LSB).
// The number is always kept in groups of 8 bits.
class BinaryNumber{
public:
    node *head=NULL;
    node *tail=NULL;
    int length=0;      // total bits

    // free all nodes when the object is destroyed
    ~BinaryNumber(){
        Clear();
    }

    bool isEmpty(){
        return head==NULL;
    }

    // delete all nodes
    void Clear(){
        node *temp;
        while(head!=NULL){
            temp=head;
            head=head->next;
            delete temp;
        }
        tail=NULL;
        length=0;
    }

    // add a bit at the right end (LSB side)
    void InsertAtEnd(int b){
        node *newnode = new node();
        newnode->bit = b;
        newnode->next = NULL;
        newnode->prev = NULL;
        if(isEmpty()){
            head = newnode;
            tail = newnode;
        }
        else{
            newnode->prev = tail;
            tail->next = newnode;
            tail = newnode;
        }
        length++;
    }

    // add a bit at the left end (MSB side)
    void InsertAtBegin(int b){
        node *newnode = new node();
        newnode->bit = b;
        newnode->next = NULL;
        newnode->prev = NULL;
        if(isEmpty()){
            head = newnode;
            tail = newnode;
        }
        else{
            newnode->next = head;
            head->prev = newnode;
            head = newnode;
        }
        length++;
    }

    // add zeros on the left until the length is a multiple of 8
    void MakeBlocksOf8(){
        while(length%8!=0)
            InsertAtBegin(0);
    }

    // Store Binary Number: take a string of bits and store it in the list
    // returns false if the input is not a valid binary number
    bool Store(string s){
        if(s.length()==0)
            return false;
        for(int i=0;i<(int)s.length();i++){
            if(s[i]!='0' && s[i]!='1')
                return false;
        }
        Clear();                         // remove the old number
        for(int i=0;i<(int)s.length();i++)
            InsertAtEnd(s[i]-'0');       // '0' -> 0 and '1' -> 1
        MakeBlocksOf8();                 // extra zeros on the left
        return true;
    }

    // print the number in groups of 8 bits
    void Print(){
        if(isEmpty()){
            cout<<"(empty)"<<endl;
            return;
        }
        node *temp=head;
        int count=0;
        while(temp!=NULL){
            cout<<temp->bit;
            count++;
            if(count%8==0 && temp->next!=NULL)
                cout<<" ";               // space after every 8 bits
            temp=temp->next;
        }
        cout<<endl;
    }

    // make this number a copy of another number
    void CopyFrom(BinaryNumber &other){
        Clear();
        node *temp=other.head;
        while(temp!=NULL){
            InsertAtEnd(temp->bit);
            temp=temp->next;
        }
    }

    // 1's Complement: flip every bit
    void OnesComplement(){
        node *temp=head;
        while(temp!=NULL){
            if(temp->bit==0)
                temp->bit=1;
            else
                temp->bit=0;
            temp=temp->next;
        }
    }

    // add 1 to this number (start from the last bit and move to the left)
    // if a carry comes out from the first bit it is dropped, so the size stays same
    void AddOne(){
        node *temp=tail;
        while(temp!=NULL){
            if(temp->bit==0){
                temp->bit=1;      // 0 + 1 = 1, no carry, we are done
                return;
            }
            temp->bit=0;          // 1 + 1 = 0 with carry, go left
            temp=temp->prev;
        }
    }

    // 2's Complement: 1's complement and then add 1
    void TwosComplement(){
        OnesComplement();
        AddOne();
    }

    // Binary Addition: this = a + b
    // we start from the last bit of both numbers and move to the left
    void Add(BinaryNumber &a, BinaryNumber &b){
        Clear();
        node *p=a.tail;
        node *q=b.tail;
        int carry=0;
        while(p!=NULL || q!=NULL || carry==1){
            int sum=carry;
            if(p!=NULL){
                sum=sum+p->bit;
                p=p->prev;
            }
            if(q!=NULL){
                sum=sum+q->bit;
                q=q->prev;
            }
            InsertAtBegin(sum%2);    // bit of the answer
            carry=sum/2;             // carry for the next bit
        }
        MakeBlocksOf8();
    }

    // shift left by one: add a 0 at the right end (same as multiply by 2)
    void ShiftLeft(){
        InsertAtEnd(0);
    }

    // Binary Multiplication: this = a * b  (repeated addition + shifting)
    // for every 1 bit of b (from right to left) we add the shifted a to the answer
    void Multiply(BinaryNumber &a, BinaryNumber &b){
        BinaryNumber answer;         // answer so far, starts from 0
        answer.InsertAtEnd(0);
        BinaryNumber shifted;        // a shifted to the left step by step
        shifted.CopyFrom(a);
        BinaryNumber temp;

        node *q=b.tail;
        while(q!=NULL){
            if(q->bit==1){
                temp.Add(answer,shifted);   // temp = answer + shifted
                answer.CopyFrom(temp);
            }
            shifted.ShiftLeft();            // next bit is worth double
            q=q->prev;
        }
        CopyFrom(answer);
        MakeBlocksOf8();
    }

    // Conversion to Decimal: go from left to right
    // value = value * 2 + bit
    unsigned long long ToDecimal(){
        unsigned long long value=0;
        node *temp=head;
        while(temp!=NULL){
            value=value*2+temp->bit;
            temp=temp->next;
        }
        return value;
    }
};

void ShowMenu(){
    cout<<"\n===== BINARY CALCULATOR ====="<<endl;
    cout<<"1. Store number A"<<endl;
    cout<<"2. Store number B"<<endl;
    cout<<"3. Show A and B"<<endl;
    cout<<"4. 1's Complement"<<endl;
    cout<<"5. 2's Complement"<<endl;
    cout<<"6. Add (A + B)"<<endl;
    cout<<"7. Multiply (A * B)"<<endl;
    cout<<"8. Convert to Decimal"<<endl;
    cout<<"0. Exit"<<endl;
    cout<<"Enter choice: ";
}

// ask the user to pick A or B and return a pointer to that number
BinaryNumber* Pick(BinaryNumber &A, BinaryNumber &B){
    char c;
    cout<<"Which number? (A or B): ";
    cin>>c;
    if(c=='A' || c=='a')
        return &A;
    if(c=='B' || c=='b')
        return &B;
    cout<<"Wrong choice."<<endl;
    return NULL;
}

int main(){
    BinaryNumber A, B;
    int choice=-1;

    while(choice!=0){
        ShowMenu();
        if(!(cin>>choice)){
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid input."<<endl;
            choice=-1;
            continue;
        }

        if(choice==1 || choice==2){
            string s;
            cout<<"Enter binary number (only 0 and 1): ";
            cin>>s;
            bool ok;
            if(choice==1)
                ok=A.Store(s);
            else
                ok=B.Store(s);
            if(ok)
                cout<<"Number stored."<<endl;
            else
                cout<<"Invalid binary number."<<endl;
        }
        else if(choice==3){
            cout<<"A = ";
            A.Print();
            cout<<"B = ";
            B.Print();
        }
        else if(choice==4 || choice==5){
            BinaryNumber *p=Pick(A,B);
            if(p!=NULL && !p->isEmpty()){
                BinaryNumber result;
                result.CopyFrom(*p);       // work on a copy so the original stays same
                if(choice==4)
                    result.OnesComplement();
                else
                    result.TwosComplement();
                cout<<"Original: ";
                p->Print();
                cout<<"Result  : ";
                result.Print();
            }
            else if(p!=NULL)
                cout<<"That number is empty. Store it first."<<endl;
        }
        else if(choice==6 || choice==7){
            if(A.isEmpty() || B.isEmpty())
                cout<<"Store both A and B first."<<endl;
            else{
                BinaryNumber result;
                if(choice==6)
                    result.Add(A,B);
                else
                    result.Multiply(A,B);
                cout<<"A      = ";
                A.Print();
                cout<<"B      = ";
                B.Print();
                cout<<"Result = ";
                result.Print();
                cout<<"Decimal: "<<A.ToDecimal();
                if(choice==6)
                    cout<<" + ";
                else
                    cout<<" * ";
                cout<<B.ToDecimal()<<" = "<<result.ToDecimal()<<endl;
            }
        }
        else if(choice==8){
            BinaryNumber *p=Pick(A,B);
            if(p!=NULL && !p->isEmpty()){
                p->Print();
                cout<<"Decimal = "<<p->ToDecimal()<<endl;
            }
            else if(p!=NULL)
                cout<<"That number is empty. Store it first."<<endl;
        }
        else if(choice==0)
            cout<<"Goodbye!"<<endl;
        else
            cout<<"Invalid choice."<<endl;
    }
    return 0;
}