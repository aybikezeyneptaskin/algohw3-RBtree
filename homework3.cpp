#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

int NumProcesses,SimulatorRunTime;
ifstream inputfile;
ofstream outfile;
string lines;
char separator=' ';
bool IsAllCompleted=false;
vector<string> completedvector;

struct Process{
        string ProcessID; //int mi alayım?
        int TimeOfArrival;
        int BurstTime;
        int TaskVrunTime;
        bool Completed;
    public:
        Process(string processID, int timeOfArrival, int burstTime, int taskVrunTime, bool completed){
            ProcessID=processID;
            TimeOfArrival=timeOfArrival;
            BurstTime=burstTime;
            TaskVrunTime=taskVrunTime;
            Completed=completed;
        }
};

struct Node {
        Node *parent, *left, *right;
        int color; //red=1 and black=0
        Process *nodeProcess;
    public:
        Node(Process *processnode,Node* Parent){
            nodeProcess=processnode;
            right=NULL;
            left=NULL;
            parent=Parent;
        }
        Node(){
            nodeProcess=NULL;
            right=NULL;
            left=NULL;
            parent=NULL;
        }
};

class RB {
    public:
        Node *nil;
        Node *root;
        RB(){
            nil = new Node();
            root = nil;
            nil->left=nil;
            nil->right=nil;
            nil->parent=nil;
            nil->color=0;
        }
        //void NullNodes(Node*, Node*);
        void Insertion(Process *);
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
        void RBTraverse(Node*);
        //bool IsTreeEmpty(RB*);
};


void RB::display(){
    displayall(root);
}
void RB::displayall(Node *node){
    if(root==nil){
        cout<<"tree is empty!"<<endl;
        return;
    }
    if(node!=NULL){
        cout<<"\n\t NODE: ";
        cout<<"\n Data: "<<node->nodeProcess->TaskVrunTime;
        cout<<"\n Color: ";
    }
    if(node->color==0){
        cout<<"black";
    }else cout<<"red";
                 if(node->parent!=NULL)
                       cout<<"\n Parent: "<<node->parent->nodeProcess->TaskVrunTime;
                else
                       cout<<"\n There is no parent of the node.  ";
                if(node->right!=NULL)
                       cout<<"\n Right Child: "<<node->right->nodeProcess->TaskVrunTime;
                else
                       cout<<"\n There is no right child of the node.  ";
                if(node->left!=NULL)
                       cout<<"\n Left Child: "<<node->left->nodeProcess->TaskVrunTime;
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
    nodeinit->left = nil;
    nodeinit->right = nil;
    root = nodeinit;
}

void RB::NullNodes(Node* nodeinit, Node * parent){
    nodeinit = NULL;
    root = NULL;
    node->data = 0;
    node->color = 0; //black
    node->left = nil;
    node->right = nil;
    node->parent = parent;
}
*/
/*
void RB::RBPrint(){
    if(root!=nil){
        //cout<<"root: "<<root->data<<endl;
        printall(this->root, "", true);
    }
}

void RB::printall(Node* root, string indentation, bool last){
    if(root!=nil){
        cout<<indentation;
        if(last){
            cout<<"R---";
            indentation+= "     ";
        }else{
            cout<<"L---";
            indentation += "|     ";
        }

        string rbcolor = root->color ? "RED":"BLACK";
        cout<<root->nodeProcess->TaskVrunTime<<"("<<rbcolor<<")"<<endl;
        printall(root->left, indentation, false);
        printall(root->right, indentation, true);
    }
}
*/
void RB::Transplant(Node *u, Node*v){
    if(u->parent == nil){
        root = v;
    }
    else if(u==u->parent->left){
        u->parent->left=v;
    }
    else{
        u->parent->right=v;
    }v->parent = u->parent;
}

void RB::Deletion(Node *z){
    if(root==nil) return;
    //Node*node=NULL;
    Node *y=z;
    //y-original-color = y->color
    int y_orig_color = y->color;
    Node *x;
    if(z->left == nil){
        x=z->right;
        Transplant(z,z->right);
    }
    else if(z->right==nil){
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
    //delete z;//??
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

    }
    x->color = 0; //black
}

Node *RB::FindMin(Node *node){
    while(node->left != nil){
        node=node->left;
    }
    return node;
}

void RB::Insertion(Process * node_process){
    cout<<"new: "<<node_process->ProcessID<<" "<<node_process->BurstTime<<endl;
    Node* z = new Node(node_process,z->parent);
    /*
    z->parent = nil;
    z->nodeProcess->TaskVrunTime=node_process->TaskVrunTime;
    z->left = nil; //nil
    z->right = nil; //nil
    z->color = 1; //red
    */
    Node *y = nil;
    Node *x = root;
    while(x!=nil){
        y = x;
        if(z->nodeProcess->TaskVrunTime < x->nodeProcess->TaskVrunTime){
            x=x->left;  
        } 
        else{
            x = x->right;  
        } 
        }
        z->parent = y;
        if(y==nil){
            root = z;
        }else if(z->nodeProcess->TaskVrunTime < y->nodeProcess->TaskVrunTime){
            y->left = z;
        }
        else{
            y->right = z;
        }
        /*
        if(z->parent==nil){
            z->color = 0;
            return;
        }
        if(z->parent->parent==nil){
            return;
        }*/
    

    z->left = nil;
    z->right = nil;
    z->color = 1; //red
    Fixup_Insertion(z);
}

