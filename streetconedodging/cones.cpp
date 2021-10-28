#include "cones.hpp"

void Cones::initializeGL(GLuint program) {
  terminateGL();
  
  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Lista de cones vazia
  m_cones.clear();
  
    std::array<glm::vec2, 16> positions{
      // Cone
      glm::vec2{-1.4211f, -2.5f}, glm::vec2{1.4211f, -2.5f},   
      glm::vec2{-1.9222f, -3.8604f}, glm::vec2{1.9222f, -3.8604f},
      glm::vec2{-2.6855f, -5.9320f}, glm::vec2{2.6855f, -5.9320f},
      glm::vec2{-3.1711f, -7.2500f}, glm::vec2{3.1711f, -7.2500f},
      glm::vec2{-4.5f, -9.5f}, glm::vec2{4.5f, -9.5f},
      glm::vec2{-4.5f, -10.0f}, glm::vec2{4.5f, -10.0f},
      glm::vec2{-0.5f, 0.0f}, glm::vec2{0.5f, 0.0f},
      glm::vec2{-4.0f, -9.5f}, glm::vec2{4.0f, -9.5f}};
      
  // normaliza o cone para o tamanho da tela
  for (auto &position : positions) {
    position /= glm::vec2{4.5f, 4.5f};
  }

  const std::array indices{// Cone
                           0, 1, 2,
                           1, 2, 3,
                           4, 5, 6,
                           5, 6, 7,
                           8, 9, 10,
                           9, 10, 11,
                           12, 13, 14,
                           13, 14, 15};

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Cones::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &cone : m_cones) {
  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &cone.m_translation.x);
  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);

  abcg::glUniform2f(m_translationLoc, cone.m_translation.x, cone.m_translation.y);

  m_color = glm::vec4(1.0f, 0.644f, 0.0f, 1.0f); // laranja da base
  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_INT, nullptr);

  m_color = glm::vec4(1.0f, 0.469f, 0.0f, 1.0f); // laranja
  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, nullptr);

  m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // branco
  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
  }
  abcg::glUseProgram(0);
}

void Cones::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Cones::update(const GameData &gameData, float deltaTime) {
  for (auto &cone : m_cones) {
      if (gameData.m_state == State::GameOver) { // cone parado quando o jogo acaba
      cone.m_velocity = {0.0f, 0.0f};
    }
  }
  if(m_coneTimer.elapsed() > 1100.0 /1000) { 
      if(firstCone) { // espera mais um ciclo antes de gerar o primeiro cone pra evitar colisão instântanea 
        m_coneTimer.restart();
        firstCone = false;
        return;
      }
      int x = m_randomConesQtt(m_randomEngine);
      m_coneTimer.restart();

      if(x == 0) { // gera 1 cone
         int y = m_randomConesLoc(m_randomEngine);
         if(y == 0) { //cone a esquerda
             Cone cone {.foraDaTela = false, .m_translation = {-0.3f, +1.5f}, .m_velocity = {0.0f, 0.8f}};
             m_cones.push_back(cone);
         } else if(y == 1) { // cone no centro
             Cone cone {.foraDaTela = false, .m_translation = {0.0f, +1.5f}, .m_velocity = {0.0f, 0.8f}};
             m_cones.push_back(cone);
           } else { // cone a direita
             Cone cone {.foraDaTela = false, .m_translation = {0.3f, +1.5f}, .m_velocity = {0.0f, 0.8f}};
             m_cones.push_back(cone);
           }
      }
      else { // gera 2 cones
         int y = m_randomConesLoc(m_randomEngine);
         if(y == 0) { //cones a esquerda
             Cone cone1 {.foraDaTela = false, .m_translation = {0.0f, +1.5f}, .m_velocity = {0.0f, 0.8f}};
             Cone cone2 {.foraDaTela = false, .m_translation = {-0.375f, +1.5f}, .m_velocity = {0.0f, 0.8f}};
             m_cones.push_back(cone1);
             m_cones.push_back(cone2);
          }
          else if(y == 1) { // cones no centro
             Cone cone1 {.foraDaTela = false, .m_translation = {0.0f, +1.5f}, .m_velocity = {0.0f, 0.8f}};
             Cone cone2 {.foraDaTela = false, .m_translation = {0.375f, +1.5f}, .m_velocity = {0.0f, 0.8f}};
             m_cones.push_back(cone1);
             m_cones.push_back(cone2);
          }
          else { // cones a direita
             Cone cone1 {.foraDaTela = false, .m_translation = {0.175f, +1.5f}, .m_velocity = {0.0f, 0.8f}};
             Cone cone2 {.foraDaTela = false, .m_translation = {-0.175f, +1.5f}, .m_velocity = {0.0f, 0.8f}};
             m_cones.push_back(cone1);
             m_cones.push_back(cone2);
          }
       }
   }

   for(auto &cone : m_cones) {
      cone.m_translation -= cone.m_velocity * deltaTime;
   
      if(cone.m_translation.y < -1.5f) cone.foraDaTela = true; // cone saiu da tela = true
   }
    m_cones.remove_if([](const Cone &a) { return a.foraDaTela; }); // cone é removido da lista
}

