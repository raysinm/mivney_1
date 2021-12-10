
#include <exception>
#include "./PlayersManager.h"


using namespace AVL;
namespace PM{

    StatusType PlayersManager::AddGroup(int groupId){   //O(logk) k- num of groups
        try{
            GroupKey new_group_key(groupId);
            GroupData new_group_data(groupId);
            if(groups.AVLExist(new_group_key)) return FAILURE;
            groups.AVLInsert(new_group_key, new_group_data);
            
            /* groups.printTree();
            std::cout << "ADDED group " << groupId << "\n" << std::endl;
             */

            return SUCCESS;
        }catch(...){
            return ALLOCATION_ERROR;
        }
    }
    
    StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level){ //O(logn + logk) n- num of players in group, k- num of groups
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

        players.AVLInsert(new_player_key, new_player_data); //INSERT TO PLAYERS GROUP
        PlayerData* new_player = &players.AVLGet(new_player_key);
        

        if(prev_group_size == 0){
            ++this->num_of_nonempty_groups;
            best_in_non_empty_groups.AVLInsert(GroupKey(GroupID),PlayerID);
        }
        auto pre_best_in_group = group_data.best_in_group;


        group_data.best_in_group = players.AVLMax();
        /* std::cout << "\nprevious best in group: " << group_data.best_in_group << std::endl;
        
        std::cout << "\nnew best in group: " << group_data.best_in_group << std::endl;
        
        std::cout << "\nBest Players Tree: " <<std::endl;
        best_in_non_empty_groups.printTreeData(); */

        if(group_data.best_in_group < pre_best_in_group){
            best_in_non_empty_groups.AVLRemove(GroupKey(GroupID));
            best_in_non_empty_groups.AVLInsert(GroupKey(GroupID),PlayerID);
            /* auto player_data_temp = best_in_non_empty_groups.AVLGet(GroupKey(GroupID));
            int* player_in_non_empty = &player_data_temp;
            *player_in_non_empty = group_data.best_in_group; */
        }
        /* std::cout << "\nBest Players Tree: " <<std::endl;
        best_in_non_empty_groups.printTreeData(); */

        all_players.AVLInsert(PlayerID, new_player);
        all_players_sorted.AVLInsert(new_player_key, new_player);
        best_of_all = all_players_sorted.AVLMax().id;
        return SUCCESS;
        }catch(...){
            return ALLOCATION_ERROR;
        }
    }

    StatusType PlayersManager::RemovePlayer(int PlayerID){  //O(logn) n- num of players TOTAL   -should we keep another tree of players only?
        
        if(!all_players.AVLExist(PlayerID)) return FAILURE;
        
        auto& player_data = *(all_players.AVLGet(PlayerID));
        auto& player_group_data = *(player_data.owner_group_data);
        auto& player_group_tree = player_group_data.group_players;
        int level = player_data.level;
        PlayerKey player_key(PlayerID, level);

        all_players_sorted.AVLRemove(player_key);
        player_group_tree.AVLRemove(player_key);
        
        if(all_players_sorted.size() != 0){
            best_of_all = all_players_sorted.AVLMax().id;
        }else{
            best_of_all = -1;
        }
        
        int GroupID = player_group_data.group_id;
        auto prev_best_in_group = player_group_data.best_in_group;
        if(player_group_tree.size() != 0){
            player_group_data.best_in_group = player_group_tree.AVLMax();
            
        }else{
            player_group_data.best_in_group.id = -1;
            player_group_data.best_in_group.id = -1;
            num_of_nonempty_groups--;
            best_in_non_empty_groups.AVLRemove(GroupKey(player_group_data.group_id));
        }

        if((player_group_tree.size() != 0) && !(prev_best_in_group == player_group_data.best_in_group)){
            best_in_non_empty_groups.AVLRemove(GroupKey(GroupID));
            best_in_non_empty_groups.AVLInsert(GroupKey(GroupID),player_group_data.best_in_group.id);
            /* int& player_in_non_empty = best_in_non_empty_groups.AVLGet(GroupKey(player_group_data.group_id));
            player_in_non_empty = player_group_data.best_in_group; */
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
            
            /* std::cout << "\nbefore merge" << std::endl;
            groups.printTree(); */
            
            if(players_to_move.size() == 0){
                groups.AVLRemove(GroupKey(GroupID));
                /* groups.printTree();
                std::cout << "REMOVED group EMPTY" << GroupID << "\n" << std::endl; */
            }
            else{
                for(PlayerData& old_player_data : players_to_move){
                    old_player_data.owner_group_data = &replacement_data;
                }

                if( group_data.best_in_group < replacement_data.best_in_group){
                    replacement_data.best_in_group = group_data.best_in_group;
                    if(replacement_data.group_players.size() != 0){
                        best_in_non_empty_groups.AVLRemove(GroupKey(ReplacementID));
                        best_in_non_empty_groups.AVLInsert(GroupKey(ReplacementID),replacement_data.best_in_group.id);
                        /* int* old_best_data = &best_in_non_empty_groups.AVLGet(GroupKey(ReplacementID)); //added&
                        *old_best_data = group_data.best_in_group; */
                    }else{
                        best_in_non_empty_groups.AVLInsert(GroupKey(ReplacementID), group_data.best_in_group.id);
                    }
                }
                
                players.AVLMerge(players_to_move);//make sure used merge right and new tree is inserted back
                groups.AVLRemove(GroupKey(GroupID));
                best_in_non_empty_groups.AVLRemove(GroupKey(GroupID));
                /* groups.printTree();
                std::cout << "REMOVED group " << GroupID << "\n" << std::endl; */
                num_of_nonempty_groups--;
            }

            /* std::cout << "\nafter merge and remove" << std::endl;
            groups.printTree(); */

            return SUCCESS;
        
        }catch(...){
            return ALLOCATION_ERROR;
        }
    }

    
    StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease){  //O(logn) n- TOTAL number of players
        
        try{
            if(!all_players.AVLExist(PlayerID)) return FAILURE;

            PlayerData*& player_old_data = all_players.AVLGet(PlayerID);
            GroupData& player_group_data = *(player_old_data->owner_group_data);
            auto& player_group_tree = player_group_data.group_players;
            int GroupID =  player_old_data->id;

            int old_level = player_old_data->level;
            int new_level = old_level + LevelIncrease;

            PlayerKey player_old_key(PlayerID, old_level);
            PlayerKey player_new_key(PlayerID, new_level);
            PlayerData player_new_data(&player_group_data, PlayerID, new_level);
            
            player_group_tree.AVLRemove(player_old_key);
            player_group_tree.AVLInsert(player_new_key, player_new_data);
            PlayerData* new_player = &player_group_tree.AVLGet(player_new_key);
            player_old_data = new_player;

            auto old_best = player_group_data.best_in_group;
            player_group_data.best_in_group = player_group_tree.AVLMax();

            if(!(old_best == player_group_data.best_in_group)){
                best_in_non_empty_groups.AVLRemove(GroupKey(GroupID));
                best_in_non_empty_groups.AVLInsert(GroupKey(GroupID),PlayerID);
                /* int* player_data = &best_in_non_empty_groups.AVLGet(GroupKey(old_best));
                *player_data = player_new_data.id; */
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
            /* std::cout << "\nall_players_sorted: " <<std::endl;
            all_players_sorted.printTree(); */
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
        int* players_arr = *Players;
        try{
            if(GroupID >0){
                GroupKey new_group_key(GroupID);
                if(!groups.AVLExist(new_group_key)){
                    throw Failure();
                }

                /* std::cout << "\nGroup: " << GroupID << std::endl;
                groups.printTree(); */
                
                
                auto& group_data = groups.AVLGet(new_group_key);
                auto& players = group_data.group_players;

                /* std::cout << "Players Tree: \n" <<std::endl;
                players.printTree(); */

                if(players.size() == 0){
                    throw Skip();
                }

                *numOfPlayers = players.size();
                players_arr = (int*) malloc (sizeof(int) * (*numOfPlayers));

                int i = 0;
                for(const PlayerData& current_id : players){
                    players_arr[i] = current_id.id;
                    i++;
                }
                return SUCCESS;

            }else{
                if(all_players.size() == 0){
                    throw Skip();
                }

                /* std::cout << "\nall_players: " <<std::endl;
                all_players.printTree();
                std::cout << "\nall_players_sorted: " <<std::endl;
                all_players_sorted.printTree();
                std::cout << "\nBest Players Tree: " <<std::endl;
                best_in_non_empty_groups.printTreeData();
                std::cout << "\nGroups Tree: " <<std::endl;
                groups.printTree();  */

                *numOfPlayers = all_players.size();
                int* players_arr = (int*) malloc (sizeof(int) * (*numOfPlayers));
                *Players = players_arr;

                

                int i = 0;
                for(const PlayerData* current_id : all_players_sorted){
                    players_arr[i] = current_id->id;
                    i++;
                    if(i >= *numOfPlayers){
                        break;
                    }
                }
                return SUCCESS;
            }
        }catch(Skip){
            *numOfPlayers = 0;
            *Players = NULL;
            return SUCCESS;
        }catch(Failure){
            *numOfPlayers = 0;
            *Players = NULL;
            return FAILURE;
        }catch(std::bad_alloc){
            *numOfPlayers = 0;
            *Players = NULL;
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
        int* players_arr = *Players;
        try{
            if(numOfGroups > num_of_nonempty_groups) return FAILURE;
            
            players_arr = (int*) malloc(numOfGroups * sizeof(int));

            int i = 0;
            for (int id : best_in_non_empty_groups)
            {
                if (i >= numOfGroups)
                {
                    break;
                }
                players_arr[i] = id; 
                i++;
            }
            *Players = players_arr;
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