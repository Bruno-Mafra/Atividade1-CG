#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "carro.hpp"
#include "nuvens.hpp"
#include "rua.hpp"
#include "ruamov.hpp"
#include "cones.hpp"
#include "arvores.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;
  Carro m_carro;
  Nuvens m_nuvens;
  Rua m_rua;
  Ruamov m_ruamov;
  Cones m_cones;
  Arvores m_arvores;

  abcg::ElapsedTimer m_restartWaitTimer;
  abcg::ElapsedTimer m_segundosTimer;
  abcg::ElapsedTimer m_pontosTimer;

  ImFont* m_font{};
  ImFont* m_font2{};

  std::default_random_engine m_randomEngine;

  void restart();
  void update();
  void checkCollisions();
  void textCentered(std::string text);
};
#endif
