
#include "./course_files/library1.h"


template <class Data, class KeyElem>
class AVLTree{
    
    std::shared_ptr<TNode> root; //Not sure about syntax, but every node has to be smart ptr(for easy destroying)

    TNode* AVLSearchFather(TNode* node, bool* exists);  //Searches for the node given. if it exists- returns ptr to the node and true in "exists". 
                                                    //if it doesnt exist- returns it's father and false in "exists". 
    
    enum{
        AVL_SUCCESS,
        AVL_SUCCESS_W_CHANGE,
        AVL_EMPTY,
        AVL_OUT_OF_MEMORY,
        AVL_OTHER_ERROR
    }AVL_RESULT;

    friend TNode;

    public:
        AVLTree() = default;
        AVLTree(const &AVLTree); //Should we use user's copy functions?
        ~AVLTree(); //Should we use user's free functions?

        AVL_RESULT& AVLFind(key)
        AVL_RESULT& AVLInsert(const &KeyElem, const &Data);
        AVL_RESULT& AVLRemove(const &KeyElem);
        Data* AVLGet(const &KeyElem);

        void AVLRotate_LL();
        void AVLRotate_RR();
        void AVLRotate_LR();
        void AVLRotate_RL();
        

}

