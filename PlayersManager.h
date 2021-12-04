#ifndef _DS_H
#define _DS_H

#include "./AVLtree.h"

namespace DS{

class PlayersManager{
    class GenKey{
        protected:
        int id;
        public:
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
        bool operator<(const PlayerKey& other){ 
            if(this->level == other.level){
                return this->id > other.id;
            }
            else{
            return (this->level < other.level);
            }
        }
        bool operator==(const PlayerKey& key){
            return (this->level == other.level && this->id == other.id);
        }
    };

    class GroupKey : public GenKey{};
    
    class PlayerData{
        AVL::AVLTree<PlayerKey,PlayerData>* owner_tree;

        public:
            PlayerData(AVL::AVLTree<PlayerKey,PlayerData>* owner): owner_tree(owner){};
    };

    class GroupData{
        AVL::AVLTree<PlayerKey,PlayerData> group_players;
        PlayerKey best;
    };
    
    AVL::AVLTree<GroupKey,GroupData> groups;
    AVL::AVLTree<PlayerKey,PlayerData*> all_players;
    PlayerKey best_of_all;
    
    public:
        PlayersManager();
        //void Init();     //O(1)
        StatusType AddGroup(int groupId);  //O(logk) k- num of groups
        StatusType AddPlayer(int PlayerID, int GroupID, int Level); //O(logn + logk) n- num of players in group, k- num of groups
        StatusType RemovePlayer(int PlayerID);  //O(logn) n- num of players TOTAL   -should we keep another tree of players only?
        StatusType ReplaceGroup(int GroupID, int ReplacementID);   //O(n_group + n_replacement + logk) k-number of groups 
        StatusType IncreaseLevel(int PlayerID, int LevelIncrease);  //O(logn) n- TOTAL number of players
        StatusType GetHighestLevel(int GroupID, int *PlayerID);  //O(logk) k- num of groups. if GroupId<0 : O(1)
        StatusType GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers); //O(n_groupId +logk) k-num of groups
        StatusType GetGroupsHighestLevel(int numOfGroups, int **Players);   //O(numOfGroups +logk) k-num of groups
        void Quit();    //O(n+k) n- num of players, k- num of groups
};

PlayersManager::PlayersManager() : groups(), best_of_all(NULL){}

//take care of return vals in add&remove
template <class GroupKey, class GroupData> //do we need?
StatusType AddPlayer(int PlayerID, int GroupID, int Level){
    auto player_group = groups.AVLFind(GroupID);

    GroupKey new_group_key = new GroupKey(GroupId);
    PlayerKey new_player_key = new PlayerKey(PlayerId, Level);
    
    groups.AVLFind(new_group_key);
    return player_group.players.AVLInsert(PlayerID, Level);
}

template <class GroupKey, class GroupData>
StatusType RemovePlayer(int PlayerID){
    //need to figure how to find player
}

template <class GroupKey, class GroupData>
StatusType ReplaceGroup(int GroupID, int ReplacementID){
    auto group_to_remove = groups.AVLFind(GroupID);
    auto group_to_merge = groups.AVLFind(ReplacementID);
    if(group_to_remove == nullptr || group_to_merge == nullptr) return FAILURE;
    //merge - dont have func yet: merge(group_to_remove.players, group_to_merge.players) and put into group_to_merge
    return groups.AVLRemove(GroupID);
}

template <class GroupKey, class GroupData>
StatusType IncreaseLevel(int PlayerID, int LevelIncrease){
    //need to figure how to find player
}


}
#endif












