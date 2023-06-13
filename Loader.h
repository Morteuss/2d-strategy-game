#ifndef LOADER_H
#define LOADER_H

#include <atomic>

namespace sf
{
	class RenderWindow;
}

class Loader
{
public:
	static void start(sf::RenderWindow *window, std::atomic<bool> & isLoaded);
};

#endif // LOADER_H
