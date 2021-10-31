#ifndef CARRO_HPP_
#define CARRO_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Carro {
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

  float m_scale{0.1f};
  glm::vec4 m_color{1};
  glm::vec2 m_translation{glm::vec2(0)};
};

#endif
