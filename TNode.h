/* #ifndef _TNODE_H
#define _TNODE_H

#include "./course_files/library1.h"
#include "AVLtree.h"


namespace AVL{

    template <class KeyElem, class Data>
    class TNode{

        KeyElem key;
        Data data;
        int BF; //balance factor
        int height; //?

        TNode<KeyElem,Data>* father, left_son, right_son;

        public:
            TNode(const KeyElem& key, const Data& data):
                key(key), data(data),
                father(nullptr), left_son(nullptr), right_son(nullptr){};

            bool operator<(const TNode& other_node){  
                if (other_node == nullptr || *this == other_node){
                    return false;
                }
                return (this->key < other_node.key);
            };

            bool operator==(const TNode& other_node){
                return this->key == other_node.key;
            }

            void brotherExists(){
                auto father_node = this->father;
                if(this->key < father_node.key){
                    return (father_node.right_son != nullptr);
                }
                else{           //we have to make sure its not equal
                    return (father_node.left_son != nullptr);
                }
            }
    };

    template <class Data, class KeyElem>
    bool TNode<KeyElem,Data>::brotherExists(){    //add ptr for error messages

        auto father_node = this->father;
        if(this->key < father_node.key){
            return (father_node.right_son != nullptr);
        }
        if(this->key >= father_node.key){           //we have to make sure its not equal
            return (father_node.left_son != nullptr);
        }
        
    }
}

#endif */