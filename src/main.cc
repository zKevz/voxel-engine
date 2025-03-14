#include "Application.hh"

int main()
{
    srand(time(nullptr));
    GetApplication()->Initialize();
    GetApplication()->Poll();
    GetApplication()->Destroy();
}
