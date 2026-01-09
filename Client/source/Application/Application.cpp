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

    std::cout << "raw data size = "  << sound_data->raw_data().size << " bytes" << std::endl;

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
    constexpr unsigned int Max_Sound_Data_Size = 32000;
    constexpr unsigned int Duration_Milliseconds = 500;

    LSound::Input_Device_Settings device_settings = LSound::Input_Device_Settings::voip_standard((float)Duration_Milliseconds * 0.001f);
    LSound::Input_Device input_device(device_settings);

    LNet::Client_Socket client_socket(Max_Sound_Data_Size);

    std::string pause_crutch;
    std::cin >> pause_crutch;

    // client_socket.connect("127.0.0.1", 12345);
    client_socket.connect("192.168.1.103", 12345);

    unsigned int iterations_expected = 0;

    while(true)
    {
        ++iterations_expected;
        if(iterations_expected > 1000)
            break;

        std::cout << "\nstarting capture" << std::endl;

        input_device.start_capture();
        std::this_thread::sleep_for(std::chrono::milliseconds(Duration_Milliseconds));
        input_device.stop_capture();

        LSound::Sound_Data* sound_data = input_device.extract_capture();
        L_ASSERT(sound_data);

        std::cout << "raw data size = "  << sound_data->raw_data().size << " bytes" << std::endl;

        std::string raw_sound_data(sound_data->raw_data().size + 1, 0);

        raw_sound_data.reserve(sound_data->raw_data().size + 1);
        for(unsigned int i = 0; i < sound_data->raw_data().size; ++i)
            raw_sound_data[i] = sound_data->raw_data().data[i];
        raw_sound_data[sound_data->raw_data().size] = 0;

        bool sent = client_socket.send(raw_sound_data);

        delete sound_data;

        if(sent)
            continue;

        std::cout << "error sending package" << std::endl;
    }
}
