
#include <exception>
#include "./PlayersManager.h"


using namespace AVL;
namespace PM{

    StatusType PlayersManager::AddGroup(int groupId){   //O(logk) k- num of groups
        try{
            GroupKey new_group_key(groupId);
            GroupData new_group_data(groupId);
            groups.AVLInsert(new_group_key, new_group_data);
            return SUCCESS;
        }catch(...){
            return ALLOCATION_ERROR;
        }
    }
    
    StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level){ //O(logn + logk) n- num of players in group, k- num of groups
        try{

        PlayerKey new_player_key(PlayerID, Level);
        if(!groups.AVLExist(GroupID)){
            return FAILURE;
        }
        auto group_data = groups.AVLGet(GroupID);
        auto& players = group_data.group_players;
        int player_id = PlayerID;

        PlayerData new_player_data(&group_data, player_id, Level);
        int prev_group_size = players.size();

        players.AVLInsert(new_player_key, new_player_data);

        group_data.best_in_group = players.AVLMax().id;

        if(prev_group_size == 0){
            ++this->num_of_nonempty_groups;
            best_in_non_empty_groups.AVLInsert(GroupID,PlayerID);
        }
        all_players.AVLInsert(PlayerID, &new_player_data);
        all_players_sorted.AVLInsert(new_player_key, &new_player_data);
        best_of_all = all_players_sorted.AVLMax().id;
        return SUCCESS;
        }catch(...){
            return ALLOCATION_ERROR;
        }
        /* auto player_group = groups.AVLFind(GroupID);

        GroupKey new_group_key = new GroupKey(GroupId);
        PlayerKey new_player_key = new PlayerKey(int, Level);
        
        groups.AVLFind(new_group_key);
        return player_group.players.AVLInsert(PlayerID, Level); */
    }

    //DONE
    StatusType PlayersManager::RemovePlayer(int PlayerID){  //O(logn) n- num of players TOTAL   -should we keep another tree of players only?
        if(!all_players.AVLExist(PlayerID)) return FAILURE;
        auto& player_data = *(all_players.AVLGet(PlayerID));
        auto& player_group_data = *(player_data.owner_group_data);
        auto& player_group_tree = player_group_data.group_players;
        int level = player_data.level;
        PlayerKey player_key(PlayerID, level);
        all_players_sorted.AVLRemove(player_key);
        player_group_tree.AVLRemove(player_key);
        best_of_all = all_players_sorted.AVLMax().id;
        player_group_data.best_in_group = player_group_tree.AVLMax().id;
        if(player_group_tree.size() == 0){
            num_of_nonempty_groups--;
            best_in_non_empty_groups.AVLRemove(GroupKey(player_group_data.group_id));
        }
        all_players.AVLRemove(PlayerID);
        return SUCCESS;
    }

    StatusType PlayersManager::ReplaceGroup(int GroupID, int ReplacementID){    //O(n_group + n_replacement + logk) k-number of groups
        try{
            if(!(groups.AVLExist(GroupID) && groups.AVLExist(ReplacementID))) return FAILURE;
            auto& group_data = groups.AVLGet(GroupID);
            auto& players_to_move = group_data.group_players;
            auto& replacement_data = groups.AVLGet(ReplacementID);
            auto& players = replacement_data.group_players;
            if(players_to_move.size() == 0){
                groups.AVLRemove(GroupID);
            }
            else{
                AVL::AVLTree<PlayerKey,PlayerData>::Iterator to_move_iter(players_to_move);
                to_move_iter.begin();
                for(int i = 0; i < players_to_move.size(); i++){
                    auto old_player_data = *to_move_iter;
                    old_player_data.owner_group_data = &replacement_data;
                    to_move_iter++;
                }

                if(replacement_data.best_in_group < group_data.best_in_group){
                    replacement_data.best_in_group = group_data.best_in_group;
                    auto old_best_data = best_in_non_empty_groups.AVLGet(ReplacementID);
                    old_best_data = int(group_data.best_in_group);
                }

                players.AVLMerge(players_to_move);//make sure used merge right and new tree is inserted back
                groups.AVLRemove(GroupID);
                num_of_nonempty_groups--;
            }
            return SUCCESS;
        
        }catch(...){
            return ALLOCATION_ERROR;
        }
       /*  auto group_to_remove = groups.AVLFind(GroupID);
        auto group_to_merge = groups.AVLFind(ReplacementID);
        if(group_to_remove == nullptr || group_to_merge == nullptr) return FAILURE;
        //merge - dont have func yet: merge(group_to_remove.players, group_to_merge.players) and put into group_to_merge
        return groups.AVLRemove(GroupID); */
    }

    //DONE
    StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease){  //O(logn) n- TOTAL number of players
        
        try{
            if(!all_players.AVLExist(PlayerID)) return FAILURE;

            auto& player_old_data = *(all_players.AVLGet(PlayerID));
            GroupData& player_group_data = *(player_old_data.owner_group_data);
            auto& player_group_tree = player_group_data.group_players;

            int old_level = player_old_data.level;
            int new_level = old_level + LevelIncrease;

            PlayerKey player_old_key(PlayerID, old_level);
            PlayerKey player_new_key(PlayerID, new_level);
            PlayerData player_new_data(&player_group_data, new_level, PlayerID);
            
            player_group_tree.AVLRemove(player_old_key);
            player_group_tree.AVLInsert(player_new_key, player_new_data);

            auto old_best = player_group_data.best_in_group;
            player_group_data.best_in_group = player_group_tree.AVLMax().id;

            if(old_best != player_group_data.best_in_group){
                auto& player_data = best_in_non_empty_groups.AVLGet(old_best);
                player_data = player_new_data.id;
            }

            all_players_sorted.AVLRemove(player_old_key);
            all_players_sorted.AVLInsert(player_new_key, &player_new_data);
            best_of_all = all_players_sorted.AVLMax().id;

            return SUCCESS;
        }catch(...){
            return ALLOCATION_ERROR;
        }
    }

    //DONE
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
    }
 
        
    StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers){ //O(n_groupId +logk) k-num of groups
        try{
            if(GroupID >0){
                if(!groups.AVLExist(GroupID)){
                    throw Failure();
                }

                auto& group_data = groups.AVLGet(GroupID);
                auto& players = group_data.group_players;

                if(players.size() == 0){
                    throw Skip();
                }

                *numOfPlayers = players.size();
                int* players_arr = (int*) malloc (sizeof(int) * (*numOfPlayers));
                Players = &players_arr;

                AVL::AVLTree<PlayerKey,PlayerData>::Iterator players_iter(players);
                players_iter.begin();

                for(int i = 0; i < *numOfPlayers; i++){
                    auto current_id = int((*players_iter).id);
                    players_arr[i] = current_id;
                    players_iter++;
                }
                return SUCCESS;

            }else{
                if(all_players.size() == 0){
                    throw Skip();
                }

                *numOfPlayers = all_players.size();
                int* players_arr = (int*) malloc (sizeof(int) * (*numOfPlayers));
                Players = &players_arr;

                AVL::AVLTree<int,PlayerData*>::Iterator all_players_iter(all_players);
                all_players_iter.begin();

                for(int i = 0; i < *numOfPlayers; i++){
                    auto current_id = int((*all_players_iter)->id);
                    players_arr[i] = current_id;
                    all_players_iter++;
                }
                return SUCCESS;
            }
        }catch(Skip){
            *numOfPlayers = 0;
            Players = NULL;
            return SUCCESS;
        }catch(Failure){
            *numOfPlayers = 0;
            Players = NULL;
            return FAILURE;
        }catch(std::bad_alloc){
            *numOfPlayers = 0;
            Players = NULL;
            return ALLOCATION_ERROR;
        }
        /*
        if GroupId<0:
            InOrder scan (with node limit) and input in players array in *all_players_sorted*
        else
            InOrder scan (with node limit) and input in players array in *group_tree* (find the group first)
        */
    } 
    

    StatusType PlayersManager::GetGroupsHighestLevel(int numOfGroups, int **Players){   //O(numOfGroups +logk) k-num of groups
        try{
            if(numOfGroups < num_of_nonempty_groups) return FAILURE;

            //players should be allocated in library1.cpp
            auto tree_iter = best_in_non_empty_groups.iterator;
            tree_iter.begin();

            for(int i; i < numOfGroups; i++){
                Players[i] = &(*tree_iter);
                tree_iter++;
            }
            return SUCCESS;

        }catch(...){
            return ALLOCATION_ERROR;
        }
        /*
        use numOfGroups as counter to inorder scan function.
        EMPTY group- doesnt count!
        1) check if numOfgroups smaller than number of non-empty groups (should be in PlayersManager), return error
        2) Inorder function with node limit- access each group's best_player (if not empty)
        */
    }  

   

    

}