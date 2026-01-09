#include <Application.h>

#include <thread>
#include <chrono>

#include <LV_Registration.h>

#include <Sound_Engine.h>
#include <Devices/Input_Device.h>
#include <Sound.h>

#include <Net_Engine.h>
#include <Client_Socket.h>
#include <Server_Socket.h>

#include <Common_Constants.h>

using namespace LVC;


Application::Application()
{
    LV::register_types(m_object_constructor);

    LSound::Sound_Engine::instance();
    LNet::Net_Engine::instance();

    L_CREATE_LOG_LEVEL(LNet::Net_Engine::instance().log_level());

    M_register_messages();
    M_register_types();
    M_on_components_initialized();
}

Application::~Application()
{

}



void Application::M_register_messages()
{

}

void Application::M_register_types()
{

}

void Application::M_on_components_initialized()
{

}



void Application::run()
{
    constexpr unsigned int Duration_Milliseconds = 500;

    LSound::Input_Device_Settings device_settings = LSound::Input_Device_Settings::voip_standard((float)Duration_Milliseconds * 0.001f);
    LSound::Input_Device input_device(device_settings);

    LNet::Client_Socket client_socket(Max_Message_Size);

    std::string pause_crutch;
    std::cin >> pause_crutch;

    client_socket.connect("127.0.0.1", Port);
    // client_socket.connect("192.168.1.105", 12345);

    while(true)
    {
        std::cout << "\nstarting capture" << std::endl;

        input_device.start_capture();
        std::this_thread::sleep_for(std::chrono::milliseconds(Duration_Milliseconds));
        input_device.stop_capture();

        LSound::Sound_Data* sound_data = input_device.extract_capture();
        L_ASSERT(sound_data);

        std::cout << "raw data size = "  << sound_data->raw_data().size << " bytes" << std::endl;

        LNet::Package package;
        package.append_data(sound_data->raw_data().data, sound_data->raw_data().size);

        bool sent = client_socket.send(package);

        delete sound_data;

        if(sent)
            continue;

        std::cout << "error sending package" << std::endl;
    }
}
