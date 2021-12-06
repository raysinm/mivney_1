#ifndef _AVLTREE_H
#define _AVLTREE_H


#include <memory>
#include <iostream>
#include <cassert>
#include <math.h>
#include "./course_files/library1.h"

namespace AVL{
    
    template <class KeyElem, class Data>
    class AVLTree{
        private:

            class TNode{

                friend class AVLTree;
                KeyElem key;
                Data data;
                int BF; //balance factor
                int height; //?

                TNode* father;
                TNode* left_son; //smaller
                TNode* right_son; //bigger

                public:
                    TNode(const KeyElem& key, const Data& data):
                        key(key), data(data),
                        father(nullptr), left_son(nullptr), right_son(nullptr),
                        height(0), BF(0){};

                    bool operator<(const TNode& other_node){  
                        if (other_node == nullptr || *this == other_node) return false;
                        return (this->key < other_node.key);
                    };

                    bool operator==(const TNode& other_node){
                        return this->key == other_node.key;
                    }
                    
                    bool isRightSon(){
                        if(!this->father) return false;
                        return(this->father->key < this->key);
                    }
                    bool isLeftSon(){
                        if(!this->father) return false;
                        return(this->key < this->father->key);
                    }
                    bool rightSonExists(){
                        return this->right_son;
                    }
                    bool leftSonExists(){
                        return this->left_son;
                    }
            };
            
            TNode* root;
            int size;

            void AVLDestroy_rec(TNode*);
            void AVLBalance(TNode*);
            TNode* AVLFind_rec(TNode* current_node, const KeyElem& key_to_find, TNode** father_to_ret);
            void AVLRemove_rec(TNode* node, const KeyElem& key);
            void AVLNodeRefreshHeight(TNode* node);
            void AVLNodeRefreshBF(TNode* node);
            TNode* findReplacingNode(TNode* node);
            void AVLPrintInOrder_rec(TNode* node);
            void InOrderOutputTNodes_rec(TNode* node, TNode** arr, int& arr_index, const int arr_size);
            void MergeArray(AVLTree<KeyElem,Data>::TNode** arr1, const int arr1_size,
                                 AVLTree<KeyElem,Data>::TNode** arr2, const int arr2_size, AVLTree<KeyElem,Data>::TNode** merged_arr);
            TNode* ArrayToAVLTree(TNode** array, int start, int end, TNode* father);
            void InOrderOutputDatas_rec(TNode* node, Data** arr, int& arr_index, const int arr_size);


        public:

            AVLTree(): root(nullptr), size(0){};
            AVLTree(const AVLTree&);
            ~AVLTree(){
                AVLDestroy_rec(root);
            };
            bool AVLExist(const KeyElem&);
            TNode* AVLFind(const KeyElem& key);
            StatusType AVLInsert(const KeyElem&, const Data&);
            StatusType AVLRemove(const KeyElem&);
            void AVLPrintInOrder();
            //Data& AVLGet(const KeyElem&);
            StatusType AVLMerge(AVLTree<KeyElem,Data>& other_tree);
            Data& AVLGet(KeyElem key);
            int size(){
                return size;
            }
            const KeyElem& AVLMax();
            void InOrderOutputDatas(Data** arr, const int arr_size);
            

            void AVLRotate_LL(TNode*);
            void AVLRotate_RR(TNode*);
            void AVLRotate_LR(TNode*);
            void AVLRotate_RL(TNode*);
            
            /*
            class GenFunctor{
                public:
                GenFunctor();
                ~GenFunctor = virtual;
            }
            class PrintKey : public GenFunctor{
                ElemKey& key;
                public:
                PrintKey();
                PrintKey(const ElemKey& key): key(key){};
                void operator()(ElemKey& key){
                    std::cout << key << ", "
                }
            }
            class InsertTNode : public GenFunctor{
                TNode* node;
                TNode** array;
                int index;
                public:
                InsertTNode();
                InsertTNode(const (const (TNode*)& node, (TNode**)& array, const int& index): node(node), array(array), index(index){};
                void operator()(const (TNode*)& node, (TNode**)& array, const int& index){
                    array[index] = node;
                }
            }
            class InsertData : public GenFunctor{
                TNode* node;
                (Data&)* array;
                int index;
                public:
                InsertData();
                InsertData(const (TNode*)& node, ((Data&)*)& array, const int& index): node()
            } */

    };

    

