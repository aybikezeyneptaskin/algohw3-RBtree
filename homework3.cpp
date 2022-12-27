#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

struct Node {
    Node *parent, *left, *right;
    float data;
    int color; //red=1 and black=0
};

class RB {
    private:
        Node *root;
        Node *nil;
        Node *nodeinit;
    public:
        RB(){
            root = NULL;
            nodeinit = NULL;
        }
        void NullNodes(Node*, Node*);
        void Insertion(float);
        void Deletion(Node*);
        void Transplant(Node*,Node*);
        void LeftRotate(Node*);
        void RightRotate(Node*);
        void Fixup_Insertion(Node*);
        void Fixup_Deletion(Node*);
        Node * FindMin(Node*);
        void RBPrint();
        void printall(Node*, string, bool);
        void display();
        void displayall(Node*);
};

void RB::display(){
    displayall(root);
}
void RB::displayall(Node *node){
    if(root==nullptr){
        cout<<"tree is empty!"<<endl;
        return;
    }
    if(node!=NULL){
        cout<<"\n\t NODE: ";
        cout<<"\n Data: "<<node->data;
        cout<<"\n Color: ";
    }
    if(node->color==0){
        cout<<"black";
    }else cout<<"red";
                 if(node->parent!=NULL)
                       cout<<"\n Parent: "<<node->parent->data;
                else
                       cout<<"\n There is no parent of the node.  ";
                if(node->right!=NULL)
                       cout<<"\n Right Child: "<<node->right->data;
                else
                       cout<<"\n There is no right child of the node.  ";
                if(node->left!=NULL)
                       cout<<"\n Left Child: "<<node->left->data;
                else
                       cout<<"\n There is no left child of the node.  ";
                cout<<endl;
    if(node->left)
    {
                 cout<<"\n\nLeft:\n";
     displayall(node->left);
    }
    /*else
     cout<<"\nNo Left Child.\n";*/
    if(node->right)
    {
     cout<<"\n\nRight:\n";
                 displayall(node->right);
    }
    /*else
     cout<<"\nNo Right Child.\n"*/
}

/*
RB::RB(){
    nodeinit = new Node;
    nodeinit->color = 0;
    nodeinit->left = nullptr;
    nodeinit->right = nullptr;
    root = nodeinit;
}

void RB::NullNodes(Node* nodeinit, Node * parent){
    nodeinit = NULL;
    root = NULL;
    node->data = 0;
    node->color = 0; //black
    node->left = nullptr;
    node->right = nullptr;
    node->parent = parent;
}
*/
void RB::RBPrint(){
    if(root!=nullptr){
        //cout<<"root: "<<root->data<<endl;
        printall(this->root, "", true);
    }
}

void RB::printall(Node* root, string indentation, bool last){
    if(root!=nullptr){
        cout<<indentation;
        if(last){
            cout<<"R---";
            indentation+= "     ";
        }else{
            cout<<"L---";
            indentation += "|     ";
        }

        string rbcolor = root->color ? "RED":"BLACK";
        cout<<root->data<<"("<<rbcolor<<")"<<endl;
        printall(root->left, indentation, false);
        printall(root->right, indentation, true);
    }
}

void RB::Transplant(Node *u, Node*v){
    if(u->parent == nullptr){
        this->root = v;
    }
    else if(u=u->parent->left){
        u->parent->left=v;
    }
    else{
        u->parent->right=v;
    }v->parent = u->parent;
}

