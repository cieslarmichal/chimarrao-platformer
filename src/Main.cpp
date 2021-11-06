#include "GameFactory.h"
//#include "Windows.h"

int main()
{
    try
    {
        auto game = game::GameFactory::createGame();
        game->run();
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what();
    }

    return 0;
}

//#ifdef _WIN32
// int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//{
//    return main();
//}
//#endif