#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>
#include <random>
#include <chrono>

//constans
const float SNAKE_SIZE = 50.f;
const float WINDOW_HEIGHT = 1000.f;
const float WINDOW_WIDTH = 1500.f;

std::default_random_engine RNG(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<> DIST_X(0, 29);//one less to keep on screen
std::uniform_int_distribution<> DIST_Y(0, 19);

enum Entity {WALL, FRUIT, BODY, NONE};

class Snake{
    
public:
    std::list<sf::RectangleShape> body;
    sf::Vector2f snakeVector;
    void move(); //moves the tail to the front
    void addTail(); // add a tail if apple picked
    void draw(sf::RenderWindow &window); // draw the snake
    Entity collisionHead(sf::RectangleShape &fruit); // return a collisioned position and then highlight the area
    bool collisionBody(sf::RectangleShape &fruit);
    //constructor
    Snake(){
        
        sf::RectangleShape tail(sf::Vector2f(SNAKE_SIZE, SNAKE_SIZE));
        tail.setPosition(WINDOW_WIDTH /2, WINDOW_HEIGHT/2);
        tail.setFillColor(sf::Color::Green);
        tail.setOutlineColor(sf::Color::Black);
        tail.setOutlineThickness(2.f);
        this->body.push_back(tail);
        
    }
    
};

inline void Snake::addTail(){
    
    sf::RectangleShape tail(sf::Vector2f(SNAKE_SIZE, SNAKE_SIZE));
    sf::Vector2f tailVector;
    std::list<sf::RectangleShape>::iterator it = this->body.end();
    //before last one
    --it;
    //checks where to put the last tail
    if(it->getPosition().y < this->body.back().getPosition().y){
        tailVector = sf::Vector2f(0.f, SNAKE_SIZE);
    }else if(it->getPosition().y > this->body.back().getPosition().y){
        tailVector = sf::Vector2f(0.f, -SNAKE_SIZE);
    }else if(it->getPosition().x < this->body.back().getPosition().x){
        tailVector = sf::Vector2f(-SNAKE_SIZE, 0.f);
    }else if(it->getPosition().x > this->body.back().getPosition().x){
        tailVector = sf::Vector2f(SNAKE_SIZE, 0.f);
    }
    
    tail.setPosition(this->body.back().getPosition() - tailVector);
    tail.setFillColor(sf::Color::Green);
    tail.setOutlineColor(sf::Color::Black);
    tail.setOutlineThickness(2.f);
    this->body.push_back(tail);
    
}

inline void Snake::move(){

    
    
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && this->snakeVector != sf::Vector2f(0.f, SNAKE_SIZE)){
            this->snakeVector = sf::Vector2f(0.f, -SNAKE_SIZE);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && this->snakeVector != sf::Vector2f(0.f, -SNAKE_SIZE)){
            this->snakeVector = sf::Vector2f(0.f, SNAKE_SIZE);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && this->snakeVector != sf::Vector2f(SNAKE_SIZE, 0.f)){
            this->snakeVector = sf::Vector2f(-SNAKE_SIZE, 0.f);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && this->snakeVector != sf::Vector2f(-SNAKE_SIZE, 0.f)){
            this->snakeVector = sf::Vector2f(SNAKE_SIZE, 0.f);
        }
    
    
    //insert the tail in the beginning and delete it
    this->body.back().setPosition(this->body.front().getPosition() + snakeVector);
    this->body.push_front(this->body.back());
    this->body.pop_back();
    
    
}

inline void Snake::draw(sf::RenderWindow &window){
    for(auto &part : this->body){
        
        window.draw(part);
    }
    //draw the head over everything to display collision after setting it's color white
    window.draw(this->body.front());
}


//can't intersect with fruit and body at the same time
inline Entity Snake::collisionHead(sf::RectangleShape &fruit){
    
    sf::Vector2f head = this->body.front().getPosition();
    
    if(fruit.getPosition() == head){
        return FRUIT;
    }
    else if(head.y < 0 || head.y >= WINDOW_HEIGHT || head.x < 0 || head.x >= WINDOW_WIDTH){
        //need to roll back
        return WALL;
    }
    else{
    
        std::list<sf::RectangleShape>::iterator it = this->body.begin();
        for(++it; it != this->body.end(); ++it){
            if(it->getPosition() == head){
                return BODY;
            }
        }
        
        return NONE;
    }
}

//returns true if body collisions with body
inline bool Snake::collisionBody(sf::RectangleShape &fruit){
    
    sf::Vector2f fruitPos = fruit.getPosition();
    for (auto &part : this->body){
        if(part.getPosition() == fruitPos) return true;
    }
    
    return false;
}

