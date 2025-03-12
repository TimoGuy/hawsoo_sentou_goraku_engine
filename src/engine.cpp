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
        std::vector<Job_source*> job_sources;
        Monolithic_renderer renderer{
            app_name,
            screen_width,
            screen_height,
            fallback_screen_width,
            fallback_screen_height
        };
        World_simulation simulation{ };
        job_sources.emplace_back(&renderer);
        job_sources.emplace_back(&simulation);

        // Create job system with job sources.
        Job_system job_system{ num_threads, std::move(job_sources) };
        return job_system.run();
    }

    std::vector<World_entity> m_world_entities;
};

// Wrapper implementation.
Engine::Engine()
    : m_pimpl(std::make_unique<Impl>())
{
}

Engine::~Engine() = default;  // Define in .cpp for smart ptr pimpl.

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
