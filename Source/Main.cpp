#include "Application.hpp"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    try
    {
        Application application{};
        application.Init();
        application.Run();
    }
    catch (const std::exception &exception)
    {
        std::cerr << "Error: " << exception.what() << std::endl;
        return 1;
    }

    return 0;
}