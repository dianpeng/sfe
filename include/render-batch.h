#ifndef RENDER_BATCH_H_
#define RENDER_BATCH_H_

#include "misc.h"

#include <SFML/Graphics.hpp>

#include <dinject/dinject.h>
#include <cassert>
#include <cmath>

namespace sfe {

class Quad;

class RenderBatch {
 public:
  RenderBatch( sf::BlendMode bm , const sf::Texture* texture = NULL ,
                                  const sf::Shader*  shader  = NULL ,
                                  sf::PrimitiveType type = sf::TriangleStrip ):
    varray_    (type),
    blend_mode_(bm),
    texture_   (texture),
    shader_    (shader)
  {}

  RenderBatch():
    varray_    (sf::TriangleStrip),
    blend_mode_(),
    texture_   (),
    shader_    ()
  {}

  const sf::BlendMode& blend_mode() const { return blend_mode_; }
  const sf::Texture*   texture()    const { return texture_;    }
  const sf::Shader*    shader ()    const { return shader_;     }

  // Render all enqueued Quad object into the underlying render target
  void Render  ( sf::RenderTarget* );
 public:

  // Enqueue a quad render unit into the vertex array queue. The draw
  // will actually happen when user call Render function of RenderBatch
  // object
  void Enqueue ( const sf::Vertex& vert , const sf::Transform& trans );

 private:
  // DINJECT APIs
  void SetBlendMode( const std::string& );
  void SetTexture  ( const std::string& );
  void SetShader   ( const std::string& );

  DINJECT_FRIEND_REGISTRY(RenderBatch);

 private:
  sf::VertexArray    varray_;
  sf::BlendMode      blend_mode_;
  const sf::Texture* texture_;
  const sf::Shader*  shader_;
};

// A quad shape objects , or the normal sprite to be rendered. The name
// is purposely used as Quad to keep it different from sf::Sprite
class Quad : protected sf::Transformable {
  typedef sf::Transformable Base;
 public:
  inline Quad( RenderBatch* , const sf::IntRect& );

  void SetPosition( float x , float y ) { Base::setPosition(x,y); }
  void SetRotation( float rot )         { Base::setRotation(rot); }
  void SetScale   ( float x , float y ) { Base::setScale(x,y);    }
  void SetAnchor  ( float x , float y ) { Base::setOrigin(x,y);   }

  inline void GetPosition( float* x , float* y ) const;
  void GetRotation( float* rot ) const { *rot = Base::getRotation(); }
  inline void GetScale   ( float* x , float* y ) const;
  inline void GetAnchor  ( float* x , float* y ) const;

  // Set a certain vertex's color internally, if index left to be -1
  // then set all vertex with the input color
  inline void SetColor   ( const sf::Color& , int index = -1 );
  inline void GetColor   ( sf::Color*       , int index ) const;

  inline void SetTextureRect( const sf::IntRect& );
  const sf::IntRect& GetTextureRect() const { return texture_rect_; }

  // Get the corresponding render batch
  RenderBatch* batch() const { return batch_; }

  // Render this sprite into the underlying RenderBatch object
  inline void Render();

 private:
  RenderBatch* batch_;
  sf::IntRect  texture_rect_;
  sf::Vertex   vertex_[4];

  friend class RenderBatch;
  DISALLOW_COPY_AND_ASSIGN(Quad)
};

inline Quad::Quad( RenderBatch* batch , const sf::IntRect& texture_rect ):
  batch_(batch),
  texture_rect_(),
  vertex_()
{ SetTextureRect(texture_rect); }

inline void Quad::GetPosition( float* x , float* y ) const {
  auto r = Base::getPosition();
  *x = r.x; *y = r.y;
}

inline void Quad::GetScale   ( float* x , float* y ) const {
  auto r = Base::getScale();
  *x = r.x; *y = r.y;
}

inline void Quad::GetAnchor  ( float* x , float* y ) const {
  auto r = Base::getOrigin();
  *x = r.x; *y = r.y;
}

inline void Quad::SetColor( const sf::Color& col , int index ) {
  if(index <0) {
    vertex_[0].color = col;
    vertex_[1].color = col;
    vertex_[2].color = col;
    vertex_[3].color = col;
  } else {
    assert( index >= 0 && index < 4 );
    vertex_[index].color = col;
  }
}

inline void Quad::GetColor( sf::Color* col , int index ) const {
  assert(index >= 0 && index < 4);
  *col = vertex_[index].color;
}

inline void Quad::SetTextureRect( const sf::IntRect& rect ) {
  if(texture_rect_ != rect) {

    auto w = static_cast<float>(rect.width);
    auto h = static_cast<float>(rect.height);

    // update the vertex position
    vertex_[0].position = sf::Vector2f(0,0);
    vertex_[1].position = sf::Vector2f(0,h);
    vertex_[2].position = sf::Vector2f(w,0);
    vertex_[3].position = sf::Vector2f(w,h);

    // update the texture coordinate
    {
      auto left = static_cast<float>(rect.left);
      auto right= left + static_cast<float>(rect.width);
      auto top  = static_cast<float>(rect.top);
      auto bot  = top + static_cast<float>(rect.height);

      vertex_[0].texCoords = sf::Vector2f(left,top);
      vertex_[1].texCoords = sf::Vector2f(left,bot);
      vertex_[2].texCoords = sf::Vector2f(right,top);
      vertex_[3].texCoords = sf::Vector2f(right,bot);
    }
  }
}

inline void Quad::Render() {
  auto trans = Base::getTransform();
  batch_->Enqueue(vertex_[0],trans);
  batch_->Enqueue(vertex_[1],trans);
  batch_->Enqueue(vertex_[2],trans);
  batch_->Enqueue(vertex_[3],trans);
}

} // namespace sfe

#endif // RENDER_BATCH_H_