void RB::Fixup_Insertion(Node*z){
    Node *y;
    while(z->parent->color==1){
        if(z->parent == z->parent->parent->left){
            y=z->parent->parent->right;
            if(y->color==1){
                z->parent->color=0;//black
                y->color=0;//black
                z->parent->parent->color=1;//red
                z=z->parent->parent;
            }
            else if(z==z->parent->right){
                    z=z->parent;
                    LeftRotate(z);
            }
            z->parent->color=0;//black
            z->parent->parent->color=1;//red
            RightRotate(z->parent->parent);
            
        }
        else{
            y=z->parent->parent->left;
            if(y->color==1){
                z->parent->color=0;//black
                y->color=0;//black
                z->parent->parent->color=1;//red
                z=z->parent->parent;
            }
            else if(z==z->parent->left){
                    z=z->parent;
                    RightRotate(z);
            }
            z->parent->color=0;//black
            z->parent->parent->color=1;//red
            LeftRotate(z->parent->parent);
            
        }
        
    }root->color=0;//black
}

void RB::LeftRotate(Node *x){
    Node * y = x->right;
    x->right = y->left;
    if(y->left!=nil){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent==nil){
        root = y;
    }
    else if(x==x->parent->left){
        x->parent->left = y;
    }else{
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
    
}

void RB::RightRotate(Node *x){
    Node * y = x->left;
    x->left = y->right;
    if(y->right!=nil){
        y->right->parent=x;
    }
    y->parent = x->parent;
    if(x->parent==nil){
        root = y;
    }
    else if(x==x->parent->right){
            x->parent->right=y;
        }
    else{
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
    
}

/*
bool IsTreeEmpty(Node * root){
    if(root==NULL)
        return true;
    else
       return false;
}
*/
void RB::RBTraverse(Node*root1){
    if(root1==nil)
        return;
    else{
        RBTraverse(root1->left);
        string nodecolor;
        if(root1->color==0){
            nodecolor="Black";
        }else{
            nodecolor="Red";
        }
        outfile<< root1->nodeProcess->ProcessID<<":"<<root1->nodeProcess->TaskVrunTime<<"-"<<nodecolor<<";";
        RBTraverse(root1->right);
    }
}

int main(int argc, char*argv[]){ 
    inputfile.open("input.txt");
    if(!inputfile){
        cerr << "input.txt file could not be opened!";
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
            //cout<<"NumProcesses:"<<NumProcesses<<endl;
            //cout<<"Simulatorruntime:"<<SimulatorRunTime<<endl;
            string data[3];
            Process *processes[NumProcesses];
            //int vectorindex=0;
        for(int i=0;i<NumProcesses;i++){
            
            getline(inputfile, lines); // ProcessID,TimeOfArrival, BurstTime
            stringstream linedata(lines);
            for(int x=0;x<3;x++){
                getline(linedata,data[x],separator);
            }
            Process *p = new Process(data[0], stoi(data[1]), stoi(data[2]), 0, false);
            p->ProcessID = data[0];
            p->TimeOfArrival = stoi(data[1]);
            p->BurstTime = stoi(data[2]);
            p->TaskVrunTime=0;
            p->Completed=false;
            processes[i]=p;
            
            //cout<<p->ProcessID<<" "<<p->TimeOfArrival<<" "<<p->BurstTime<<" "<<endl;
        }
        inputfile.close();

        
        ofstream outfile("output.txt");
        if (!outfile){
            cerr << "output.txt file could not be opened!";
            exit(1);
        }    
        clock_t time = clock();

        RB tree_processes;
        Node * RunningTask=NULL;
        int min_vruntime=0;
        int runtime=0;
        //repeat until alloted time is finished or all processes ended.
        while(runtime!=SimulatorRunTime || !IsAllCompleted){
            //if the process arrival time has come, insert to the tree+
            for(int i=0;i<NumProcesses;i++){
                if(processes[i]->TimeOfArrival==runtime){
                    tree_processes.Insertion(processes[i]);
                }
            }
            //if no task is running->choose the one with smalllest vruntime. 
            //remove that from the tree-> increment it's vruntime. check if it should stop running or not
            if(tree_processes.root!=tree_processes.nil){
                min_vruntime = tree_processes.FindMin(tree_processes.root)->nodeProcess->TaskVrunTime;
                if(RunningTask==NULL){
                    Node * smallest=tree_processes.FindMin(tree_processes.root); //FindMin doru mu idk??
                    tree_processes.Deletion(smallest); //Deletion opeariton kontrol et!
                    smallest->nodeProcess->TaskVrunTime++;
                    RunningTask=smallest;
                    if(smallest->nodeProcess->TaskVrunTime==smallest->nodeProcess->BurstTime){
                        //task is completed
                        smallest->nodeProcess->Completed=true;
                        completedvector.push_back(smallest->nodeProcess->ProcessID);
                        //completedvector[vectorindex]=smallest->nodeProcess->ProcessID;
                        //vectorindex++;
                        RunningTask=NULL;
                    }
                    else{
                        tree_processes.Insertion(smallest->nodeProcess);
                    }

                    //min_vruntime = tree_processes.FindMin(tree_processes.root)->nodeProcess->TaskVrunTime;
                    
                }else if(RunningTask->nodeProcess->TaskVrunTime > min_vruntime){
                    //select new smallest node
                    Node * smallest=tree_processes.FindMin(tree_processes.root);
                    tree_processes.Deletion(smallest); //Deletion opeariton kontrol et!
                    smallest->nodeProcess->TaskVrunTime++;
                    RunningTask=smallest;
                    if(smallest->nodeProcess->TaskVrunTime==smallest->nodeProcess->BurstTime){
                        //task is completed
                        smallest->nodeProcess->Completed=true;
                        completedvector.push_back(smallest->nodeProcess->ProcessID);
                        //completedvector[vectorindex]=smallest->nodeProcess->ProcessID;
                        //vectorindex++;
                        RunningTask=NULL;
                    }
                    else{
                        tree_processes.Insertion(smallest->nodeProcess);
                    }
                }//if RunningTask node is still smallest
                else{
                    RunningTask->nodeProcess->TaskVrunTime++;
                    if(RunningTask->nodeProcess->TaskVrunTime==RunningTask->nodeProcess->BurstTime){
                        //task is completed
                        RunningTask->nodeProcess->Completed=true;
                        completedvector.push_back(RunningTask->nodeProcess->ProcessID);
                        //completedvector[vectorindex]=RunningTask->nodeProcess->ProcessID;
                        //vectorindex++;
                        RunningTask=NULL;
                    }
                    else{
                        tree_processes.Insertion(RunningTask->nodeProcess); //tekrar insert etmeyi sadece başka smallest seçersem mi koysam?
                    }
                }

            
                //in insertion if it has the same vruntime, add the one that is added later as a right vchild of the current
                //bool IsAllCompleted =  tree_processes.IsTreeEmpty(tree_processes->root);
                if(tree_processes.root==tree_processes.nil)
                    IsAllCompleted=true;
                else    
                    IsAllCompleted=false;
                
                //CurrTime, RunningTask, TaskVruntime, MinVruntime, RBTTraversal, TaskStatus
                outfile<<runtime<<","<<RunningTask->nodeProcess->ProcessID<<","<<RunningTask->nodeProcess->TaskVrunTime<<",";
                outfile<<min_vruntime;
                tree_processes.RBTraverse(tree_processes.root);
                string completestring;
                if(RunningTask->nodeProcess->Completed==true){
                    completestring=",Complete";
                }else{
                    completestring=",Incomplete";
                }
                outfile<<completestring<<endl;

            }

            //if no insertions have been made
            else{
                outfile<<runtime<<",-,-,-,-,-,-"<<endl;
            }

            runtime++;
        }
        outfile<<endl;
        time = clock() - time; 
        float total_time = 1000*(float)time / CLOCKS_PER_SEC;
        outfile<<"Scheduling finished in "<<total_time <<"ms."<<endl;
        outfile<<completedvector.size()<<" of "<<NumProcesses<<"  are completed."<<endl;
        outfile<<"The order of completion of the tasks: ";
        for(int i=0;i<completedvector.size();i++){
            outfile<<completedvector[i];
            if(i!=completedvector.size()){
                outfile<<"-";
            }
        }
        outfile<<endl; //ADD ORDER OF COMPLETION
        
    } 
   
   /*
    
    RB tree;
    Process *p1 = new Process;
    p1->TaskVrunTime=11;
        Process *p2 = new Process;
    p2->TaskVrunTime=2;
        Process *p3 = new Process;
    p3->TaskVrunTime=14;
        Process *p4 = new Process;
    p4->TaskVrunTime=17;
        Process *p5 = new Process;
    p5->TaskVrunTime=15;
            Process *p6 = new Process;
    p6->TaskVrunTime=5;
            Process *p7 = new Process;
    p7->TaskVrunTime=8;
    tree.Insertion(p1);
    tree.Insertion(p2);
    tree.Insertion(p3);
    tree.Insertion(p4);
    tree.Insertion(p5);
    tree.Insertion(p6);
    tree.Insertion(p7);
    //tree.display();
    //tree.RBPrint();
    tree.FindMin(tree.root);
    */
    return 0;
}


