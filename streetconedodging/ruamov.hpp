#ifndef RUAMOV_HPP_
#define RUAMOV_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Ruamov{
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

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  float m_scale{1.0f};
  glm::vec4 m_color{1};
  glm::vec2 m_velocity{0.0f, 0.8f};
  glm::vec2 m_translation{glm::vec2(0)};

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
};
#endif