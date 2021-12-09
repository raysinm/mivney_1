#include <iostream>
#include "DS.h"

void printPlayer(const std::string& prefix, const PlayerNode* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──────" : "└──────" );

//         print the value of the node
        std::cout << "player id:" << (node->player_data->player_id) << " player group : " <<(node->player_data->playing_group) << std::endl;//" height is "<< node->height << std::endl;

//         enter the next tree level - left and right branch
        printPlayer( prefix + (isLeft ? "│       " : "        "), node->right, true);
        printPlayer( prefix + (isLeft ? "│       " : "        "), node->left, false);
    }
}

void printGroups(const std::string& prefix, const GroupFullNode* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──────" : "└──────" );

//         print the value of the node
        std::cout << "group id:" << (node->group_id) << " group size: " <<(node->group_players->size) << std::endl;
        printPlayer("",node->group_players->head, false);
        // enter the next tree level - left and right branch
        printGroups( prefix + (isLeft ? "│       " : "        "), node->right, true);
        printGroups( prefix + (isLeft ? "│       " : "        "), node->left, false);
    }
}

void printGroupsIds(const std::string& prefix, const GroupIdNode* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──────" : "└──────" );

//         print the value of the node
        std::cout << "group id:" << (node->group_id) << std::endl;
//         enter the next tree level - left and right branch

        printGroupsIds( prefix + (isLeft ? "│       " : "        "), node->right, true);
        printGroupsIds( prefix + (isLeft ? "│       " : "        "), node->left, false);
    }
}

void printBT(const DS* ds)
{

    std::cout << "\ngroups id"  << std::endl;
    printGroupsIds("", ds->groups_ids->head, false);

    std::cout << "\ngroups tree"  << std::endl;
    printGroups("", ds->groups_not_empty->head, false);

    std::cout << "\nplayers_by_level tree"  << std::endl;
    printPlayer("",ds->players_by_level->head, false);
    if(ds->players_by_level->max){
        std::cout << "\nplayers_by_level max_player_id: "<< ds->players_by_level->max->player_data->player_id  << std::endl;
    }
    std::cout << "\nplayers_by_id tree"  << std::endl;
    printPlayer("",ds->players_by_id->head, false);
    if(ds->players_by_id->max){
        std::cout << "\nplayers_by_id max_player_id: "<< ds->players_by_id->max->player_data->player_id  << std::endl;
    }

}


