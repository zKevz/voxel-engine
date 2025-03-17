#include "Application.hh"
#include "Utils/Logger.hh"

int main()
{
    srand(time(nullptr));
    LogInfo("made by kepin ya..");
    GetApplication()->Initialize();
    GetApplication()->Poll();
    GetApplication()->Destroy();
}
