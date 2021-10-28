#include "ruamov.hpp"

void Ruamov::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_velocity = glm::vec2{0.0f,0.8f};

  std::array<glm::vec2, 48> positions{
      // Faixa
      glm::vec2{-0.0272f, 0.9517f}, glm::vec2{0.0272f, 0.9517f},
      glm::vec2{-0.0272f, 0.5517f}, glm::vec2{0.0272f, 0.5517f}, 
      glm::vec2{-0.0272f, 0.4517f}, glm::vec2{0.0272f, 0.4517f},
      glm::vec2{-0.0272f, 0.0517f}, glm::vec2{0.0272f, 0.0517f}, 
      glm::vec2{-0.0272f, -0.0483f}, glm::vec2{0.0272f, -0.0483f},
      glm::vec2{-0.0272f, -0.4483f}, glm::vec2{0.0272f, -0.4483f}, 
      glm::vec2{-0.0272f, -0.5483f}, glm::vec2{0.0272f, -0.5483f},
      glm::vec2{-0.0272f, -0.9483f}, glm::vec2{0.0272f, -0.9483f}, 

      // Calçada
      glm::vec2{-0.625f, 0.7575f}, glm::vec2{-0.575f, 0.7575f},
      glm::vec2{-0.625f, 0.7475f}, glm::vec2{-0.575f, 0.7475f}, 
      glm::vec2{-0.625f, 0.2575f}, glm::vec2{-0.575f, 0.2575f},
      glm::vec2{-0.625f, 0.2475f}, glm::vec2{-0.575f, 0.2475f},
      glm::vec2{-0.625f, -0.2425f}, glm::vec2{-0.575f, -0.2425f},
      glm::vec2{-0.625f, -0.2525f}, glm::vec2{-0.575f, -0.2525f},
      glm::vec2{-0.625f, -0.7475f}, glm::vec2{-0.575f, -0.7475f},
      glm::vec2{-0.625f, -0.7575f}, glm::vec2{-0.575f, -0.7575f},
      glm::vec2{0.575f, 0.7575f}, glm::vec2{0.625f, 0.7575f},
      glm::vec2{0.575f, 0.7475f}, glm::vec2{0.625f, 0.7475f}, 
      glm::vec2{0.575f, 0.2575f}, glm::vec2{0.625f, 0.2575f},
      glm::vec2{0.575f, 0.2475f}, glm::vec2{0.625f, 0.2475f},
      glm::vec2{0.575f, -0.2425f}, glm::vec2{0.625f, -0.2425f},
      glm::vec2{0.575f, -0.2525f}, glm::vec2{0.625f, -0.2525f},
      glm::vec2{0.575f, -0.7475f}, glm::vec2{0.625f, -0.7475f},
      glm::vec2{0.575f, -0.7575f}, glm::vec2{0.625f, -0.7575f}};

  const std::array indices{//Faixas
                           0, 1, 2,
                           1, 2, 3,
                           4, 5, 6,
                           5, 6, 7,
                           8, 9, 10,
                           9, 10, 11,
                           12, 13, 14,
                           13, 14, 15,
                           
                          // Meio fio
                          16, 17, 18,
                          17, 18, 19,
                          20, 21, 22,
                          21, 22, 23,
                          24, 25, 26,
                          25, 26, 27,
                          28, 29, 30,
                          29, 30, 31,
                          32, 33, 34,
                          33, 34, 35,
                          36, 37, 38,
                          37, 38, 39,
                          40, 41, 42,
                          41, 42, 43,
                          44, 45, 46,
                          45, 46, 47};

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

void Ruamov::paintGL() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  //abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

 for (auto i : {-2, 0, 2}) {
    abcg::glUniform2f(m_translationLoc, m_translation.x, m_translation.y + i);

    m_color = glm::vec4(0.11f, 0.11f, 0.11f, 1.0f); // cinza
    abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
    abcg::glDrawElements(GL_TRIANGLES, 24 * 3, GL_UNSIGNED_INT, nullptr);

    m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // branco
    abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
    abcg::glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_INT, nullptr);
  }
  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Ruamov::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Ruamov::update(const GameData &gameData, float deltaTime) {
    if (gameData.m_state == State::GameOver) m_velocity = {0.0f, 0.0f}; // Rua para quando o jogo acaba

    m_translation -= m_velocity * deltaTime;

    // Reposiciona os elementos q saem de tela   
    if (m_translation.y < -1.0f) m_translation.y += 2.0f;
    if (m_translation.y > +1.0f) m_translation.y -= 2.0f;
}