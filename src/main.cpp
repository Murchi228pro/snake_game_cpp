#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Window.hpp"
#include <SFML/Graphics.hpp>
#include <deque>
#include <cmath>
#include <thread>



/**
 * данный блок отвечает за установку основных констант используемых
 * в игре.
 * Обрати внимание на constexpr - это модификатор переменный
 * отвечающей за ее константость в одной единице трансляции
 * на этапе компиляции этой единицы трансляции.
 * По-хорошему, они выносятся в отдельное пространство имен.
**/

constexpr double CONSTANT_PI = 3.14159265358979323846;
constexpr float PLAYER_SIZE = 7.f;
constexpr float APPLE_SIZE = PLAYER_SIZE/1.3;

constexpr int WINDOW_HEIGHT = 400;
constexpr int WINDOW_WIDTH = 800;

constexpr float BASE_START_COORDINATE = 20.f;

constexpr int WINDOW_WIDTH_FOR_RANDOM = static_cast<int>(WINDOW_WIDTH - BASE_START_COORDINATE);
constexpr int WINDOW_HEIGHT_FOR_RANDOM = static_cast<int>(WINDOW_HEIGHT - BASE_START_COORDINATE);


/**
 * Сделал две новые сущности Player и Apple, чтобы
 * было легче управлять игрой.
 * У каждой сущности есть свои функции, что позволяет 
 * в более лекгой форме выделять, обновлять 
 * изменять поведение объекта и проводить тестирование 
 * только определеных функций.
 * По-хорошему, тебе бы изучить ООП в c++.
 * Найти общие черты у этих двух сущностей и переписать,
 * данный код этих сущностей с применением ООП.
**/

//тут почти без изменений, разве что добавил конструктор по-умолчанию
struct Circle {
    float x_, y_;

    Circle(float x, float y) : x_(x), y_(y) { }
    Circle() = default;
};

//выделил игрока в отдельную сущность
struct Player {
    Player(float x, float y, sf::RenderWindow& window) : coords_(x,y), window_(window) {
        playerHead_ = sf::CircleShape(size_);
        playerHead_.setFillColor(sf::Color::Green);
        for (int i=0; i < 20; i++) {
            tail.push_front(coords_);
        }
    } 

    void UpdateXY() {
        coords_.x_ += speed_ * cos(degrees_);
        coords_.y_ += speed_ * sin(degrees_);
    }

    void UpdateDegrees() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) degrees_ -= degreeStep_;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) degrees_ += degreeStep_;

        if (coords_.x_ > WINDOW_WIDTH || coords_.x_ < 0) degrees_ -= CONSTANT_PI;
        if (coords_.y_ < 0 || coords_.y_ > WINDOW_HEIGHT) degrees_ = -degrees_;

        if (degrees_ > 2 * CONSTANT_PI) degrees_ -= 2 * CONSTANT_PI;
        if (degrees_ < 0) degrees_ = 2 * CONSTANT_PI + degrees_;
    }

    void Draw() {
        DrawTail();
        DrawHead();
    }

    void DrawHead() {
        playerHead_.setPosition(coords_.x_, coords_.y_);
        window_.draw(playerHead_);
    }

    void DrawTail(){
        for(const auto & tailCircle : tail) {
            auto circle = sf::CircleShape(size_);
            circle.setPosition(tailCircle.x_, tailCircle.y_);
            window_.draw(circle);
        }
    }

    void UpdateTail() {
        tail.push_front(coords_);
        tail.pop_back();
    }

    void AppendTail() {
        tail.push_back(Circle(0, 0));
    }


    bool CheckCollision(const Circle& apple) {
        float Colizecoof = size_ + (size_ / 1.4) - 2;
        if(std::fabs(coords_.x_ - apple.x_) < Colizecoof && std::fabs(coords_.y_ - apple.y_) < Colizecoof)
            return true;
        return false;
    }

private:
    const int speed_ = 5;
    const float& size_ = PLAYER_SIZE;
    float degrees_ = 0.f;
    sf::CircleShape playerHead_;
    Circle coords_;
    std::deque<Circle> tail;
    sf::RenderWindow& window_;
    const float degreeStep_ = 0.08f;
};


//выделил яблоко в отдельную сущность
struct Apple {
    Apple(sf::RenderWindow& window) : window_(window) {
        coords_ = Circle(static_cast<float>(BASE_START_COORDINATE + rand() % WINDOW_WIDTH_FOR_RANDOM), 
                            static_cast<float>(BASE_START_COORDINATE + rand() % WINDOW_HEIGHT_FOR_RANDOM));
        shape_ = sf::CircleShape(size_);
        shape_.setPosition(coords_.x_, coords_.y_);
        shape_.setFillColor(sf::Color::Red);
    }

    void UpdateCoords() {
        coords_.x_ = static_cast<float>(BASE_START_COORDINATE + rand() % WINDOW_WIDTH_FOR_RANDOM);
        coords_.y_ = static_cast<float>(BASE_START_COORDINATE + rand() % WINDOW_HEIGHT_FOR_RANDOM);
        shape_.setPosition(coords_.x_, coords_.y_);
    }

    void Draw() {
        window_.draw(shape_);
    }

    Circle Coords_() {
        return coords_;
    }

private:
    Circle coords_;
    sf::CircleShape shape_;
    const float& size_ = APPLE_SIZE;
    sf::RenderWindow& window_;
};

int main() {
    /**
     * впринципе srand(), не так уж плох,
     * но тебе лучше посмотреть в сторону библиотеки <random> из
     * стандартной библиотеки шаблонов c++
    **/
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, 
                            WINDOW_HEIGHT), "SnakeGame");


    Player player(BASE_START_COORDINATE, BASE_START_COORDINATE, window);
    Apple apple(window);

    /**
     * облегчили основный цикл за счет выделения сущностей,
     * что делает его более легким для восприятия
    **/
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.UpdateDegrees();

        if (player.CheckCollision(apple.Coords_())) {
            apple.UpdateCoords();
            player.AppendTail();
        }

        player.UpdateTail();

        window.clear();

        player.UpdateXY();


        player.Draw();
        apple.Draw();

        window.display();

        /**
         * вынес функцию Sleep, и заменил ее на аналогичную
         * функцию из stl c++, в отдельный блок кода, чтобы 
         * исключитель применения оператора using на весь
         * последующий код, что ты мог написать/напишешь после
         * функции sleep
         */
        {
            using std::chrono_literals::operator""ms;
            std::this_thread::sleep_for(20ms);
        }
    }
    return 0;
}