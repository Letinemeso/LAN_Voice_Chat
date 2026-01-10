#pragma once

#include <Stuff/Stopwatch.h>

#include <Devices/Input_Device.h>

#include <Common_Constants.h>
#include <Network_Manager.h>


namespace LVC
{

    class Recorder
    {
    private:
        Network_Manager* m_network_manager = nullptr;

        LSound::Input_Device m_input_device = LSound::Input_Device_Settings::voip_standard(Voice_Package_Duration);
        LST::Stopwatch m_stopwatch;

    public:
        Recorder();
        ~Recorder();

    public:
        inline void inject_network_manager(Network_Manager* _ptr) { m_network_manager = _ptr; }

    private:
        void M_send_voice_package_if_needed();
        void M_start_recording_if_needed();

    public:
        void process();

    };

}
