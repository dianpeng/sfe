#ifndef APP_H_
#define APP_H_

#include <memory>
#include <string>
#include <cstdint>

#include <SFML/Graphics.hpp>

namespace sfe {

class App {
 public:
  // Initialize the application with fullscreen mode , if we failed to set
  // to fullscreen mode , the application will abort
  App( const std::string& title , std::uint32_t fps );

  // Initialize the application with windowded mode , if we failed to set to
  // windowded mode, the application will abort
  App( const std::string& title , std::uint32_t fps , const std::size_t width ,
                                                      const std::size_t height );

 public:
  // Get the underlying sfml RenderWindow object
  sf::RenderWindow* window() const { return window_.get(); }

  const sf::Color& clear_color() const { return clear_color_; }

  void set_clear_color( const sf::Color& col ) { clear_color_ = col; }
 public:

  // Called before the enter the loop
  virtual bool HandleInit  () = 0;

  // The function that is called every frame , user needs to overwrite it to
  // insert the rendering code and logical/AI code
  //
  // This function will only be called when all the event is handlede , or
  // pollEvent returns false
  virtual void HandleUpdate( float delta , sf::RenderWindow* ) = 0;

  // Called when there's event needs to be handled , if this function returns
  // true , then the application will exit the loop and closed ; otherwise it
  // will continue running.
  //
  // The default implementation check the close event and return true when needed
  virtual bool HandleEvent( const sf::Event& );

  // Called when we exit from the loop due to user issue a close
  virtual void HandleClose() = 0;

  // Call this function to enter the loop
  bool Start();

  virtual ~App() {}
 private:
  std::unique_ptr<sf::RenderWindow> window_;
  std::uint32_t fps_;
  sf::Color clear_color_;
};

} // namespace sfe

#endif // APP_H_
