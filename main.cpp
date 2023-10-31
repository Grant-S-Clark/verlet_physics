#include "Common.h"
#include "Simulation.h"
#include "Timer.h"

int main()
{
    std::srand((unsigned int)time(NULL));
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Physics",
                            sf::Style::Close);
    window.setFramerateLimit(60);

    bool in_focus = true;

    Simulation::set_window(&window);
    Simulation sim({0, 1000.0}, 8);
    double time = 0;
    sf::Font f;
    f.loadFromFile("simply_mono_2/SimplyMono-Book.ttf");
    sf::Text timer_text;
    timer_text.setFont(f);
    timer_text.setCharacterSize(20);
    timer_text.setFillColor(sf::Color::Black);
    Timer timer;
    
    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed ||
                (in_focus && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
            {
                window.close();
            }

            //Adjust window focus if changed.
            if (in_focus && e.type == sf::Event::LostFocus)
                in_focus = false;
            else if (!in_focus && e.type == sf::Event::GainedFocus)
                in_focus = true;

            // F key to slow framerate
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
                window.setFramerateLimit(2);
            else
                window.setFramerateLimit(60);
        }

        // Make objects with space key
        if (true)
            sim.create_obj();
        
        window.clear(sf::Color::White);
        
        sim.draw();
        timer.start();
        sim.update_objects(1.0/60.0);
        timer.stop();
        timer_text.setString(std::to_string(time));
        time = timer.read();
        window.draw(timer_text);
        
        window.display();
    }
    
    return 0;
}
