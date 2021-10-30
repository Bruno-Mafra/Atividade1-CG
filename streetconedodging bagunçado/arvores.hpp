#ifndef ARVORES_HPP_
#define ARVORES_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Arvores{
 public:
  void initializeGL(GLuint program, int quantity);
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

  struct Arvore {
    GLuint m_vao{};
    GLuint m_vbo{};
    GLuint m_ebo{};

    float m_scale{0.13f};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{0.0f, 0.8f};
  };

  Arvores::Arvore createArvore();

  std::list<Arvore> m_arvores;

  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
  std::default_random_engine m_randomEngine;
};

#endif