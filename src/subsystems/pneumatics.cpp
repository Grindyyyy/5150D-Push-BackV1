#include "subsystems/pneumatics.hpp"

Pneumatics::Pneumatics(
    char loader_port,
    char nanner_port,
    char descore_port
) : loader(loader_port), nanner(nanner_port), descore(descore_port)
{
    loader.set_value(false);
    nanner.set_value(false);
    descore.set_value(false);
};

void Pneumatics::set_loader(bool state){
    loader.set_value(state);
}

void Pneumatics::set_nanner(bool state){
    nanner.set_value(state);
}

void Pneumatics::set_descore(bool state){
    descore.set_value(state);
}