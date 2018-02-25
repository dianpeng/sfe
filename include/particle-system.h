#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_
#include <memory>
#include <cstdint>

#include <SFML/Graphics.h>
#include <dinject/dinject.h>

#include "adt.h"


namespace sfe {
namespace detail {

struct Particle {
  Particle():
    position_x(),
    position_y(),
    velocity_x(),
    velocity_y(),
    gravity   (),
    radial_acc(),
    tangential_acc(),
    spin      (),
    spin_acc  (),
    size      (),
    size_delta(),
    color     (),
    delta_r   (),
    delta_g   (),
    delta_b   (),
    delta_a   (),
    life      (),
    age       (-1.0f),
    sprite    ()
  {}

  void Init( float pos_x , float pos_y ,
             float vel_x , float vel_y ,
             float grav,
             float racc,
             float tacc,
             float sp , float sacc,
             float sz , float del_sz,
             std::uint8_t col_r ,
             std::uint8_t col_g ,
             std::uint8_t col_b ,
             std::uint8_t col_a,
             float del_r , float del_g , float del_b , float del_a ,
             float l ) {
    position_x = pos_x; position_y = pos_y;
    velocity_x = vel_x; velocity_y = vel_y;
    gravity = grav;
    radial_acc = racc;
    tangential_acc = tacc;
    spin = sp; spin_acc = sacc;
    size = sz; size_delta = del_sz;
    color.r = col_r; color.g = col_g; color.b = col_b; color.a = col_a;
    delta_r = del_r; delta_g = del_g; delta_b = del_b; delta_a = del_a;
    life = l;
    age = 0.0f;
  }

  bool IsStart() const { return age >= 0.0f; }
  bool IsDead () const { return age < 0.0f;  }

  void Update( float delta );

 public:
  float position_x , position_y ;
  float velocity_x , velocity_y ;
  float gravity;
  float radial_acc;
  float tangential_acc;
  float spin;
  float spin_acc;
  float size;
  float size_delta;
  sf::Color color;
  float delta_r , delta_g , delta_b , delta_a;
  float life;
  float age;
  Quad sprite;
};

class ParticleEmitter;

} // namespace detail

class ParticleSystem {
 public:


 private:
  FloatRect frame_;
  std::uint32_t max_particles_;
  double full_life_;
  double direction_;
  double spread_;
  DoubleRange speed_;
  DoubleRange spawn_x_;
  DoubleRange spawn_y_;
  DoubleRange life_;
  DoubleRange gravity_;
  DoubleRange raidal_acc_;
  DoubleRange tangential_acc_;
  DoubleRange size_;
  DoubleRange size_delta_;
  DoubleRange spin_;
  DoubleRange spin_delta_;
  sf::Color   color_start_;
  sf::Color   color_end_;
  float       color_delta_;

  // field for tracking status of ParticleSystem
  std::unique_ptr<ParticleEmitter> emitter_;
  std::size_t emitted_particle_;
  std::size_t dead_particle_;
  bool dead_;
  bool need_spwan_;
  float age_;
  float rate_;
  float last_emit_time_;

  std::unique_ptr<RenderBatch> render_batch_;
};

} // namespace sfe

#endif // PARTICLE_SYSTEM_H_
