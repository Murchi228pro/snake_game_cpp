#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <deque>

struct Circle {
    float x, y;
    Circle(float x, float y){
        this->x = x;
        this->y = y;
    }
};

int main()
{
    srand(time(NULL));

    std::deque<Circle> circles;
    float PlayerDegress = 0.f;
    float PlayerX = 20.f;
    float PlayerY = 20.f;
    float PlayerSize = 7.f;
    float Colizecoof = PlayerSize + PlayerSize / 1.4 - 2;
    Circle appleCoords(20 + rand() % 780, 20 + rand() % 380);
    for (int i=0; i < 20; i++) {
        circles.push_front(Circle(PlayerX, PlayerY));
    }
    short PlayerSpeed = 5;
    sf::RenderWindow window(sf::VideoMode(800, 400), "SnakeGame");
    sf::CircleShape shape(PlayerSize);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::CircleShape apple(PlayerSize/1.3);
        apple.setPosition(appleCoords.x, appleCoords.y);
        apple.setFillColor(sf::Color::Red);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) PlayerDegress += -0.08;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) PlayerDegress += 0.08;

        
        if (PlayerX > 800 || PlayerX < 0) PlayerDegress -= 3.141592;
        if (PlayerY < 0 || PlayerY > 400) PlayerDegress = - PlayerDegress;

        if (PlayerDegress > 2 * 3.141592) PlayerDegress -= 2 * 3.141592;
        if (PlayerDegress < 0) PlayerDegress = 2 * 3.141592 + PlayerDegress;

        if (fabs(PlayerX - appleCoords.x) < Colizecoof && fabs(PlayerY - appleCoords.y) < Colizecoof)
        {
            appleCoords.x = 20 + rand() % 780;
            appleCoords.y = 20 + rand() % 380;
            circles.push_back(Circle(0, 0));
        }
        circles.push_front(Circle(PlayerX, PlayerY));
        circles.pop_back();

        window.clear();
        for (int i = 0; i < circles.size(); ++i){
            auto circle = sf::CircleShape(PlayerSize);
            circle.setPosition(circles[i].x, circles[i].y);
            window.draw(circle);
        }
        PlayerX += PlayerSpeed * cos(PlayerDegress);
        PlayerY += PlayerSpeed * sin(PlayerDegress);

        shape.setPosition(PlayerX, PlayerY);
        window.draw(shape);
        window.draw(apple);
        window.display();
        Sleep(20);
    }
    return 0;
}