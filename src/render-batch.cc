#include "render-batch.h"
#include "util.h"

namespace sfe {

DINJECT_CLASS(RenderBatch) {
  dinject::Class<RenderBatch>("graphics.RenderBatch")
    .AddString("BlendMode",&RenderBatch::SetBlendMode)
    .AddString("Texture"  ,&RenderBatch::SetTexture  )
    .AddString("Shader"   ,&RenderBatch::SetShader   );
}

void RenderBatch::SetBlendMode( const std::string& blend_mode ) {
  fatal_if(util::ParseBlendMode(blend_mode.c_str(),&blend_mode_),
      "cannot load blend mode with name %s",blend_mode.c_str());
}

void RenderBatch::SetTexture  ( const std::string& ) {}
void RenderBatch::SetShader   ( const std::string& ) {}

void RenderBatch::Enqueue( const sf::Vertex& v , const sf::Transform& trans ) {
  sf::Vertex temp(v);
  temp.position = trans.transformPoint(temp.position);
  varray_.append(temp);
}

void RenderBatch::Render( sf::RenderTarget* target ) {
  sf::RenderStates states;
  if(texture_) states.texture = texture_;
  if(shader_ ) states.shader  = shader_ ;
  target->draw(varray_,states);
  varray_.clear();
}

} // namespace sfe
