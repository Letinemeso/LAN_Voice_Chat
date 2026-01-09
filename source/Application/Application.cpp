#include <Application/Application.h>

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



void Application::M_test_sound()
{
    constexpr float Duration = 2.0f;

    LSound::Input_Device_Settings device_settings = LSound::Input_Device_Settings::voip_standard(Duration);
    // device_settings.raw_volume_multiplier = 2.0f;

    LSound::Input_Device input_device(device_settings);

    std::string pause_crutch;
    std::cin >> pause_crutch;

    std::cout << "\nstarting capture" << std::endl;

    input_device.start_capture();
    std::this_thread::sleep_for(std::chrono::seconds((unsigned int)Duration));
    input_device.stop_capture();

    LSound::Sound_Data* sound_data = input_device.extract_capture();
    L_ASSERT(sound_data);

    std::cout << "\nplaying capture" << std::endl;


    LSound::Sound sound;
    sound.set_sound_data(sound_data);

    LSound::Sound::Settings settings;
    // settings.volume = 2.0f;
    sound.set_settings(settings);
    sound.play();

    std::this_thread::sleep_for(std::chrono::seconds((unsigned int)Duration));

    LSound::Sound_Engine::instance().set_listener_volume_multiplier(50.0f);
    // settings.volume = 2.0f;
    sound.set_settings(settings);
    sound.play();

    std::this_thread::sleep_for(std::chrono::seconds((unsigned int)Duration));

    delete sound_data;
}

void Application::M_test_network()
{
    LNet::Server_Socket server_socket(12345, 1024);
    LNet::Client_Socket client_socket(1024);

    client_socket.connect("127.0.0.1", 12345);

    client_socket.send("ass ass ass bass!");

    LNet::Server_Socket::Message client_message = server_socket.receive();
    std::cout << "client sent: " << client_message.message << std::endl;

    server_socket.send("ass to you!", client_message.client_address);

    std::string server_response = client_socket.receive();
    std::cout << "server responded: " << server_response << std::endl;
}



void Application::run()
{
    M_test_network();
}
