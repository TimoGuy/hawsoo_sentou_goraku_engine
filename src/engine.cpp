#include "engine.h"

// Only 64 bit system is supported.
#include <cstdint>
#if INTPTR_MAX != INT64_MAX
    #error "Only 64 bit environment is supported"
#endif

#include <atomic>
#include <vector>
#include "input_handling_public.h"
#include "monolithic_renderer_public.h"
#include "ticking_world_simulation_public.h"
#include "world_entity_public.h"

// @NOCHECKIN: ASDFASDFASDF //
#include "simulating_ifc.h"
#include <memory>
#include "standard_behaviors.h"
#include "physics_objects.h"
//////////////////////////////


// Impl.
class Engine::Impl
{
public:
    bool run(
        const std::string& app_name,
        uint32_t num_threads,
        int32_t screen_width,
        int32_t screen_height,
        int32_t fallback_screen_width,
        int32_t fallback_screen_height)
    {
        // Init input handling.
        input_handling::set_num_state_sets(1);  // Single player.
        input_handling::init_key_mouse_adapter();

        // Set default input scheme.
        using IHC_e = input_handling::Key_mouse_control;
        input_handling::set_key_binding_map({
            { IH_KEY_A,            IHC_e::GP_MOVEMENT_LEFT           },
            { IH_KEY_D,            IHC_e::GP_MOVEMENT_RIGHT          },
            { IH_KEY_W,            IHC_e::GP_MOVEMENT_UP             },
            { IH_KEY_S,            IHC_e::GP_MOVEMENT_DOWN           },
            { IH_KEY_SPACE,        IHC_e::GP_JUMP_BOOL               },
            { IH_KEY_LEFT_SHIFT,   IHC_e::GP_DODGE_SPRINT_BOOL       },
            { IH_KEY_LEFT_CONTROL, IHC_e::GP_SWITCH_WEAPON_BOOL      },
            { IH_KEY_E,            IHC_e::GP_INTERACT_BOOL           },
            { IH_KEY_A,            IHC_e::UI_NAVIGATE_MOVEMENT_LEFT  },
            { IH_KEY_D,            IHC_e::UI_NAVIGATE_MOVEMENT_RIGHT },
            { IH_KEY_W,            IHC_e::UI_NAVIGATE_MOVEMENT_UP    },
            { IH_KEY_S,            IHC_e::UI_NAVIGATE_MOVEMENT_DOWN  },
            { IH_KEY_ENTER,        IHC_e::UI_CONFIRM_BOOL            },
            { IH_KEY_SPACE,        IHC_e::UI_CONFIRM_BOOL            },
            { IH_KEY_ESCAPE,       IHC_e::UI_CANCEL_BOOL             },
            { IH_KEY_A,            IHC_e::LE_MOVEMENT_LEFT           },
            { IH_KEY_D,            IHC_e::LE_MOVEMENT_RIGHT          },
            { IH_KEY_W,            IHC_e::LE_MOVEMENT_UP             },
            { IH_KEY_S,            IHC_e::LE_MOVEMENT_DOWN           },
            { IH_KEY_E,            IHC_e::LE_MOVE_WORLD_Y_AXIS_UP    },
            { IH_KEY_Q,            IHC_e::LE_MOVE_WORLD_Y_AXIS_DOWN  },
            { IH_KEY_LEFT_SHIFT,   IHC_e::LE_LSHIFT_MODIFIER_BOOL    },
            { IH_KEY_LEFT_CONTROL, IHC_e::LE_LCTRL_MODIFIER_BOOL     },
        });
        input_handling::set_mousebutton_binding_map({
            { IH_MOUSE_BUTTON_LEFT,  IHC_e::GP_ATTACK_BOOL        },
            { IH_MOUSE_BUTTON_RIGHT, IHC_e::GP_DEFLECT_BLOCK_BOOL },
            { IH_MOUSE_BUTTON_RIGHT, IHC_e::LE_CAMERA_MOVE_BOOL   },
        });

        // Instantiate job sources.
        std::atomic_size_t num_job_sources_setup_incomplete{ 2 };

        std::vector<Job_source*> job_sources;
        Monolithic_renderer renderer{
            num_job_sources_setup_incomplete,
            app_name,
            screen_width,
            screen_height,
            fallback_screen_width,
            fallback_screen_height
        };
        World_simulation simulation{
            num_job_sources_setup_incomplete,
            num_threads
        };

        // @TODO: @NOCHECKIN: Sample level. //
        class Player_entity : public simulating::Entity_ifc
        {
        public:
            Player_entity(Monolithic_renderer& renderer)
                : m_renderer(renderer)
            {
            }

            void on_create(simulating::Edit_behavior_groups_ifc& editor,
                           size_t creation_idx) override
            {
                auto phys_actor{
                    phys_obj::Actor_character_controller(
                        JPH::RVec3{ 0.0f, 0.0f, 0.0f },
                        phys_obj::ACTOR_CC_TYPE_PLAYER,
                        phys_obj::Shape_params_cylinder{
                            .radius = 0.5f,
                            .half_height = 1.0f,
                        }) };

                m_transform_holder =
                    std::make_unique<phys_obj::Transform_holder>(false, phys_actor);

                m_render_geo_obj_key =
                    m_renderer.create_render_geo_obj("model_player",
                                                     "mat_set_player_0",
                                                     geo_instance::Geo_render_pass::OPAQUE,
                                                     true,
                                                     m_transform_holder.get());
                assert(false);  // @ASDFASDF: @TODO: @INCOMPLETE: Add connection from transform holder to render object.

                std::vector<std::unique_ptr<simulating::Behavior_ifc>> behaviors;
                behaviors.reserve(3);
                auto& input{
                    behaviors.emplace_back(
                        std::make_unique<std_behavior::Gamepad_input_behavior>()) };
                auto& movement{
                    behaviors.emplace_back(
                        std::make_unique<std_behavior::Humanoid_movement>(std::move(phys_actor))) };
                auto& animator{
                    behaviors.emplace_back(
                        std::make_unique<std_behavior::Humanoid_animator>()) };
                assert(behaviors.size() == 3);

                static_cast<std_behavior::Gamepad_input_behavior*>(input.get())
                    ->set_output(movement->get_data_key());
                static_cast<std_behavior::Humanoid_movement*>(movement.get())
                    ->set_animator(animator->get_data_key());

                m_behavior_group_key = editor.add_behavior_group(std::move(behaviors));
            }

            void on_teardown(
                simulating::Edit_behavior_groups_ifc& editor) override
            {
                m_transform_holder = nullptr;
                editor.remove_behavior_group(m_behavior_group_key);
                m_renderer.destroy_render_geo_obj(m_render_geo_obj_key);
            }

        private:
            Monolithic_renderer& m_renderer;

            using behavior_group_key_t =
                simulating::Edit_behavior_groups_ifc::behavior_group_key_t;
            using render_geo_obj_key_t =
                Monolithic_renderer::render_geo_obj_key_t;

            std::unique_ptr<phys_obj::Transform_holder> m_transform_holder;
            render_geo_obj_key_t m_render_geo_obj_key;
            behavior_group_key_t m_behavior_group_key;
        };

        class Ground : public simulating::Entity_ifc
        {
        public:
            Ground(Monolithic_renderer& renderer)
                : m_renderer(renderer)
            {
            }

            void on_create(simulating::Edit_behavior_groups_ifc& editor,
                           size_t creation_idx) override
            {
                std::vector<uint32_t> shapes_memory;
                shapes_memory.reserve(3);

                auto kinematic_phys_actor{
                    phys_obj::Actor_kinematic(
                        JPH::RVec3{ 0.0f, -10.0f, 0.0f },
                        JPH::Quat::sIdentity(),
                        {
                            {
                                .shape_type = phys_obj::SHAPE_TYPE_BOX,
                                .shape_params{
                                    phys_obj::construct_shape_into_memory<phys_obj::Shape_params_box>(
                                        shapes_memory,
                                        phys_obj::Shape_params_box{
                                            .half_x = 10.0f,
                                            .half_y = 1.0f,
                                            .half_z = 10.0f,
                                        }
                                    ) },
                            },
                        }) };

                assert(shapes_memory.size() != shapes_memory.max_size());

                m_transform_holder =
                    std::make_unique<phys_obj::Transform_holder>(false, kinematic_phys_actor);

                m_render_geo_obj_key =
                    m_renderer.create_render_geo_obj("model_ground",
                                                     "mat_set_ground_0",
                                                     geo_instance::Geo_render_pass::OPAQUE,
                                                     true,
                                                     m_transform_holder.get());
                // @IDEA: @THEA: Perhaps could have there be an easy to create wrapper for a geo instance with a transform holder/reader. That might be really good!
                // @IDEA: And then you could have another wrapper with the same interface for creating one without a transform reader. This kind could be manually updated!
                // @TODO: There would have to be a way to notify the renderer to update its instance information when updating information from here tho.
                assert(false);  // @ASDFASDF: @TODO: @INCOMPLETE: Add connection from transform holder to render object.

                std::vector<std::unique_ptr<simulating::Behavior_ifc>> behaviors;
                behaviors.reserve(1);
                auto& collider{
                    behaviors.emplace_back(
                        std::make_unique<std_behavior::Kinematic_collider>(std::move(kinematic_phys_actor))) };
                assert(behaviors.size() == 1);

                m_behavior_group_key = editor.add_behavior_group(std::move(behaviors));
            }

            void on_teardown(
                simulating::Edit_behavior_groups_ifc& editor) override
            {
                m_transform_holder = nullptr;
                editor.remove_behavior_group(m_behavior_group_key);
                m_renderer.destroy_render_geo_obj(m_render_geo_obj_key);
            }

        private:
            Monolithic_renderer& m_renderer;

            using behavior_group_key_t =
                simulating::Edit_behavior_groups_ifc::behavior_group_key_t;
            using render_geo_obj_key_t =
                Monolithic_renderer::render_geo_obj_key_t;

            std::unique_ptr<phys_obj::Transform_holder> m_transform_holder;
            render_geo_obj_key_t m_render_geo_obj_key;
            behavior_group_key_t m_behavior_group_key;
        };

        simulation.add_sim_entity_to_world(std::make_unique<Player_entity>(renderer));
        simulation.add_sim_entity_to_world(std::make_unique<Ground>(renderer));
        //////////////////////////////////////

        // Compile job sources.
        job_sources.emplace_back(&renderer);
        job_sources.emplace_back(&simulation);
        assert(num_job_sources_setup_incomplete.load() == job_sources.size());

        // Create job system with job sources.
        Job_system job_system{ num_threads, std::move(job_sources) };
        return job_system.run();  // @NOTE: Does not return except until program completion.
    }
};

// Wrapper implementation.
Engine::Engine()
    : m_pimpl(std::make_unique<Impl>())
{
}

Engine::~Engine() = default;  // Define in .cpp for unique_ptr pimpl.

bool Engine::run(
    const std::string& app_name,
    uint32_t num_threads,
    int32_t screen_width,
    int32_t screen_height,
    int32_t fallback_screen_width,
    int32_t fallback_screen_height)
{
    return m_pimpl->run(app_name,
                        num_threads,
                        screen_width,
                        screen_height,
                        fallback_screen_width,
                        fallback_screen_height);
}
