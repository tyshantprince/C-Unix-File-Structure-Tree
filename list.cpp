#include <iostream>
#include <fstream>
#include <string>


class dirTree{
public:
    class dirNode{
    public:
        dirNode(std::string p, std::string n): path(p), name(n){}
        void addChild(std::string p, std::string n){
            firstChild = new dirNode(p, n);
        };
        void addSibling(std::string p, std::string n){
            nextSibling = new dirNode(p, n);
        };
        void print(int level){
            for(int i=0; i<level*5;i++) std::cout << " ";
            std::cout << this->name << std::endl;
            dirNode *child = this->firstChild;
            while(child != NULL){
                child->print(level+1);
                child = child->nextSibling;
            }

        };
        dirNode* findNode(std::string pathname){
            if(pathname.back() == ':')
                pathname = pathname.substr(0,pathname.length()-1);
            if(pathname[0] == '/')
                pathname = pathname.substr(1);
            int i = pathname.find("/");
            std::string dir = pathname.substr(0, i);
            if(this->name != dir) return NULL;
            if(i==-1) return this;
            pathname = pathname.substr(i+1);
            dirNode *node, *child = this->firstChild;
            while(child != NULL){
                node = child->findNode(pathname);
                if(node != NULL) return node;
                child = child->nextSibling;
            }
            return NULL;

        };

        dirNode *firstChild = NULL;
        dirNode *nextSibling = NULL;
        std::string path;
        std::string name;

    };


    void insert(std::string path, std::string name){

        if(name == ":")
            return;
        if(this->root == NULL){
                path = path.substr(0, path.length()-1);
            this->root = new dirNode(path, name);
            return;
        }
        if(this->root->findNode(path)){
            return;
        }
        dirNode *temp = this->root;
        while(true){
            if(path.find(temp->path) != std::string::npos) {
                if (temp->firstChild == NULL) {
                    temp->addChild(path, name);
                    return;
                }
                temp = temp->firstChild;
            }
            else {
                if (temp->nextSibling == NULL){
                    temp->addSibling(path, name);
                    return;
                }
                temp = temp->nextSibling;
            }
        }



       }

       void printTree(){
        this->root->print(1);
    }
    dirNode * root;
};


class dirTokenizer{
public:
    dirTokenizer(std::string fullname): fullname(fullname+"/"), i(0), numOfFolders(std::count(fullname.begin(), fullname.end(), '/')){};
    std::string next(){
        int j = fullname.find("/", i+1);
        if(j == -1) return "";
        std::string name = fullname.substr(i+1, j-i-1);
        numOfFolders--;
        i = j;
        return name;
    };

    bool isEmpty(){ // helper method that determines if the entire path has been read 
        if(numOfFolders == 0)
            return true;
        else
            return false;
    };

    std::string fullname;
    int i;
    int numOfFolders;

};

int main(){

    dirTree *dirTree1 = new dirTree();

    std::ifstream inStream("ITUnix");
    std::string aline;
    std::string currentPath;

    if(inStream.is_open()){
        for(int i = 0; i < 1000; i++)  // only work with the first 20 lines while testing
        {
            getline(inStream, aline);
            if(aline.back() == ':'){ // if last char in line is a : , it is a directory
                currentPath = aline.substr(0, aline.length()-1);

                // create tokenizer of path name
                dirTokenizer *dirTokenizer1 = new dirTokenizer(aline);
                while(!dirTokenizer1->isEmpty()){
                    dirTree1->insert(aline, dirTokenizer1->next());
                }
            }
            else if(aline == ""){
            }
            else if(aline != ""){
                if(aline.find("/") != std::string::npos)
                    aline = "/" + aline;
                dirTree1->insert(currentPath+aline,aline);
            }
        }
    }

    dirTree1->printTree();


    inStream.close();

    return 0;


}



