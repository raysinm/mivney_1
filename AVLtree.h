
#ifndef _AVLTREE_H
#define _AVLTREE_H

#include <memory>
#include "./course_files/library1.h"
#include "TNode.h"

namespace AVL{

    template <class KeyElem, class Data>
    class AVLTree{
        
        private:
            friend TNode;
            
            std::shared_ptr<TNode<KeyElem, Data> root; //Not sure about syntax, but every node has to be smart ptr(for easy destroying)

        /*   std::shared_ptr<TNode<KeyElem, Data> AVLSearchFather(std::shared_ptr<TNode<KeyElem, Data> node, bool* exists);  //Searches for the node given. if it exists- returns ptr to the node and true in "exists". 
                                                            //if it doesnt exist- returns it's father and false in "exists". 
            */
            enum{
                AVL_SUCCESS,
                AVL_SUCCESS_W_CHANGE,
                AVL_EMPTY,
                AVL_OUT_OF_MEMORY,
                AVL_OTHER_ERROR
            }AVL_RESULT;

            void AVLBalance(std::shared_ptr<TNode<KeyElem,Data>>&);
            void AVLExist(const KeyElem&);
            std::shared_ptr<TNode<KeyElem,Data>> AVLFind_rec(const std::shared_ptr<TNode<KeyElem,Data>>& current_node,
                                                    const KeyElem& key_to_find, std::shared_ptr<TNode<KeyElem,Data>>& father_to_ret);


        public:
            AVLTree(): root(nullptr);
            AVLTree(const AVLTree&); //Should we use user's copy functions?
            ~AVLTree(); //Should we use user's free functions?

            AVL_RESULT& AVLFind(const KeyElem& key);
            AVL_RESULT& AVLInsert(const KeyElem&, const Data&);
            AVL_RESULT& AVLRemove(const KeyElem&);
            //Data& AVLGet(const KeyElem&);

            void AVLRotate_LL();
            void AVLRotate_RR();
            void AVLRotate_LR();
            void AVLRotate_RL();
            
    };


    /* template<class KeyElem, class Data>
    AVLTree(): root(nullptr){}
    */

    template<class KeyElem, class Data>
    StatusType& AVLInsert(const KeyElem& key, const Data& data){

        std::shared_ptr<TNode> new_node = new(TNode(key, data));
        bool node_exists = false;
        
        if(this->root == nullptr){
            root = new_node;
            return AVL_SUCCESS;    
        }
        else{
            auto father = AVLSearchFather(new_node, &node_exists);
            if (node_exists == true){
                father.data = new_node.data;
                return AVL_SUCCESS_W_CHANGE;
            }
            else{
                if(father.right > new_node){
                    //rotate LL or RL ?????????
                }
                else{
                    //rotate RR or LR ?????????????????????
                }
            }
        }

    }

    template <class KeyElem, class Data>
    StatusType AVLInsert(const KeyElem& key, const Data& data){
        
        if(!key || this->AVLExist(key)){  //checks if key is already in this tree
            return INVALID_INPUT;
        }

        std::shared_ptr<TNode<KeyElem,Data>> insert_after_node;
        auto not_used_node = AVLFind_rec(this->root, key, insert_after_node);
        assert(insert_after_node != nullptr); //sanity check

        insert_node = std::make_shared<TNode<KeyElem,Data>>(key, data);
        if(key < insert_after_node->key){
            insert_after_node->left_son = insert_node;
        } 
        else{
            insert_after_node->right_son = insert_node;
        }
        
        insert_node->father = insert_after_node;
    /*     insert_node->father->height = 1;    //will update even if it is already 1 (ok)
        
        this->AVLRefreshHeight(insert_node); */

        this->AVLBalance(insert_node->father);    //SUPER important

        return SUCCESS;
    }

    template <class KeyElem, class Data>
    void AVLBalance(std::shared_ptr<TNode<KeyElem,Data>> start){
        
        auto current_node = start
        while(current_node){
            int prev_height = current_node->height;
            this->AVLNodeRefreshHeight(current_node);
            this->AVLNodeRefreshBF(current_node);
            
            if(current_node->height == prev_height){
                break;
            }
            if(current_node->BF == 2 && current_node->left_son->BF >= 0){
                this->AVLRotate_LL(current_node);
            }
            else if(current_node->BF == 2 && current_node->left_son->BF == -1){
                this->AVLRotate_LR(current_node);
            }
            else if(current_node->BF == -2 && current_node->right_son->BF == -1){
                this->AVLRotate_RR(current_node);
            }
            else if(current_node->BF == -2 && current_node->right_son->BF >=0){
                this->AVLRotate_RL(current_node);
            }

            current_node = current_node -> father;
        }

    }

