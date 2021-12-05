

#include "./PlayersManager.h"

namespace PM{

    StatusType PlayersManager::AddGroup(int groupId){   //O(logk) k- num of groups
        /*
        1) AVLInsert into groups tree 
        */
        GroupKey new_group_key(groupId);
        GroupData new_group_data;
        return groups.AVLInsert(new_group_key, new_group_data);
    }

    //take care of return vals in add&remove'
    StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level){ //O(logn + logk) n- num of players in group, k- num of groups
        
        PlayerKey new_player_key(PlayerID, Level);
        if(!groups.AVLExist(GroupID)){
            return FAILURE;
        }
        auto group_data = groups.AVLGet(GroupID);
        auto& players = group_data.group_players;

        PlayerData new_player_data(&group_data.group_players, Level);
        int prev_group_size = players.size();
        auto result = players.AVLInsert(new_player_key, new_player_data);
        if(result != SUCCESS) return result;
        group_data.best_in_group = players.AVLMax().id;

        if(prev_group_size == 0){
            ++this->num_of_nonempty_groups;
        }
        all_players.AVLInsert(PlayerID, &new_player_data);
        
        
        /*
        1) create PlayerKey with PlayerId and level
        2) AVLGet to the group (find and return to us the data) + returning error if not found
        3) AVLInsert into the group
        3.5) update best_player
        3.75) if group size was 0 and now >0 , increase number of non-empty groups in PlayersManager
        4) AVLInsert into the all_players + all_players_sorted
        5) update best_of_all with help of AVLFindBest() (to be written)
        */
        
        /* auto player_group = groups.AVLFind(GroupID);

        GroupKey new_group_key = new GroupKey(GroupId);
        PlayerKey new_player_key = new PlayerKey(PlayerId, Level);
        
        groups.AVLFind(new_group_key);
        return player_group.players.AVLInsert(PlayerID, Level); */
    }

    StatusType PlayersManager::RemovePlayer(int PlayerID){  //O(logn) n- num of players TOTAL   -should we keep another tree of players only?
        
        /*
        1) find player in all_players
        2) create playerKey with level inside player's data
        3) AVLRemove in all_players_sorted
        4) AVLRemove in owner_group_tree
        4.5) update best_player
        5)if group.size == 0 , decrease the number of non-empty groups in PlayersManager
        5) AVLRemove in all_players
        6) update best_of_all
        7) in debugging- make sure that all data is actually deleted, not just pointers
        */
    }

    StatusType PlayersManager::ReplaceGroup(int GroupID, int ReplacementID){    //O(n_group + n_replacement + logk) k-number of groups
        /*
        AVLGet on each of the groups (get their data)
        if both of the groups are *non_empty* (size > 0), decrease number of non-empty groups in PlayersManager
        merge the player trees in both groups
        AVLRemove on old, merged group
        change the old players data to point to new group tree
        */
       /*  auto group_to_remove = groups.AVLFind(GroupID);
        auto group_to_merge = groups.AVLFind(ReplacementID);
        if(group_to_remove == nullptr || group_to_merge == nullptr) return FAILURE;
        //merge - dont have func yet: merge(group_to_remove.players, group_to_merge.players) and put into group_to_merge
        return groups.AVLRemove(GroupID); */
    }

    StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease){  //O(logn) n- TOTAL number of players
        if(!all_players.AVLExist(PlayerID)) return FAILURE;
        auto player_data = all_players.AVLGet(PlayerID);
        int old_level = player_data->level;
        PlayerKey player_key(PlayerID, old_level);
        /*
        1) AVLFind in all_players
        2) use the ptr to the group in player's data to go to the group
        3) AVLFind in group tree
        4) new_level = data->level + LevelIncrease
        5) AVLRemove player (using old level)
        6) create to new playerData and new PLayerKey with ptr to the group (can be in temp) and new_level
        7)AVLInsert this new player in the group tree
        7.5) update best_player
        8)AVLRemove in all_players_sorted
        9)AVLInsert new player in all_players_sorted
        10)update best_of_all
        */
    }

    StatusType PlayersManager::GetHighestLevel(int GroupID, int *PlayerID){   //O(logk) k- num of groups. if GroupId<0 : O(1)
        if(GroupID < 0)
        {
            *PlayerID = best_of_all;
            return SUCCESS;
        }
        else{
            if(!groups.AVLExist(GroupID)){
                return FAILURE;
            }
            GroupData groupData = groups.AVLGet(GroupID);
            *PlayerID = groupData.best_in_group;
            return SUCCESS;
        }
        /*
        returns best_player in group (find the group)
        or best_of_all if GroupId<0
        */
    }
 
        
    StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers){ //O(n_groupId +logk) k-num of groups
        /*
        if GroupId<0:
            InOrder scan (with node limit) and input in players array in *all_players_sorted*
        else
            InOrder scan (with node limit) and input in players array in *group_tree* (find the group first)
        */
    } 
    

    //need AVL func that returns Data of Root
    StatusType PlayersManager::GetGroupsHighestLevel(int numOfGroups, int **Players){   //O(numOfGroups +logk) k-num of groups
        if(numOfGroups < num_of_nonempty_groups) return FAILURE;
        Players = (int**)malloc(numOfGroups*sizeof(PlayerId)); //not sure if its good
        if(!Players) return ALLOCATION_ERROR;
        int count = 0;
        //InorderRec((root group), Players, numOfGroups, count);
        
        /*
        use numOfGroups as counter to inorder scan function.
        EMPTY group- doesnt count!
        1) check if numOfgroups smaller than number of non-empty groups (should be in PlayersManager), return error
        2) Inorder function with node limit- access each group's best_player (if not empty)
        */
    }  

   
   //for this func I need AVL funcs that return to me data of left & right sons
   //data might not be good enought to know where am I in tree
    void PlayersManager:: InorderRec(GroupData *group, int **Players, int numOfGroups, int count){
        if(count == 0 || group == nullptr) return;
        /*
        GroupData *leftGroup = (get group on the left);
        InorderRec(leftGroup, Players, numOfGroups, count);

        PlayerId bestPlayer = group.best_in_group;
        Players[count] = bestPlayer;
        count++;

        GroupData *rightGroup = (get group on the right);
        InorderRec(rightGroup, Players, numOfGroups, count);
        */
    } 
    
    
    void PlayersManager::Quit(){
        //destroy everything (call destructors actively?)
    }

}