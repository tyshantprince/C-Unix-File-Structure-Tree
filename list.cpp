#include <iostream>
#include <fstream>
#include <string>


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


    std::ifstream inStream("ITUnix");
    std::string aline;

    if(inStream.is_open()){
        for(int i = 0; i < 20; i++)
        {
            getline(inStream, aline);
            if(aline.back() == ':'){
                std::cout << "This is a directory" << std::endl;
                std::cout << aline << std::endl ;

                // create tokenizer of path name
                dirTokenizer *dirTokenizer1 = new dirTokenizer(aline);
                int x = 0;

                do{
                    std::cout << dirTokenizer1->next() << std::endl;

                }
                while(!dirTokenizer1->isEmpty());
                // check if first level of path is a node already,
                // if so, continue down to the next level of file path
                // if not, add this node to the binary tree.

                // if(dirTree=>findNode(dirTokenizer->next()))



            }
            else if(aline == ""){
//                std::cout << "start of a new directory" << std::endl;
            }
            else{
//                std::cout << "this is a file in a directory" << std::endl;
            }
//            std::cout << aline << std::endl ;
        }
    }


    inStream.close();

    return 0;


}