void RB::Deletion(Node *z){
    if(root==nullptr) return;
    Node *y=z;
    //y-originaal-color = y->color
    int y_orig_color = y->color;
    Node *x;
    if(z->left == nullptr){
        x=z->right;
        Transplant(z,z->right);
    }
    else if(z->right==nullptr){
        x=z->left;
        Transplant(z,z->left);
    }
    else{
        //y=Tree_min(z->right)
        y = FindMin(z->right);
        //y-original-color = y->color
        y_orig_color = y->color;
        x=y->right;
        if(y->parent==z){
            x->parent=y;
        }
        else{
            Transplant(y,y->right);
            y->right=z->right;
            y->right->parent=y;
        }

        Transplant(z,y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    //delete
    if(y_orig_color == 0){
        Fixup_Deletion(x);
    }

}

void RB::Fixup_Deletion(Node *x){
    Node *w;
    while(x!=root && x->color ==0){
        if(x==x->parent->left){
            w=x->parent->right;
            if(w->color==1){
                w->color=0;//black
                x->parent->color=1;//red
                LeftRotate(x->parent);
                w=x->parent->right;
            }
            if(w->left->color == 0 && w->right->color==0){
                w->color==1;//red
                x=x->parent; 
            }else{
                if(w->right->color==0){
                    w->left->color=0;//black
                    w->color=1;//red
                    RightRotate(w);
                    w=x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = 0;//black
                w->right->color = 0;//black
                LeftRotate(x->parent);
                x=root;
            }
        }
        else{
            w=x->parent->left;
            if(w->color==1){
                w->color=0;//black
                x->parent->color=1;//red
                RightRotate(x->parent);
                w=x->parent->left;
            }
            if(w->right->color == 0 && w->left->color==0){
                w->color==1;//red
                x=x->parent; 
            }else{
                if(w->left->color==0){
                    w->right->color=0;//black
                    w->color=1;//red
                    LeftRotate(w);
                    w=x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = 0;//black
                w->left->color = 0;//black
                RightRotate(x->parent);
                x=root;
            }
        }
        x->color = 0; //black
        //root->color=0;
    }
}

Node *RB::FindMin(Node *node){
    while(node->left != nullptr){
        node=node->left;
    }
    return node;
}

void RB::Insertion(float newdata){
    cout<<"new: "<<newdata<<endl;
    Node* z = new Node;
    z->parent = nullptr;
    z->data=newdata;
    z->left = nullptr; //nil
    z->right = nullptr; //nil
    z->color = 1; //red
    Node *y = NULL;
    Node *x = root;
    if(root==NULL){
        root=z;
        z->parent=NULL;
    }
    else{
        while(x!=NULL){
            y = x;
            if(z->data < x->data){
                x=x->left;  
            } 
            else{
                x = x->right;  
            } 
        }
        z->parent = y;
        if(y==nullptr){
            root = z;
        }else if(z->data < y->data){
            y->left = z;
        }
        else{
            y->right = z;
        }
        /*
        if(z->parent==nullptr){
            z->color = 0;
            return;
        }
        if(z->parent->parent==nullptr){
            return;
        }*/
    }

    //z->left = nullptr;
    //z->right = nullptr;
    //z->color = 1; //red
    Fixup_Insertion(z);
}

void RB::Fixup_Insertion(Node*z){
    cout<<"fixup "<<endl;
    Node *y;
    if(root==z){
        z->color=0;//black
        return;
    }
    while(z->parent!=nullptr && z->parent->color==1){
        if(z->parent == z->parent->parent->left){
            if(z->parent->parent->right != nullptr){
                y=z->parent->parent->right;
                if(y->color==1){
                    z->parent->color=0;//black
                    y->color=0;//black
                    z->parent->parent->color=1;//red
                    z=z->parent->parent;
                }
            }
            else{
                if(z==z->parent->right){
                    z=z->parent;
                    LeftRotate(z);
                }
                z->parent->color=0;//black
                z->parent->parent->color=1;//red
                RightRotate(z->parent->parent);
            }
            
        }
        else{
            if(z->parent->parent->left!=nullptr){
                y=z->parent->parent->left;
                if(y->color==1){
                    z->parent->color=0;//black
                    y->color=0;//black
                    z->parent->parent->color=1;//red
                    z=z->parent->parent;
                }
            }
            else{
                if(z==z->parent->left){
                    z=z->parent;
                    RightRotate(z);
                }
                z->parent->color=0;//black
                z->parent->parent->color=1;//red
                LeftRotate(z->parent->parent);
            }
        }
        root->color=0;//black
    }
}

void RB::LeftRotate(Node *x){
    if(x->right==nullptr){
        return;
    }
    else{
        Node * y = x->right;
        if(y->left!=nullptr){
            x->right = y->left;
            y->left->parent = x;
        }
        else{
            x->right=nullptr;
        }
        if(x->parent!=nullptr){
            y->parent = x->parent;
        }
        if(x->parent==nullptr){
            root = y;
        }
        else{
            if(x==x->parent->left){
                x->parent->left = y;
            }else{
                x->parent->right = y;
            }
        }
            y->left = x;
            x->parent = y;
    }
}

void RB::RightRotate(Node *x){
    if(x->left==nullptr){
        return;
    }
    else{
        Node * y = x->left;
        if(y->right!=nullptr){
            x->left = y->right;
            y->right->parent=x;
        }
        else{
            x->left=nullptr;
        }
        if(x->parent!=nullptr){
            y->parent = x->parent;
        }
        if(x->parent==nullptr){
            root = y;
        }
        else{
            if(x==x->parent->left){
                x->parent->left=y;
            }
            else{
                x->parent->right = y;
            }
        }
        y->right = x;
        x->parent = y;
    }
}

int NumProcesses,SimulatorRunTime;
ifstream inputfile;
ofstream outfile;
string lines;
char separator=' ';

struct Process{
    string ProcessID; //int mi alayım?
    int TimeOfArrival;
    int BurstTime;
};

int main(int argc, char*argv[]){ 
    inputfile.open("input.txt");
    if(!inputfile){
        cerr << "input1.txt file could not be opened!";
        exit(1);
    }
    while(!inputfile.eof()){
        string firstline[2];
        getline(inputfile, lines); //first line = NumProcesses SimulatorRunTime
        stringstream line(lines);
        for(int j=0;j<2;j++){
            getline(line,firstline[j],separator);
        }
            NumProcesses=stoi(firstline[0]);
            SimulatorRunTime=stoi(firstline[1]);
            cout<<"NumProcesses:"<<NumProcesses<<endl;
            cout<<"Simulatorruntime:"<<SimulatorRunTime<<endl;
            string data[3];
        for(int i=0;i<NumProcesses;i++){
            Process *p = new Process;
            getline(inputfile, lines); // ProcessID,TimeOfArrival, BurstTime
            stringstream linedata(lines);
            for(int x=0;x<3;x++){
                getline(linedata,data[x],separator);
            }
            p->ProcessID = data[0];
            p->TimeOfArrival = stoi(data[1]);
            p->BurstTime = stoi(data[2]);
        }
    }
    /*
    RB tree;
    tree.Insertion(11);
    tree.Insertion(2);
    tree.Insertion(14);
    tree.Insertion(17);
    tree.Insertion(15);
    tree.Insertion(5);
    tree.Insertion(8);
    tree.display();
    //tree.RBPrint();
    */
    return 0;
}


