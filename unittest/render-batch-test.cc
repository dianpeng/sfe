#include "app.h"
#include "render-batch.h"

#include <iostream>

namespace sfe {

class TestApp : public App {
 public:
  TestApp(): App( "test" , 30, 800, 600 ) , texture_ () , batch_ () , sprites_ () {}
  virtual bool HandleInit() {
    texture_.reset( new sf::Texture() );
    if(!texture_->loadFromFile("my_image.png")) {
      std::cerr<<"failed to load my_image.png"<<std::endl;
      return false;
    }

    batch_.reset(new RenderBatch(
          sf::BlendMode(sf::BlendMode::SrcColor,sf::BlendMode::DstColor)));

#if 0
    r_.resize(10000);
    for( auto &e : r_ ) {
      e.reset( new sf::RectangleShape(sf::Vector2f(128,128)) );
      e->setFillColor(sf::Color(255,123,2,255));
    }
#endif

    sprites_.resize(20);
    for( auto &e : sprites_ ) {
      e.reset( new Quad(batch_.get(),sf::IntRect(0,0,128,128)) );
      e->SetColor(sf::Color(255,123,2,255));
    }
    set_clear_color(sf::Color::White);
    return true;
  }

  virtual void HandleUpdate( float delta , sf::RenderWindow* window ) {
    std::cout<<"TIME:"<<1.0f/delta<<"\n";
#if 0
    for( auto &e : r_ ) {
      e->setPosition(500,400);
      window->draw(*e);
    }
#endif

    for( std::size_t i = 0 ; i  < 100 ; ++i ) {
      {
        for( auto &e : sprites_ ) {
          e->SetPosition(500,400);
          e->Render();
        }
      }

      {
        batch_->Render(window);
      }
    }
  }

  virtual void HandleClose() {}
 private:
  std::unique_ptr<sf::Texture> texture_;
  std::unique_ptr<RenderBatch> batch_;
  std::vector<std::unique_ptr<Quad>> sprites_;
  std::vector<std::unique_ptr<sf::RectangleShape>> r_;
};

} // namespace sfe

int main() {
  sfe::TestApp app;
  app.Start();
  return 0;
}
