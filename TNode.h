#ifndef _TNODE_H
#define _TNODE_H

#include "./course_files/library1.h"
#include "AVLtree.h"


namespace AVL{

    template <class KeyElem, class Data>
    class TNode{
        //AVLTree* owner_tree; //not needed?

        KeyElem key;
        Data data;
        int BF; //balance factor
        int height; //?

        std::shared_ptr<TNode<KeyElem,Data>> father, left_son, right_son;

        
        friend AVLTree;
    /*  enum {
            FIRST_BIGGER = -1,
            EQUAL = 0,
            SECOND_BIGGER = 1   
            } CompareNodeData; */

        public:
            TNode(const &KeyElem, const &Data):
                key(key), data(data),
                father(nullptr), left_son(nullptr), right_son(nullptr){};
            TNode(const &TNode);
            ~TNode();


            bool operator<(const &TNode other_node){
                
                if (other_node == nullptr || *this == other_node){
                    return false;
                }
                
                return (this->key < other_node.key);
            };

            //dataCompare()

            void refreshBF();
            void brotherExists();

    };

    template <class Data, class KeyElem>
    bool brotherExists(){    //add ptr for error messages

        auto father_node = this->father;
        if(this->key < father_node.key){
            return (father_node.right_son != nullptr);
        }
        if(this->key >= father_node.key){           //we have to make sure its not equal
            return (father_node.left_son != nullptr);
        }
        
    }
}

#endif