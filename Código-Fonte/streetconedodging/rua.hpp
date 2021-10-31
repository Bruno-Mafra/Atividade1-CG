#ifndef RUA_HPP_
#define RUA_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Rua{
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

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
  glm::vec2 m_translation{glm::vec2(0)};
  glm::vec4 m_color{1};
};
#endif