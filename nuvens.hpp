#ifndef NUVENS_HPP_
#define NUVENS_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Nuvens{
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

  struct Nuvem {
    GLuint m_vao{};
    GLuint m_vbo{};
    GLuint m_ebo{};

    float m_scale{0.2f};
    glm::vec2 m_velocity{0.05f, 0.8f};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec4 m_color{0.9608f, 0.9608f, 0.9608f, 0.4f};
  };

  std::list<Nuvem> m_nuvens;

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

  Nuvens::Nuvem createNuvem();
};
#endif
