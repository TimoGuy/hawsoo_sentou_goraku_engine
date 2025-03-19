#include "entity_player.h"


Entity_player::Entity_player()
    : Simulating_entity_ifc{}
    , World_entity{ false }
    , m_actor{ // @TODO: START HERE!!!!!!}
{

}

void Entity_player::on_sim_entity_start(pool_elem_key_t key)
{
    m_sim_entity_key = key;
}

void Entity_player::tick_sim_entity()
{
    
}

void Entity_player::on_sim_entity_destroy()
{

}

