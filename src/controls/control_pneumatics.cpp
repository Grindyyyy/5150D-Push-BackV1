#include "controls/control_pneumatics.h"
#include "api.h"

std::atomic<bool> nanner{false};
std::atomic<bool> loader{false};
std::atomic<bool> descore{false};

void control_pneumatics(Pneumatics& pneumatics, pros::Controller& master){
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
        bool curr = nanner.load();
        nanner.store(!curr);
        pneumatics.set_nanner(nanner);
    }

    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
        bool curr = loader.load();
        loader.store(!curr);
        pneumatics.set_loader(loader);
    }

    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
        bool curr = descore.load();
        descore.store(!curr);
        pneumatics.set_descore(descore);
    }
}