class Menu{
    
public:
    Menu(float width, float height){
        
        font.loadFromFile("arial.ttf");
        
        selectedItemIndex = 0;
        
        menu[0].setFont(font);
        menu[0].setFillColor(sf::Color::Red);
        menu[0].setString("PLAY");
        menu[0].setStyle(sf::Text::Bold);
        menu[0].setCharacterSize(150);
        menu[0].setPosition(width/6, height/3);
        
        menu[1].setFont(font);
        menu[1].setFillColor(sf::Color::White);
        menu[1].setString("EXIT");
        menu[1].setStyle(sf::Text::Bold);
        menu[1].setCharacterSize(150);
        menu[1].setPosition(width/1.6f, height/3);
        
        menu[2].setFont(font);
        menu[2].setFillColor(sf::Color::White);
        menu[2].setString("Your score : ");
        menu[2].setStyle(sf::Text::Bold);
        menu[2].setCharacterSize(100);
        menu[2].setPosition(width/3.9f, height/2.f);
        
        
        menu[3].setFont(font);
        menu[3].setFillColor(sf::Color::White);
        menu[3].setStyle(sf::Text::Bold);
        menu[3].setCharacterSize(100);
        menu[3].setPosition(width/1.6f, height/2.f);
        
    }
    
    void drawGameOver(sf::RenderWindow &window, int score){

        window.draw(menu[0]);
        window.draw(menu[1]);
        window.draw(menu[2]);
        menu[3].setString(std::to_string(score));
        window.draw(menu[3]);
        


    }
    
    void moveRight(){
        menu[0].setFillColor(sf::Color::White);
        menu[1].setFillColor(sf::Color::Red);
        selectedItemIndex = 1;
    }
    
    void moveLeft(){
        menu[0].setFillColor(sf::Color::Red);
        menu[1].setFillColor(sf::Color::White);
        selectedItemIndex = 0;
    }
    
    //data
    int selectedItemIndex;
    sf::Font font;
    sf::Text menu[4];
    
};

int main(){

    bool gameRunning = true;
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake", sf::Style::Close | sf::Style::Titlebar);
    
    window.setFramerateLimit(20);
    Snake mySnake;
    Menu menu(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    //spawn fruit
    sf::RectangleShape fruit(sf::Vector2f(SNAKE_SIZE, SNAKE_SIZE));
    fruit.setFillColor(sf::Color::Red);
    fruit.setPosition(DIST_X(RNG) * 50.f,DIST_Y(RNG) * 50.f);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                    
                case sf::Event::KeyReleased:
                    if(!gameRunning){
                        switch (event.key.code) {
                            case sf::Keyboard::Left:
                                menu.moveLeft();
                                break;
                                
                            case sf::Keyboard::Right:
                                menu.moveRight();
                                break;
                            case sf::Keyboard::Enter:
                                if(menu.selectedItemIndex == 0){
                                    
                                    gameRunning = true;
                                    //update speed
                                    mySnake.snakeVector = sf::Vector2f(0.f, 0.f);
                                    
                                    mySnake.body.clear();
                                    sf::RectangleShape tail(sf::Vector2f(SNAKE_SIZE, SNAKE_SIZE));
                                    tail.setPosition(WINDOW_WIDTH /2, WINDOW_HEIGHT/2);
                                    tail.setFillColor(sf::Color::Green);
                                    tail.setOutlineColor(sf::Color::Black);
                                    tail.setOutlineThickness(2.f);
                                    mySnake.body.push_back(tail);
                                    
                                    
                                }else window.close();
                                break;
                                
                            default:
                                break;
                        }
                        
                    }
                    break;
                    
                default:
                    break;
            }
        }
        
        if(gameRunning){
            mySnake.move();
        
            Entity collidedEntity = mySnake.collisionHead(fruit);
            
            if(collidedEntity == FRUIT){
                mySnake.addTail();
                fruit.setPosition(DIST_X(RNG) * 50.f, DIST_Y(RNG) * 50.f);
                
                while (mySnake.collisionBody(fruit))
                    fruit.setPosition(DIST_X(RNG) * 50.f, DIST_Y(RNG) * 50.f);
            }else if (collidedEntity == WALL){
                if(mySnake.body.size() != 1){
                    mySnake.body.pop_front();
                    mySnake.addTail();
                    mySnake.body.front().setFillColor(sf::Color::White);
                    gameRunning = false;
                }else{
                    mySnake.body.front().setPosition(mySnake.body.front().getPosition()-mySnake.snakeVector);
                    mySnake.body.front().setFillColor(sf::Color::White);
                    gameRunning = false;
                }
            }else if (collidedEntity == BODY){
                mySnake.body.front().setFillColor(sf::Color::White);
                gameRunning = false;
            }
            
        }
        
        window.clear();
        
        window.draw(fruit);
        mySnake.draw(window);
        if(!gameRunning) menu.drawGameOver(window, mySnake.body.size());
        
        window.display();
    }
    
    return 0;
}
