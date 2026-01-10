#pragma once

namespace LVC
{

    constexpr int Port = 73556;
    constexpr int Max_Message_Size = 32000;

    constexpr unsigned int Handshake_Expectation_Ms = 500;

    constexpr float Client_Existence_Duration = 2.0f;

    constexpr float Voice_Package_Duration = 0.15f;
    constexpr unsigned int Voice_Package_Duration_Ms = Voice_Package_Duration * 1000.0f;
    constexpr unsigned int Max_Delay_Ms = 20;

}
