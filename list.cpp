#include <iostream>
#include <fstream>
#include <string>
#include <time.h>


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

        if(name == "")
            return;
        if(this->root == NULL){
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

       void printTree(std::string path = "/home" ){
        dirNode *temp = root->findNode(path);
        temp->print(1);
    }
    dirNode * root;
};




int main(int argc, char* argv[]){
    clock_t tStart = clock();

    dirTree *dirTree1 = new dirTree();

    std::ifstream inStream("ITUnix");
    std::string aline;

    std::string currentPath;

    if(inStream.is_open()){
        for(std::string aline; getline(inStream, aline);)
//        for(int i =0; i < 2000; i ++)
        {
//            getline(inStream, aline);
            if(aline.back() == ':'){ // if last char in aline is a : , it is a directory
                currentPath = aline.substr(0, aline.length()-1);
                if(currentPath == "/home/")
                    currentPath = currentPath.substr(0, currentPath.length()-1);

                // create tokenizer of path name
                dirTokenizer *dirTokenizer1 = new dirTokenizer(currentPath);
                while(!dirTokenizer1->isEmpty()){
                    dirTree1->insert(currentPath, dirTokenizer1->next());
                }
            }
            else if(aline == ""){
                continue;
            }
            else if(aline != ""){
                dirTree1->insert(currentPath + "/" + aline,aline);
            }
        }
    }


    if(!dirTree1->root->findNode(argv[1])){
        std::cout << "This path does not exist";
        return 0;
    }

    std::string arg(argv[1]);
    if(arg.back() == '/')
        arg = arg.substr(0, arg.length()-1);

    dirTree1->printTree(arg);


    inStream.close();

    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


    return 0;


}



