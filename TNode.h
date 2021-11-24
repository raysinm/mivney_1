#include "./course_files/library1.h"
#include "AVLtree.h"

template <class Data, class KeyElem>
class TNode{
    AVLTree* owner_tree; //not needed?

    KeyElem key;
    Data data;
    int BF; //balance factor
    int height; //?

    TNode* father;
    TNode* left_son, right_son;

    
    friend AVLTree;
   /*  enum {
         FIRST_BIGGER = -1,
         EQUAL = 0,
         SECOND_BIGGER = 1   
        } CompareNodeData; */

    public:
        TNode(const &KeyElem, const &Data);
        TNode(const &TNode);
        ~TNode();


        bool operator<(const &TNode other_node){
            
            if (other_node == nullptr || *this == other_node){
                return false;
            }
            
            return (this->key > other_node.key);
        };

        //dataCompare()

        void refreshBF();
        

}