#ifndef _DS_H
#define _DS_H

#include "./AVLtree.h"

namespace PM{

typedef int PlayerId;

class PlayersManager{
    class GenKey{
        friend class PlayersManager;
        protected:
        int id;
        public:
        explicit GenKey(const int& id): id(id){}
        virtual bool operator<(const GenKey& other){
            return this->id < other.id;
        }
        virtual bool operator==(const GenKey& other){
            return (this->id == other.id);
        }
    };

    class PlayerKey : public GenKey{
        int level;
        public:
        PlayerKey(const int& id, const int& level): GenKey(id),level(level){}
        bool operator<(const PlayerKey& other){ 
            if(this->level == other.level){
                return this->id > other.id;
            }
            else{
            return (this->level < other.level);
            }
        }

        bool operator==(const PlayerKey& other){
            return (this->level == other.level && this->id == other.id);
        }
    };

    class GroupKey : public GenKey{
        public:
        GroupKey(const int& group_id): GenKey(group_id){}
    };
    
    class PlayerData{
        friend class PlayersManager;
        //AVL::AVLTree<PlayerKey,PlayerData>* owner_group_tree;
        PM::PlayersManager::GroupData* owner_group_data; //maybe remove PM::PlayersManager::
        PlayerId id;
        int level;
        public:
            PlayerData(PM::PlayersManager::GroupData* owner, const PlayerId& id =-1, int level=-1): 
                owner_group_data(owner), id(id), level(level){};
    };

    class GroupData{
        friend class PlayersManager;
        friend class AVLTree;
        AVL::AVLTree<PlayerKey,PlayerData> group_players;
        PlayerId best_in_group;
        GroupKey group_id;
        public:
            GroupData(GroupKey id): group_players(), best_in_group(-1), group_id(id){}; //make sure we really send id
            bool isOk(){
                return (group_players.size() != 0);
            }
    };
    
    AVL::AVLTree<GroupKey,GroupData> groups;
    AVL::AVLTree<GroupKey,int> best_in_non_empty_groups;
    AVL::AVLTree<PM::PlayerId,PlayerData*> all_players;
    AVL::AVLTree<PlayerKey,PlayerData*> all_players_sorted;
    
    PlayerId best_of_all;   //check when removing or adding a player
    int num_of_nonempty_groups;
    public:
        PlayersManager(): groups(), best_in_non_empty_groups(), best_of_all(-1), num_of_nonempty_groups(0){};   //check if needs anything for manual initiation
        //void Init();     //O(1)
        StatusType AddGroup(int groupId);  //O(logk) k- num of groups
        StatusType AddPlayer(int PlayerID, int GroupID, int Level); //O(logn + logk) n- num of players in group, k- num of groups
        StatusType RemovePlayer(int PlayerID);  //O(logn) n- num of players TOTAL   -should we keep another tree of players only?
        StatusType ReplaceGroup(int GroupID, int ReplacementID);   //O(n_group + n_replacement + logk) k-number of groups 
        StatusType IncreaseLevel(int PlayerID, int LevelIncrease);  //O(logn) n- TOTAL number of players
        StatusType GetHighestLevel(int GroupID, int *PlayerID);  //O(logk) k- num of groups. if GroupId<0 : O(1)
        StatusType GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers); //O(n_groupId +logk) k-num of groups
        StatusType GetGroupsHighestLevel(int numOfGroups, int **Players);   //O(numOfGroups +logk) k-num of groups

        class Failure : public std::exception{};
        class Skip : public std::exception{};
    };






}
#endif



