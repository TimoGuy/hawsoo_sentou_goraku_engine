#include "entity_player.h"


Entity_player::Entity_player()
    : Simulating_entity_ifc{}
    , m_world_entity{ false }
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

