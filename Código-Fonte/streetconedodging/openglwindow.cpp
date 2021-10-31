#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

  glm::ivec2 mousePosition;
  glm::vec2 direction;
  std::string pontos = "Score:"; 
  std::string record = "Record:"; 
  std::string tempo = "Restarting in:";

  bool primeirosec = true;
  bool newRecord = false;
  int pontosNum = 0;
  int recordNum = 0;
  int segundos = 5;

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }

    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
    glm::vec2 direction{glm::vec2{mousePosition.x - m_viewportWidth / 2,
                                  mousePosition.y - m_viewportHeight / 2}};
  
  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT  && direction.x > 0.0f) {
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
      }
    if (event.button.button == SDL_BUTTON_LEFT  && direction.x < 0.0f) {
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
      }
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
      }
  } 
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "ARCADE_N.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 35.0f);
  m_font2 = io.Fonts->AddFontFromFileTTF(filename.c_str(), 20.0f);
  if (m_font == nullptr || m_font2 == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  abcg::glClearColor(0, 0, 0, 1);

  // Start pseudo-random number generator
  m_randomEngine.seed(std::chrono::steady_clock::now().time_since_epoch().count());

  m_nuvens.initializeGL(m_objectsProgram, 4);
  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_rua.initializeGL(m_objectsProgram);
  m_ruamov.initializeGL(m_objectsProgram);
  m_arvores.initializeGL(m_objectsProgram, 8);
  m_carro.initializeGL(m_objectsProgram);
  m_cones.initializeGL(m_objectsProgram);

  for(auto &nuvem : m_nuvens.m_nuvens) nuvem.m_velocity = glm::vec2({0.05f, 0.8f});

  primeirosec = true;
  newRecord = false;
  pontosNum = 0;
  segundos = 5;
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing && m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  m_nuvens.update(m_gameData, deltaTime);
  m_cones.update(m_gameData, deltaTime);
  m_carro.update(m_gameData, deltaTime);
  m_arvores.update(m_gameData, deltaTime);
  m_ruamov.update(m_gameData, deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
  }
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_rua.paintGL();
  m_ruamov.paintGL();
  m_carro.paintGL();
  m_cones.paintGL();
  m_arvores.paintGL();
  m_nuvens.paintGL();
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  {
    const auto size{ImVec2(m_viewportWidth, 400)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) { 
      if(m_segundosTimer.elapsed() > 1000.0 /1000) { // Timer de recomeço 
        segundos -= 1;
         m_segundosTimer.restart();
      }
      if(newRecord) {
        ImGui::Text(" ");
        textCentered("Game Over!");
        ImGui::Text(" ");
        textCentered("*New Record*");
        textCentered("Congratulations!");
        ImGui::Text(" ");
        ImGui::Text(" ");
        textCentered((tempo + std::to_string(segundos).c_str()));
      } else {
        ImGui::Text(" ");
        ImGui::Text(" ");
        textCentered("Game Over!");
        ImGui::Text(" ");
        ImGui::Text(" ");
        textCentered((tempo + std::to_string(segundos).c_str()));
      }
    }   
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::End();
  }

  abcg::OpenGLWindow::paintUI();
  {
    const auto size{ImVec2(m_viewportWidth * 0.5f, 100)};
    const auto position{ImVec2(-1, -1)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" g ", nullptr, flags);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,215,0,255));
    ImGui::PushFont(m_font2);
    textCentered((pontos + std::to_string(pontosNum).c_str()));
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::End();
  }

  abcg::OpenGLWindow::paintUI();
  {
    const auto size{ImVec2(m_viewportWidth * 0.5f, 100)};
    const auto position{ImVec2(m_viewportWidth * 0.5f, -1)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" f ", nullptr, flags);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,215,0,255));
    ImGui::PushFont(m_font2);
    textCentered((record + std::to_string(recordNum).c_str()));
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::End();
  }

  if(m_gameData.m_state == State::Playing) {
    if(m_pontosTimer.elapsed() > 1000.0 /1000) {
      if(primeirosec) { // Garante que o score comece em 0
        m_pontosTimer.restart();
        primeirosec = false;
        return;
      } 
      pontosNum += 15;
      m_pontosTimer.restart();
    }
  }

  if(m_gameData.m_state == State::GameOver) {
    if(pontosNum > recordNum) { // Verifica se ocorreu um  novo record
      recordNum = pontosNum;
      newRecord = true;
    }
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_objectsProgram);

  m_nuvens.terminateGL();
  m_carro.terminateGL();
  m_cones.terminateGL();
  m_arvores.terminateGL();
  m_rua.terminateGL();
  m_ruamov.terminateGL();
}

void OpenGLWindow::checkCollisions() { // verifica colisões entre retângulos (base do cone e o carro)
  float larguraCone = m_cones.m_scale*2 -0.08;
  float larguraCarro = m_carro.m_scale*2;
  float alturaCone = larguraCone - 0.21f;
  float alturaCarro = larguraCarro + 0.14f;

  for(auto &cone : m_cones.m_cones) {
     if(cone.m_translation[0] < m_carro.m_translation[0] + larguraCarro &&
        cone.m_translation[0] + larguraCone >  m_carro.m_translation[0] &&
        cone.m_translation[1] < m_carro.m_translation[1] + alturaCarro  &&
        cone.m_translation[1] + alturaCone > m_carro.m_translation[1]) {
          m_gameData.m_state = State::GameOver;
          m_restartWaitTimer.restart();
        }
  } 
}

void OpenGLWindow::textCentered(std::string text) { // centraliza texto do ImGui
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth   = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text(text.c_str());
}
