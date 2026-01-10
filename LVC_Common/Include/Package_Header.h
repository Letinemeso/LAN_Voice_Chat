#pragma once


namespace LVC
{

    enum Command_Type
    {
        Unknown = 0,
        Handshake,
        Sound_Data
    };


    struct Package_Header
    {
        Command_Type command_type = Command_Type::Unknown;
        long unsigned int timestamp = 0;
    };

}