    //changed to return StatusType to return FAIL and ALLOCATION_ERROR
    template <class KeyElem, class Data>
    StatusType AVLTree<KeyElem,Data>:: AVLInsert(const KeyElem& key, const Data& data){
        
        /* if(key < 0 || this->AVLExist(key)){  //checks if key is already in this tree
            return FAILURE;
        } */
        try{
        TNode* insert_node = new TNode(key, data);
        if(!insert_node) return ALLOCATION_ERROR;

        if(!this->root){
            this->root = insert_node;
            size++;
            return SUCCESS;
        }

        TNode* insert_after_node = nullptr;
        auto not_used_output = AVLFind_rec(root, key, &insert_after_node);
        //assert(insert_after_node != nullptr); //sanity check

        if(key < insert_after_node->key){
            insert_after_node->left_son = insert_node;
        } 
        else{
            insert_after_node->right_son = insert_node;
        }
        
        insert_node->father = insert_after_node;
        AVLBalance(insert_node->father);    //SUPER important
        size++;

        return SUCCESS;
        }catch(std::bad_alloc e&){
            throw;
        }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLBalance(TNode* start){
        
        auto current_node = start;
        while(current_node){
            int prev_height = current_node->height;
            AVLNodeRefreshHeight(current_node);
            AVLNodeRefreshBF(current_node);
            
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
            AVLNodeRefreshHeight(current_node->left_son);
            AVLNodeRefreshHeight(current_node->right_son);
            AVLNodeRefreshHeight(current_node);
            AVLNodeRefreshBF(current_node->left_son);
            AVLNodeRefreshBF(current_node->right_son);
            AVLNodeRefreshBF(current_node);
            current_node = current_node -> father;
        }
    }

    /* void AVLBalance_rec(std::shared_ptr<TNode*<KeyElem, Data> start)
    */
    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLNodeRefreshHeight(TNode* node){
        if(!node) return;

        if(!node->right_son && !node->left_son){
            node->height = 0;
        }
        else if(!node->right_son){
            node->height = node->left_son->height + 1;
        }
        else if(!node->left_son){
            node->height = node->right_son->height +1;
        }
        else{
            node->height = std::max(node->left_son->height, node->right_son->height) + 1;
        }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLNodeRefreshBF(TNode* node){
        if(!node) return;
        if(!node->right_son && !node->left_son){
            node->BF = 0;
        }
        else if(!node->right_son){
            node->BF = node->left_son->height - (-1);
        }
        else if(!node->left_son){
            node->BF = -1 - node->right_son->height;
        }
        else{
            node->BF = node->left_son->height - node->right_son->height;
        }
    }



    //******************_FIND, EXIST_********************//
    template <class KeyElem, class Data>
    bool AVLTree<KeyElem,Data>:: AVLExist(const KeyElem& key_to_find){
        return (AVLFind(key_to_find) != nullptr);
    }

    template <class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::AVLFind(const KeyElem& key_to_find){
        TNode* dummy_ptr;
        return AVLFind_rec(this->root, key_to_find, &dummy_ptr);
    }

    template <class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::AVLFind_rec(AVLTree<KeyElem,Data>::TNode* current_node,
                                     const KeyElem& key_to_find, AVLTree<KeyElem,Data>::TNode** father_to_ret){

        if(!current_node){
            return nullptr;
        }

        if(current_node->key == key_to_find){
            return current_node;
        }

        if(key_to_find < current_node->key){
            if(current_node->left_son  == nullptr){  //Knowledge for AVLInsert
                *father_to_ret = current_node;       
            }
            return (AVLFind_rec(current_node->left_son, key_to_find, father_to_ret));
        }
        else{
            if(current_node->right_son  == nullptr){  //Knowledge for AVLInsert
                *father_to_ret = current_node;       
            }
            return(AVLFind_rec(current_node->right_son, key_to_find, father_to_ret));
        }
    }


    //******************_ROTATIONS_********************//

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRotate_LL(TNode* node_uneven){      //give names to ptrs for clarity

        auto temp_left_right_son = node_uneven->left_son->right_son;
        auto left_son = node_uneven->left_son;
        
        left_son->father = node_uneven->father;
        node_uneven->father = left_son;
        left_son->right_son = node_uneven;
        node_uneven->left_son = temp_left_right_son;

        if(this->root == node_uneven){
            this->root = left_son;
            return;
        }

       if(left_son->key < left_son->father->key){
           left_son->father->left_son = left_son;
       }else{
           left_son->father->right_son = left_son;
       }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRotate_RR(TNode* node_uneven){

        auto temp_right_left_son = node_uneven->right_son->left_son;
        auto right_son = node_uneven->right_son;
        
        right_son->father = node_uneven->father;
        
        right_son->left_son = node_uneven;
        node_uneven->right_son = temp_right_left_son;
        node_uneven->father = right_son;

        if(this->root == node_uneven){
            this->root = right_son;
            return;
        }

        if(right_son->key > right_son->father->key){
            right_son->father->right_son = right_son;
        }
        else{
            right_son->father->left_son = right_son;
        }
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRotate_LR(TNode* node_uneven){
        AVLRotate_RR(node_uneven->left_son);
        AVLRotate_LL(node_uneven);
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRotate_RL(TNode* node_uneven){
        AVLRotate_LL(node_uneven->right_son);
        AVLRotate_RR(node_uneven);
    }

    //******************_AVLRemove_********************//

    
    //Recursive Return

    template <class KeyElem, class Data>
    StatusType AVLTree<KeyElem,Data>:: AVLRemove(const KeyElem& key){
        AVLRemove_rec(this->root, key);
        size--; //garentee removed??
        return SUCCESS;
    }

    template <class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLRemove_rec(TNode* node, const KeyElem& key){
        if(!node){
            return;
        }
                                         //ADD METHODS TO TNODE!!!!!!!!!!
        if(node->key == key){
            auto temp_father = node->father;
            if(!node->leftSonExists() && !node->rightSonExists()){  //This is a leaf
                if(node == this->root){
                    this->root = nullptr;
                }
                else if(node->isLeftSon()){
                    node->father->left_son = nullptr; 
                }
                else if(node->isRightSon()){
                    node->father->right_son = nullptr;
                }  
            }
            else if(!node->leftSonExists() || !node->rightSonExists()){  //Has only a right son or a left son
            
                TNode* node_son = node->leftSonExists() ? node->left_son : node->right_son;
                
                if(node == this->root){
                    this->root = node_son;
                }
                else if(node->isLeftSon()){
                    node->father->left_son = node_son;
                }
                else if(node->isRightSon()){
                    node->father->right_son = node_son;
                }
                node_son->father = node->father;
            }
            else{   //Has TWO sons
                auto replacer = findReplacingNode(node);    //replacer is the biggest node that is smaller than our node
                
                if(node == this->root){
                        this->root = replacer;
                    }
                else if(node->isLeftSon()){
                    node->father->left_son = replacer;
                }
                else if(node->isRightSon()){
                    node->father->right_son = replacer;
                }

                auto temp_node_left_son = node->left_son,
                    temp_node_right_son = node->right_son,
                    temp_replacer_left_son = replacer->left_son;
                
                if(node == replacer->father){   // special case. They are direct relatives, and therfore point to eachother
            
                    node->father = replacer;
                    replacer->left_son = node;
                
                }else{
                    node->father = replacer->father;
                    replacer->left_son = temp_node_left_son;
                }
                replacer->father = temp_father;
                node->left_son = temp_replacer_left_son;
                node->right_son = replacer->right_son;
                replacer->right_son = temp_node_right_son;
                replacer->right_son->father = replacer;
                replacer->left_son->father = replacer;

                if(node->leftSonExists() && node->rightSonExists()){ 
                    AVLRemove_rec(this->root, key);
                }
                else{
                    if(node->father == replacer){
                        node->father->left_son = node->left_son;
                    }else node->father->right_son = node->left_son;   //guranteed- has no right son!
                }
                temp_father = node->father;
            }
            
            delete[] node;
            AVLBalance(temp_father);
            return;

        }else if(node->leftSonExists() && key < node->key){
            AVLRemove_rec(node->left_son, key);
        }else if(node->rightSonExists() && node->key < key){
            AVLRemove_rec(node->right_son, key);
        }
        
    }

    template <class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::findReplacingNode(AVLTree<KeyElem,Data>::TNode* node){
        auto replacer = node->left_son;
        while(replacer->right_son){
            replacer = replacer -> right_son;
        }
        return replacer;
    }

/*  */
    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLPrintInOrder(){
        AVLPrintInOrder_rec(this->root);
    }

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLPrintInOrder_rec(AVLTree<KeyElem,Data>::TNode* node){
        if(!node){
            return;
        }

        AVLPrintInOrder_rec(node->left_son);
        std::cout << node->key << ", " ;
        AVLPrintInOrder_rec(node->right_son);
        
    }
     
    template<class KeyElem, class Data>
    Data& AVLTree<KeyElem,Data>:: AVLGet(KeyElem key){   //return data by reference or as a ptr?
        return AVLFind(key)->data;
    }

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: AVLDestroy_rec(TNode* node){
        if(!node){
            return;
        }
        
        if(node->left_son){
            AVLDestroy_rec(node->left_son);
            node->left_son = nullptr;
        }
        if(node->right_son){
            AVLDestroy_rec(node->right_son);
            node->right_son = nullptr;
        }
        delete[] node;
        return;
    }

    //******************_AVLMerge_********************//

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>::AVLMerge(AVLTree<KeyElem,Data>& other_tree){
        //allocating two arrays for merging
        TNode** tree1_arr;
        TNode** other_tree_arr;
        TNode** merged_arr; 
        try{
        tree1_arr = new TNode*[this->size];
        other_tree_arr = new TNode*[other_tree.size];
        merged_arr = new TNode*[this->size + other_tree.size];

        int i1 = 0, i2 = 0;
        InOrderOutputTNodes_rec(this->root, tree1_arr, i1, this->size);
        InOrderOutputTNodes_rec(other_tree.root, other_tree_arr, i2, other_tree.size);
        MergeArray(tree1_arr, this->size, other_tree_arr, other_tree.size, merged_arr);
        //need to send merged_arr like this? (want it to be updated)- yes
        /* for(int i=0;i<(this->size + other_tree.size);i++){
            std::cout<<merged_arr[i]->key << std::endl;
        } */
        this->root = this->ArrayToAVLTree(merged_arr, 0, (this->size + other_tree.size - 1), nullptr);
        this->size += other_tree.size;
        other_tree.root = nullptr;
        delete[] tree1_arr, other_tree_arr, merged_arr;
        tree1_arr = nullptr;
        other_tree_arr = nullptr;
        merged_arr = nullptr;
        }
        catch(std::bad_alloc e&){
            delete[] tree1_arr, other_tree_arr, merged_arr;
            throw;
        }
    }

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: InOrderOutputDatas(Data** arr, const int arr_size){
        int& index = 0;
        InOrderOutputDatas_rec(this->root, arr, index, arr_size);
    }

    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: InOrderOutputDatas_rec(TNode* node, Data** arr, int& arr_index, const int arr_size){
        if(!node || arr_index == arr_size){    //if group is empty
            return;
        }
        InOrderOutputDatas_rec(node->left_son, arr, arr_index, arr_size);
        if(node->data.isOk()){
            arr[arr_index] = &node->data;
            arr_index++;
        }
        InOrderOutputDatas_rec(node->right_son, arr, arr_index, arr_size);
        
        
    }



    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>:: InOrderOutputTNodes_rec(TNode* node, TNode** arr, int& arr_index, const int arr_size){
        if(!node || arr_index == arr_size){
            return;
        }
        InOrderOutputTNodes_rec(node->left_son, arr, arr_index, arr_size);
        arr[arr_index] = node;
        arr_index++;
        InOrderOutputTNodes_rec(node->right_son, arr, arr_index, arr_size);
    }

    template<class KeyElem, class Data>
    typename AVLTree<KeyElem,Data>::TNode* AVLTree<KeyElem,Data>::ArrayToAVLTree(AVLTree<KeyElem,Data>::TNode** array,
                                                        int start, int end, AVLTree<KeyElem,Data>::TNode* father){
        if(end < start || start > end){
            return nullptr;
        }
        int middle = (start + end)/2;
        TNode* current_root = array[middle];
        std::cout << current_root->key << std::endl;
        current_root->father = father;
        current_root->left_son = ArrayToAVLTree(array, start, middle - 1, current_root);
        current_root->right_son = ArrayToAVLTree(array, middle + 1, end, current_root);
        
        return current_root;
    }



    //recieves pointer to merged_arr so will update without having to return?  -should do just that
    template<class KeyElem, class Data>
    void AVLTree<KeyElem,Data>::MergeArray(typename AVLTree<KeyElem,Data>::TNode** arr1, const int arr1_size, 
                    typename AVLTree<KeyElem,Data>::TNode** arr2, const int arr2_size, typename AVLTree<KeyElem,Data>::TNode** merged_arr){
        int index = 0;
        int index1 = 0;
        int index2 = 0;
        while((index1 < arr1_size) && (index2 < arr2_size)){
            if(arr1[index1]->key < arr2[index2]->key){
                merged_arr[index] = arr1[index1];
                index1++;
            }
            else{
                merged_arr[index] = arr2[index2];
                index2++;
            }
            //std::cout << merged_arr[index]->key << std::endl;
            index++;
            
            //did not include arr1[index1] == arr2[index2] because not possible
        }
        if(index1 < arr1_size){
            while(index1 < arr1_size){
                merged_arr[index] = arr1[index1];
                index1++;
                index++;
            }
        }
        if(index2 < arr2_size){
            while(index2 < arr2_size){
                merged_arr[index] = arr2[index2];
                index2++;
                index++;
            }
        }
    }

    template<class KeyElem, class Data>
    const KeyElem& AVLTree<KeyElem,Data>::AVLMax(){
        if(!root){
            return NULL;
        }
        auto current = this->root;
        while(current->right_son){
            current = current->right_son;
        }
        return current->key;
    }
}

#endif