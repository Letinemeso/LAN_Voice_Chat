#include <Application.h>

#include <Stuff/File.h>

#include <MDL_Reader.h>

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
#include <Settings.h>

using namespace LVC;


Application::Application()
{
    LV::Type_Manager::register_basic_types();

    LSound::Sound_Engine::instance();
    LNet::Net_Engine::instance();

    L_CREATE_LOG_LEVEL(LNet::Net_Engine::instance().log_level());
}

Application::~Application()
{

}



void Application::run()
{
    Settings settings;

    if(LST::File("Settings.mdl").exists())
    {
        LV::MDL_Reader reader;
        reader.parse_file("Settings");
        settings.assign_values(reader.get_stub("Settings"));
    }

    LSound::Sound_Engine::instance().set_listener_volume_multiplier(settings.volume);

    Recorder recorder;
    recorder.inject_network_manager(&m_network_manager);

    Player player;

    m_network_manager.inject_player(&player);
    m_network_manager.set_server_ip(settings.server_ip);

    while(true)
    {
        recorder.process();
        m_network_manager.process();
        player.process();
    }
}
