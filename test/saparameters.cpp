
#include <iostream>
#include <sstream>

template <typename T>
T str2num(const char * str)
{
    std::stringstream ss(str);

    T num;
    if (ss >> num)
    {
        return num;
    }
    else
    {
        std::cerr << "<ERR>: Malformed arguement (" << str << ")" << std::endl;
        std::exit(EXIT_FAILURE);                                
    }
}

int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        std::cout << "<ERR>: Not enough arguements" << std::endl;
        
        std::exit(EXIT_FAILURE);
    }

    double temperature = str2num<double>(argv[1UL]), cooling = str2num<double>(argv[2UL]);

    std::cout
    << "Temperature: "    << temperature << std::endl
    << "Cooling Factor: " << cooling     << std::endl;

    std::size_t iterations = 0UL;
    do { iterations++; } while ((temperature *= (1.0 - cooling)) > 1.0);

    std::cout << "Iterations: "     << iterations  << std::endl;

    return EXIT_SUCCESS;
}
