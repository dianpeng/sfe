#include "app.h"

#include <GL/gl.h>
#include <iostream>
#include <cstdio>
#include <SFML/System.h>

namespace sfe {

App::App( const std::string& title , std::uint32_t fps ):
  window_(),
  fps_   (fps),
  clear_color_() {
  {
    auto m = sf::VideoMode::getFullscreenModes();
    if(m.empty()) {
      std::cerr<<"cannot find compatible fullscreen modes"<<std::endl;
      std::abort();
    }
    window_.reset( new sf::RenderWindow(m.front() , title.c_str()) );
  }
}

App::App( const std::string& title , std::uint32_t fps , std::size_t width ,
                                                         std::size_t height ):
  window_(),
  fps_   (fps),
  clear_color_() {
  window_.reset( new sf::RenderWindow(sf::VideoMode(width,height), title.c_str()) );
}

bool App::HandleEvent( const sf::Event& event ) {
  if(event.type == sf::Event::Closed) {
    return true;
  }
  return false;
}

bool App::Start() {
  window_->setFramerateLimit(0);
  window_->setVerticalSyncEnabled(false);

  if(!HandleInit()) return false;
 
  {
    sf::Clock clock;
    float prev  = 1.0f / fps_; // guess the first frame's time to be 1.0f / fps_

    while(window_->isOpen()) {
      sf::Event event;

      // restart the timer here
      clock.restart();

      while(window_->pollEvent(event)) {
        if(HandleEvent(event)) {
          window_->close(); break;
        }
      }


      // call the callback function
      {
        window_->clear(clear_color_);

        HandleUpdate( prev , window_.get() );

        window_->display();
      }

      // get frame check point
      auto seconds = clock.getElapsedTime().asSeconds();
      prev = seconds;

      { // sleep if needed to maintain stable frame rate
        auto left   = (1.0f/fps_) - seconds;
        if(left >0) {
          sf::sleep(sf::seconds(left));
          prev = 1.0f / fps_;
        } else {
          prev = seconds;
        }
      }
    }
  } 

  HandleClose();
  return true;
}

} // namespace sfe


