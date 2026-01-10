#include <Utility.h>

#include <chrono>

using namespace LVC;


long unsigned int LVC::current_timestamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count();
}
