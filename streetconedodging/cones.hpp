#ifndef CONES_HPP_
#define CONES_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Cones{
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();
  void update(const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  glm::vec4 m_color{1};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  bool firstCone = true;
  float m_scale{0.15f};

  struct Cone {
    bool foraDaTela{};   
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{0.0f, 0.8f};
  };
  
  std::list<Cone> m_cones;

  std::default_random_engine m_randomEngine;
  abcg::ElapsedTimer m_coneTimer;

  std::uniform_real_distribution<float> m_randomConesQtt{0.0f, 2.0f};
  std::uniform_real_distribution<float> m_randomConesLoc{0.0f, 3.0f};
};
#endif