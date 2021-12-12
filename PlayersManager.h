#ifndef _PM_H
#define _PM_H

#include "./AVLtree.h"

using namespace AVL;
namespace PM{

class PlayersManager{

    class GenKey;
    class GroupKey;
    class playerKey;
    class GroupData;
    class PlayerData;
    
    class GenKey{
        friend class PlayersManager;
        protected:
        int id;
        public:
        explicit GenKey(const int& id): id(id){}
        virtual bool operator<(const GenKey& other) const{
            return this->id < other.id;
        }
        virtual bool operator==(const GenKey& other) const{
            return (this->id == other.id);
        }
        friend std::ostream& operator<<(std::ostream& os, const GenKey& key){
            os << key.id;
            return os;
        }
    };

    class PlayerKey : public GenKey{
        int level;
        public:
        PlayerKey(const int& id=-1, const int& level=-1): GenKey(id),level(level){}
        bool operator<(const PlayerKey& other) const{ 
            if(this->level == other.level){
                return this->id < other.id;
            }
            else{
            return (this->level > other.level);
            }
        }

        bool operator==(const PlayerKey& other) const{
            return (this->level == other.level && this->id == other.id);
        }
        friend std::ostream& operator<<(std::ostream& os, const PlayerKey& key){
            os << key.id;
            return os;
        }
    };

    class GroupKey : public GenKey{
        public:
        explicit GroupKey(const int& group_id): GenKey(group_id){}
    };

    class GroupData{
        friend class PlayersManager;
        AVL::AVLTree<PlayerKey,PlayerData> group_players;
        PlayerKey best_in_group;
        int group_id;
        public:
        explicit GroupData(const int& id): group_players(), best_in_group(-1,-1), group_id(id){}; //make sure we really send id
       // GroupData(const GroupData&) = delete;
    };
    
    class PlayerData{
        friend class PlayersManager;
        //AVL::AVLTree<PlayerKey,PlayerData>* owner_group_tree;
        GroupData* owner_group_data; //maybe remove PM::PlayersManager::
        int id;
        int level;
        public:
            PlayerData(PM::PlayersManager::GroupData* owner, const int& id =-1, int level=-1): 
                owner_group_data(owner), id(id), level(level){};
            friend std::ostream& operator<<(std::ostream& os, const PlayerData& data){
                os << data.id;
            return os;
            
        }
    };

    
    
    AVLTree<GroupKey,GroupData> groups;
    AVLTree<GroupKey,int> best_in_non_empty_groups;
    AVLTree<int,PlayerData*> all_players;
    AVLTree<PlayerKey,PlayerData*> all_players_sorted;
    
    int best_of_all;   //check when removing or adding a player
    //int num_of_nonempty_groups;
    public:
        PlayersManager(): groups(), best_in_non_empty_groups(), all_players(), all_players_sorted(),
                                     best_of_all(-1){};   //check if needs anything for manual initiation
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



