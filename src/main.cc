#include "Application.hh"

int main()
{
    GetApplication()->Initialize();
    GetApplication()->Poll();
    GetApplication()->Destroy();
}
