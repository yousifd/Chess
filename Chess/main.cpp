//
// Chess using SFML
// V 1.0
// First Ever Graphics Based Game
//

//Chess and Checkers – Simply put a game of chess or checkers. Try to make it playable online and if you can use a graphical user interface that can also undo or redo a step as well as keep a history of moves for replay.

#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

//for Resource Locations
#include "ResourcePath.hpp"

constexpr int windowWidth{800}, windowHeight{800};
constexpr int countX{8}, countY{8};
constexpr float tileWidth{100}, tileHeight{100};

class Tile
{
public:
    Tile(float, float);
    
    sf::RectangleShape shape;
    static int i;
    
    float x() { return shape.getPosition().x; }
    float y() { return shape.getPosition().y; }
    float left() { return x() - 50; }
    float right() { return x() + 50; }
    float top() { return y() - 50; }
    float bottom() { return y() + 50; }
};

Tile::Tile(float mX, float mY)
{
    shape.setPosition(mX, mY);
    shape.setSize({tileWidth, tileHeight});
    shape.setOrigin({tileWidth / 2, tileHeight / 2});
    
    i++;
    
    //different coloring based on number in sequence
    if (i % 2 == 0) shape.setFillColor(sf::Color(245, 245, 220));
    else shape.setFillColor(sf::Color(139, 69, 19));
}

class Piece
{
public:
    Piece(float, float, std::string);
    
    float preX, preY;
    bool dead = false, check = false;
    
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Event event;
    
    void update(sf::Window&, std::vector<Tile>&);

    virtual bool isLegal(Tile&) { return false; };
    
    float x() { return sprite.getPosition().x; }
    float y() { return sprite.getPosition().y; }
    float left() { return x() - 50; }
    float right() { return x() + 50; }
    float top() { return y() - 50; }
    float bottom() { return y() + 50; }
};

Piece::Piece(float mX, float mY, std::string image)
{
    texture.loadFromFile(image);
    
    sprite.setTexture(texture);
    sprite.setColor(sf::Color::Green);
    sprite.setPosition(mX, mY);
    sprite.setOrigin(tileWidth / 2, tileHeight / 2);
    //size should be default to 100
    
    preX = mX;
    preY = mY;
}

void Piece::update(sf::Window& window, std::vector<Tile>& tiles)
{
    //FUTURE: if mouse on piece show possible move locations
    
    check = false;
    
    //somehow this allows the user to move the piece with mouse
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
       && sf::Mouse::getPosition(window).x > left()
       && sf::Mouse::getPosition(window).x < right()
       && sf::Mouse::getPosition(window).y > top()
       && sf::Mouse::getPosition(window).y < bottom())
    {
        //other click
        if(window.waitEvent(event)) check = true;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && check)
            sprite.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        //Position of Piece is always in center of tile FUTURE: excpet when between four tiles
        for(auto& tile : tiles)
        {
            if(!(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                && sf::Mouse::getPosition(window).x > tile.left()
                && sf::Mouse::getPosition(window).x < tile.right()
                && sf::Mouse::getPosition(window).y > tile.top()
                && sf::Mouse::getPosition(window).y < tile.bottom())
            {
                //FUTURE: check if legal
                if(!(isLegal(tile)))
                {
                    std::cout << "Enter isLegal() if Statment" << std::endl;
                    std::cout << "Before preX: " << preX << std::endl;
                    std::cout << "Before preY: " << preY << std::endl;
                    sprite.setPosition(preX, preY);
                    std::cout << "After preX: " << preX << std::endl;
                    std::cout << "After preY: " << preY << std::endl;
                }
                else { preX = tile.x(); preY = tile.y(); }
                
                    sprite.setPosition(tile.x(), tile.y());
            }
            
            //FUTURE: make way to avoid bugs of invalid locations
            //else sprite.setPosition(preX, preY);
            //if(sprite.getPosition().x != tile.x() && sprite.getPosition().y != tile.y())
            //  sprite.setPosition(preX, preY);
        }
    }
}

//FUTURE: Create all Texture Images

//FUTURE: create all other peices

class Pawn: public Piece
{
public:
    Pawn(float, float, int, int, int);
    
    bool isLegal(Tile&);
};

Pawn::Pawn(float mX, float mY, int r, int g, int b) : Piece( mX, mY,(resourcePath() + "Pawn.png"))
{
    //set to color based on user either black or white
    sprite.setColor(sf::Color(r, g, b));
}

bool Pawn::isLegal(Tile& tile)
{
    //if illegal return false
    //else it is true
    if((this->y() != (tile.y() + 100)) && (this->x() == tile.x()))
    {
        std::cout << "isLegal() output: false" << std::endl;
        std::cout << "this->x() = " << this->x() << std::endl;
        std::cout << "this->y() = " << this->y() << std::endl;
        std::cout << "sprite.getPosition().x = " << sprite.getPosition().x << std::endl;
        std::cout << "sprite.getPosition().y = " << sprite.getPosition().y << std::endl;
        std::cout << "tile.x() = " << tile.x() << std::endl;
        std::cout << "tile.y() = " << tile.y() << std::endl;
        return false;
    }
    else
    {
        std::cout << "isLegal() output: true" << std::endl;
        std::cout << "this->x() = " << this->x() << std::endl;
        std::cout << "this->y() = " << this->y() << std::endl;
        std::cout << "sprite.getPosition().x = " << sprite.getPosition().x << std::endl;
        std::cout << "sprite.getPosition().y = " << sprite.getPosition().y << std::endl;
        std::cout << "tile.x() = " << tile.x() << std::endl;
        std::cout << "tile.y() = " << tile.y() << std::endl;
        return true;
    }
}

//FUTURE: create entity as user

int Tile::i = 0;

int main(int, char const**)
{
    sf::RenderWindow window({windowWidth, windowHeight} , "Chess");
    window.setFramerateLimit(60);
    
    //set initial pieces locations and create them using loop for a vector of pieces
    Pawn pawn(50, 138, 255, 255, 255);
    
    std::vector<Tile> tiles;
    
    for(int iX = 0; iX < countX; iX++)
    {
        for(int iY = 0; iY < countY; iY++)
            tiles.emplace_back((iX) * (tileWidth) + 50, (iY) * (tileHeight) + 38);
        
        //used to set the alternate coloring of the tiles
        //if column is even start from 1 + 1 = 2
        //else column is odd, start from 0 + 1 = 1
        if(iX % 2 == 0) Tile::i = 1;
        else Tile::i = 0;
    }
    
    while(true)
    {
        sf::Event event;
        window.pollEvent(event);
        
        window.clear();
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || event.type == sf::Event::Closed) break;
        
        pawn.update(window, tiles);
        
        for(auto& tile : tiles) window.draw(tile.shape);
        window.draw(pawn.sprite);
        window.display();
    }
    
    return 0;
}
