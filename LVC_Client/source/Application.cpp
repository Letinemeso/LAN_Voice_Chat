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
#include <Package_Header.h>
#include <Recorder.h>
#include <Voice_Package_Queue.h>
#include <Player.h>

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
    /*constexpr unsigned int Duration_Milliseconds = 500;

    LSound::Input_Device_Settings device_settings = LSound::Input_Device_Settings::voip_standard((float)Duration_Milliseconds * 0.001f);
    LSound::Input_Device input_device(device_settings);

    LNet::Client_Socket client_socket(Max_Message_Size);

    std::string pause_crutch;
    std::cin >> pause_crutch;

    client_socket.connect("127.0.0.1", Port);

    LNet::Package handshake_package;
    Package_Header handshake_package_header;
    handshake_package_header.command_type = Command_Type::Handshake;
    handshake_package.append_header(handshake_package_header);

    bool handshake_success = false;
    for(unsigned int i = 1; i <= 3; ++i)
    {
        client_socket.send(handshake_package);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        LNet::Package response = client_socket.receive();
        if(response.empty())
            continue;

        Package_Header response_header = response.parse_header<Package_Header>();
        if(response_header.command_type != Command_Type::Handshake)
            continue;

        handshake_success = true;
        break;
    }

    if(!handshake_success)
    {
        std::cout << "unable to connect to the server" << std::endl;

        std::cin >> pause_crutch;
        std::cout << pause_crutch << std::endl;

        return;
    }

    std::cout << "successfully connected to the server" << std::endl;


    while(true)
    {
        input_device.start_capture();
        std::this_thread::sleep_for(std::chrono::milliseconds(Duration_Milliseconds));
        input_device.stop_capture();

        LSound::Sound_Data* sound_data = input_device.extract_capture();
        L_ASSERT(sound_data);

        LNet::Package package;

        Package_Header header;
        header.command_type = Command_Type::Sound_Data;

        package.append_header(header);
        package.append_data(sound_data->raw_data().data, sound_data->raw_data().size);

        bool sent = client_socket.send(package);

        delete sound_data;

        if(sent)
            continue;

        std::cout << "error sending package" << std::endl;
    } */

    Recorder recorder;
    recorder.inject_network_manager(&m_network_manager);

    Voice_Package_Queue packages_queue;

    Player player;
    player.inject_packages_queue(&packages_queue);

    m_network_manager.inject_packages_queue(&packages_queue);
    m_network_manager.set_server_ip("127.0.0.1");

    while(true)
    {
        recorder.process();
        m_network_manager.process();
        player.process();
    }
}