    /* void AVLBalance_rec(std::shared_ptr<TNode<KeyElem, Data> start)
    */
   template <class KeyElem, class Data>
    void AVLNodeRefreshHeight(std::shared_ptr<TNode<KeyElem,Data>>& node){
        
        if(!node->right_son && !node->left_son){
            node->height = 0;
        }
        else if(!node->right_son){
            node->height = node->left_son->height + 1;
        }
        else if(!node->left_son{
            node->height = node->right_son->height +1;
        }
        else{
            node->height = max(node->left_son->height, node->right_son->height) + 1;
        }
    }

    template <class KeyElem, class Data>
    void AVLNodeRefreshBF(std::shared_ptr<TNode<KeyElem,Data>>& node){
        if(!node->right_son && !node->left_son){
            node->BF = 0;
        }
        else if(!node->right_son){
            node->BF = node->left_son->height;
        }
        else if(!node->left_son{
            node->height = 0 - node->right_son->height;
        }
        else{
            node->BF = node->left_son->height - node->right_son->height;
        }
    }


/*     template<class KeyElem, class Data>
    void AVLScanInOrder(AVLTree& tree){

        AVLScanInOrder_rec(tree.root);
    }
 */


    //******************_FIND, EXIST_********************//
    template <class KeyElem, class Data>
    bool AVLExist(const KeyElem& key_to_find){
        return (AVLFind(key_to_find != nullptr));
    }

    template <class KeyElem, class Data>
    std::shared_ptr<TNode<KeyElem,Data>> AVLFind(const KeyElem& key_to_find){
        return AVLFind_rec(this->root, key_to_find);
    }

    template <class KeyElem, class Data>
    std::shared_ptr<TNode<KeyElem,Data>> AVLFind_rec(const std::shared_ptr<TNode<KeyElem,Data>>& current_node,
                                                    const KeyElem& key_to_find, std::shared_ptr<TNode<KeyElem,Data>>& father_to_ret){

            if(!current_node){
                return nullptr;
            }

            if(current_node->key == key_to_find){
                return &current_node;
            }

            if(key_to_find < current_node->key){
                if(current_node.left_son  == nullptr){  //Knowledge for AVLInsert
                    father_to_ret = current_node;       
                }
                return (AVLFind_rec(current_node.left_son, key_to_find));
            }
            else{
                if(current_node.right_son  == nullptr){  //Knowledge for AVLInsert
                    father_to_ret = current_node;       
                }
                return(AVLFind_rec(current_node.right_son, key_to_find));
            }
    }


    //******************_ROTATIONS_********************//

    template <class KeyElem, class Data>
    void AVLRotate_LL(std::shared_ptr<TNode<KeyElem,Data>>& node_uneven){      //give names to ptrs for clarity

        auto temp_left_right_son = node_uneven->left_son->right_son;

        node_eneven.left_son->father = node_eneven.father;
        if(node_eneven.left_son->key > node_uneven.father->key){
            node_eneven.left_son->father->right_son = node_eneven.left_son;
        }
        else{
            node_eneven.left_son->father->left_son = node_eneven.left_son;
        }
        
        node_eneven.father = node_eneven.left_son;
        
        node_uneven.left_son->right_son = node_eneven;
        node_eneven.left_son = temp_left_right_son;

    }

    template <class KeyElem, class Data>
    void AVLRotate_RR(std::shared_ptr<TNode<KeyElem,Data>>& node_uneven){

        auto temp_right_left_son = node_eneven.right_son->left_son;
        auto right_son = node_eneven.right_son;
        auto right_left_son = node_uneven.right_son->left_son;
        
        right_son->father = node_eneven->father;
        
        right_son->left_son = node_eneven;
        node_eneven->right_son = temp_right_left_son;

        if(right_son->key > right_son->father->key){
            right_son->father->right_son = right_son;
        }
        else{
            right_son->father->left_son = right_son;
        }

        node_eneven.father = right_son;

    }
    template <class KeyElem, class Data>
    void AVLRotate_LR(std::shared_ptr<TNode<KeyElem,Data>>& node_uneven){
        AVLRotate_RR(node_eneven.left_son);
        AVLRotate_LL(node_uneven);
    }

    template <class KeyElem, class Data>
    void AVLRotate_RL(std::shared_ptr<TNode<KeyElem,Data>>& node_uneven){
        AVLRotate_LL(node_uneven.right_son);
        AVLRotate_RR(node_uneven);
    }

    //******************_AVLRemove_********************//

    //did not delete node, smart ptr delete itself
    //func returnes
    //shared ptr recieved in funcs
    template <class KeyElem, class Data>
    StatusType& AVLRemove(const KeyElem&){
        auto nodeToRemove = AVLFind(KeyElem);
        AVLRemoveRec(nodeToRemove);
    }

    void AVLRemoveRec(std::shared_ptr<TNode<KeyElem,Data>>& nodeToRemove){
        if((nodeToRemove->right_son == nullptr) && (nodeToRemove->left_son == nullptr))
        {
            changeInFather(KeyElem,nodeToRemove->father,nullptr);
        }
        else if((nodeToRemove->right_son == nullptr) || (nodeToRemove->left_son == nullptr)){
            if(nodeToRemove->right_son == nullptr){
                changeInFather(KeyElem,nodeToRemove->father,nodeToRemove->left_son);
            }
            else{
                changeInFather(KeyElem,nodeToRemove->father,nodeToRemove->right_son);
            }
        }
        else{
            auto nodeToReplace = nodeToRemove->left_son;
            auto nodeToRemoveFather = nodeToRemove->father;
            auto nodeToRemoveRight = nodeToRemove->right_son;
            auto nodeToRemoveLeft = nodeToRemove->left_son;
            while (nodeToReplace->right_son != nullptr)
            {
                nodeToReplace = nodeToReplace->right_son;
            }
            changeInFather(KeyElem,nodeToReplace->father,nodeToRemove);
            nodeToRemove->left_son = nodeToReplace->Left_son;
            nodeToRemove->right_son = nullptr;
            changeInFather(KeyElem,nodeToRemoveFather,nodeToReplace);
            nodeToReplace->right_son = nodeToRemoveRight;
            nodeToReplace->left_son = nodeToRemoveLeft;
            AVLRemoveRec(nodeToRemove);
        }
    }

    void changeInFather(const KeyElem&, std::shared_ptr<TNode<KeyElem,Data>>& father, std::shared_ptr<TNode<KeyElem,Data>>& newSon){
        if(father == nullptr)
        {
            root = newSon; //this.root?
        }
        else if(father->right_son->key == KeyElem){
            father->right_son = newSon;
        }                  
        else{
            father->left_son = newSon;
        }
    }

}

#endif
