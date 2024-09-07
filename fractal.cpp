#include <utility>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>

using namespace std;
using namespace sf;

const int height = 1080;
const int width = 1080;
const int length = 300;

vector<RectangleShape> rectangles;
mutex rectMutex;

void createRectangle(RenderWindow &window, pair<int, int> position, int l) {
	if (l > 6) {
		sleep(milliseconds(75));
		srand(time(0));
		Color color(rand() % 256, rand() % 256, rand() % 256);

		RectangleShape rect(Vector2f(l, l));
		rect.setPosition(position.first, position.second);
		rect.setOutlineColor(color);
		rect.setFillColor(Color::Transparent);
		rect.setOutlineThickness(2);
		
		{
			lock_guard<mutex> guard(rectMutex);
			rectangles.push_back(rect);
		}

		createRectangle(window, make_pair(position.first - l / 4, position.second - l / 4), l / 2);
		createRectangle(window, make_pair(position.first + l - l / 4, position.second - l / 4), l / 2);
		createRectangle(window, make_pair(position.first + l - l / 4, position.second + l - l / 4), l / 2);
		createRectangle(window, make_pair(position.first - l / 4, position.second + l - l / 4), l / 2);
	}
}

int main() {
	RenderWindow window(VideoMode(width, height), "Fractal");
	thread drawThread([&window]() {
		createRectangle(window, make_pair(width / 2 - length / 2, height / 2 - length / 2), length);
	});

	while (window.isOpen()) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) window.close();
		}
		{
			lock_guard<mutex> guard(rectMutex);
			for (auto& r : rectangles)
				window.draw(r);
		}
		window.display();
	}

	if (drawThread.joinable()) drawThread.join();
	return 0;
}
