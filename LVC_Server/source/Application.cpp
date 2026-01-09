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

using namespace LVC;


Application::Application()
{
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
    // std::cout << "starting server" << std::endl;

    // constexpr unsigned int Max_Sound_Data_Size = 32000;
    // LNet::Server_Socket server_socket(12345, Max_Sound_Data_Size);

    // LSound::Sound_Engine::instance().set_listener_volume_multiplier(2.0f);

    // LSound::Raw_Sound_Data raw_data;
    // raw_data.format = AL_FORMAT_MONO16;
    // raw_data.frequency = 16000;

    // LSound::Sound sound;

    // while(true)
    // {
    //     LNet::Server_Socket::Message client_message = server_socket.receive();
    //     if(client_message.package.raw_data_size() == 0)
    //     {
    //         std::cout << "error receiving message" << std::endl;
    //         continue;
    //     }

    //     std::cout << "received a message" << std::endl;

    //     raw_data.size = client_message.package.raw_data_size();
    //     raw_data.data = new char[raw_data.size + 1];
    //     memcpy(raw_data.data, client_message.package.raw_data(), raw_data.size);

    //     LSound::Sound_Data sound_data;
    //     sound_data.set_raw_data(raw_data);
    //     sound.set_sound_data(&sound_data);

    //     sound.play();

    //     while(sound.is_playing())
    //         std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // }

    while(true)
    {
        m_network_manager.process();
    }
}
