#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_
#include <SFML/Graphics.hpp>
#include <string>

namespace sfe {

/**
 * A simple resource manager wrapper , user should use it manage all the
 * in memory resources
 */
class ResourceManager {
 public:
  ResourceManager( const std::string& path );

  // load a specific level's data into the memory
  bool LoadLevel ( const std::string& name );

 public:
  sf::Texture* GetTexture( const std::string& ) const;
  sf::Shader * GetShader ( const std::string& ) const;

  // dump the loaded resource
  void Dump( std::ostream* ) const;
 private:
  std::map<std::string,std::unique_ptr<sf::Texture>> texture_;
  std::map<std::string,std::unique_ptr<sf::Shader>>  shader_;
};

} // namespace sfe

#endif // RESOURCE_MANAGER_H_
