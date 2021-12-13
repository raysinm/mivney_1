
#include <exception>
#include <string.h>
#include "./PlayersManager.h"


using namespace AVL;
namespace PM{

    StatusType PlayersManager::AddGroup(int groupId){ 
        try{
            GroupKey new_group_key(groupId);
            GroupData new_group_data(groupId);
            if(groups.AVLExist(new_group_key)) return FAILURE;
            groups.AVLInsert(new_group_key, new_group_data);

            return SUCCESS;
        }catch(...){
            return ALLOCATION_ERROR;
        }
    }
    
    StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level){ 
        try{    
        if(all_players.AVLExist(PlayerID)){
            return FAILURE;
        }
        PlayerKey new_player_key(PlayerID, Level);
        GroupKey new_group_key(GroupID);
        if(!groups.AVLExist(new_group_key)){
            return FAILURE;
        }

        GroupData& group_data = groups.AVLGet(new_group_key);
        auto& players = group_data.group_players;
        int player_id = PlayerID;
        PlayerData new_player_data(&group_data, player_id, Level);
        int prev_group_size = players.size();

        players.AVLInsert(new_player_key, new_player_data); 
        PlayerData* new_player = &players.AVLGet(new_player_key);
        

        if(prev_group_size == 0){
            best_in_non_empty_groups.AVLInsert(GroupKey(GroupID),PlayerID);
            group_data.best_in_group = new_player_key;
        }
        else{ 
            auto pre_best_in_group = group_data.best_in_group;

            group_data.best_in_group = players.AVLMax();


            if(group_data.best_in_group < pre_best_in_group){
                if(best_in_non_empty_groups.AVLExist(GroupKey(GroupID))){
                    best_in_non_empty_groups.AVLRemove(GroupKey(GroupID));
                    best_in_non_empty_groups.AVLInsert(GroupKey(GroupID),PlayerID);
                }
            }
        }

        all_players.AVLInsert(PlayerID, new_player);
        all_players_sorted.AVLInsert(new_player_key, new_player);
        best_of_all = all_players_sorted.AVLMax().id;

        return SUCCESS;
        }catch(...){
            return ALLOCATION_ERROR;
        }
    }

    StatusType PlayersManager::RemovePlayer(int PlayerID){ 
        
        if(!all_players.AVLExist(PlayerID)) return FAILURE;

        PlayerData& player_data = *(all_players.AVLGet(PlayerID));
        PlayerData after_rem_player_data(player_data);
        GroupData*& after_rem_group_data = after_rem_player_data.owner_group_data;
        auto& player_group_tree = after_rem_group_data->group_players;
        int level = player_data.level;
        PlayerKey player_key(PlayerID, level);

        
        all_players_sorted.AVLRemove(player_key);

        if(all_players_sorted.size() != 0){
            best_of_all = all_players_sorted.AVLMax().id;
        }else{
            best_of_all = -1;
        }

        player_group_tree.AVLRemove(player_key);

        int GroupID = after_rem_group_data->group_id;
        auto prev_best_in_group = after_rem_group_data->best_in_group;
        if(player_group_tree.size() != 0){
            after_rem_group_data->best_in_group = player_group_tree.AVLMax();
            
        }else if(player_group_tree.size() == 0){
            after_rem_group_data->best_in_group.id = -1;
            after_rem_group_data->best_in_group.level = -1;

            best_in_non_empty_groups.AVLRemove(GroupKey(after_rem_group_data->group_id));
        }

        if((player_group_tree.size() != 0) && !(prev_best_in_group == after_rem_group_data->best_in_group)){
            best_in_non_empty_groups.AVLRemove(GroupKey(GroupID));
            best_in_non_empty_groups.AVLInsert(GroupKey(GroupID),after_rem_group_data->best_in_group.id);              
        }

        all_players.AVLRemove(PlayerID);

        
        return SUCCESS;
    }

    StatusType PlayersManager::ReplaceGroup(int GroupID, int ReplacementID){    //O(n_group + n_replacement + logk) k-number of groups
        try{

            if(!(groups.AVLExist(GroupKey(GroupID)) && groups.AVLExist(GroupKey(ReplacementID)))) return FAILURE;

            auto& group_data = groups.AVLGet(GroupKey(GroupID));
            auto& players_to_move = group_data.group_players;
            auto& replacement_data = groups.AVLGet(GroupKey(ReplacementID));
            auto& players = replacement_data.group_players;

            if(players_to_move.size() == 0){
                groups.AVLRemove(GroupKey(GroupID));

            }
            else{
                for(PlayerData& old_player_data : players_to_move){
                    old_player_data.owner_group_data = &replacement_data;
                }

                if( group_data.best_in_group < replacement_data.best_in_group){
                    replacement_data.best_in_group = group_data.best_in_group;
                    if(replacement_data.group_players.size() != 0){
                        if(best_in_non_empty_groups.AVLExist(GroupKey(GroupID))){
                            best_in_non_empty_groups.AVLRemove(GroupKey(ReplacementID));
                            best_in_non_empty_groups.AVLInsert(GroupKey(ReplacementID),group_data.best_in_group.id);
                        }

                    }else if(players_to_move.size() != 0 && replacement_data.group_players.size() == 0){
                        best_in_non_empty_groups.AVLInsert(GroupKey(ReplacementID), group_data.best_in_group.id);
                    }
                }
                
                players.AVLMerge(players_to_move);
                groups.AVLRemove(GroupKey(GroupID));

                best_in_non_empty_groups.AVLRemove(GroupKey(GroupID));
            }

            return SUCCESS;
        
        }catch(std::bad_alloc &e){
            return ALLOCATION_ERROR;
        }
    }

    
    StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease){  //O(logn) n- TOTAL number of players
        
        try{
            if(!all_players.AVLExist(PlayerID)) return FAILURE;

            PlayerData*& player_old_data = all_players.AVLGet(PlayerID);
            GroupData& player_group_data = *(player_old_data->owner_group_data);
            auto& player_group_tree = player_group_data.group_players;
            int GroupID =  player_group_data.group_id;

            int old_level = player_old_data->level;
            int new_level = old_level + LevelIncrease;

            PlayerKey player_old_key(PlayerID, old_level);
            PlayerKey player_new_key(PlayerID, new_level);
            PlayerData player_new_data(&player_group_data, PlayerID, new_level);
            
            player_group_tree.AVLRemove(player_old_key);
            player_group_tree.AVLInsert(player_new_key, player_new_data);
            PlayerData* new_player = &player_group_tree.AVLGet(player_new_key);
            auto old_best = player_group_data.best_in_group;
            player_old_data = new_player;

            player_group_data.best_in_group = player_group_tree.AVLMax();

            if(!(old_best == player_group_data.best_in_group)){
                if(best_in_non_empty_groups.AVLExist(GroupKey(GroupID))){
                    best_in_non_empty_groups.AVLRemove(GroupKey(GroupID));
                    best_in_non_empty_groups.AVLInsert(GroupKey(GroupID),PlayerID);
                }
            }
            
            all_players_sorted.AVLRemove(player_old_key);
            all_players_sorted.AVLInsert(player_new_key, new_player);
            best_of_all = all_players_sorted.AVLMax().id;

            return SUCCESS;

        }catch(...){
            return ALLOCATION_ERROR;
        }
    }

    
    StatusType PlayersManager::GetHighestLevel(int GroupID, int *PlayerID){   //O(logk) k- num of groups. if GroupId<0 : O(1)
        if(GroupID < 0)
        {

            *PlayerID = best_of_all;
            return SUCCESS;
        }
        else{
            

            if(!groups.AVLExist(GroupKey(GroupID))){
                return FAILURE;
            }
            GroupData& groupData = groups.AVLGet(GroupKey(GroupID));
            *PlayerID = groupData.best_in_group.id;
            return SUCCESS;
        }
    }
 
        
    StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers){ //O(n_groupId +logk) k-num of groups
        int* players_arr;
        try{
            if(GroupID >0){
                GroupKey new_group_key(GroupID);
                if(!groups.AVLExist(new_group_key)){
                    throw Failure();
                }

                auto& group_data = groups.AVLGet(new_group_key);
                auto& players = group_data.group_players;

 
                if(players.size() == 0){
                    throw Skip();
                }

                *numOfPlayers = players.size();
                players_arr = (int*) malloc (sizeof(int) * (*numOfPlayers));

                int i = 0;
                for(const PlayerData& current_id : players){
                    players_arr[i] = current_id.id;
                    i++;
                    if(i >= *numOfPlayers) break;
                }

                *Players = players_arr;

                return SUCCESS;

            }else{
                if(all_players.size() == 0){
                    throw Skip();
                }

                *numOfPlayers = all_players.size();
                int* players_arr = (int*) malloc (sizeof(int) * (*numOfPlayers));
                

                int i = 0;
                for(const PlayerData* current_id : all_players_sorted){
                    players_arr[i] = current_id->id;
                    i++;
                    if(i >= *numOfPlayers) break;
                }
                
                *Players = players_arr;

                return SUCCESS;
            }
        }catch(Skip& e){
            *numOfPlayers = 0;
            *Players = NULL;
            return SUCCESS;
        }catch(Failure& e){
            *numOfPlayers = 0;
            *Players = NULL;
            return FAILURE;
        }catch(std::bad_alloc& e){
            *numOfPlayers = 0;
            *Players = NULL;
            return ALLOCATION_ERROR;
        }

    } 
    
    StatusType PlayersManager::GetGroupsHighestLevel(int numOfGroups, int **Players){ 
        int* players_arr = *Players;
        try{
            if(numOfGroups > best_in_non_empty_groups.size()) return FAILURE;

            players_arr = (int*) malloc(numOfGroups * sizeof(int));

            int i = 0;
            for (int id : best_in_non_empty_groups)
            {
                players_arr[i] = id; 
                i++;
                if (i >= numOfGroups) break;
            }
            *Players = players_arr;
            return SUCCESS;

        }catch(...){
            return ALLOCATION_ERROR;
        }

    }  

}