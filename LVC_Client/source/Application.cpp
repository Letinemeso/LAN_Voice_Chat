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
    LSound::Sound_Engine::instance().set_listener_volume_multiplier(30.0f);

    Recorder recorder;
    recorder.inject_network_manager(&m_network_manager);

    Player player;

    m_network_manager.inject_player(&player);
    // m_network_manager.set_server_ip("127.0.0.1");
    m_network_manager.set_server_ip("192.168.1.105");

    while(true)
    {
        recorder.process();
        m_network_manager.process();
        player.process();
    }
}
