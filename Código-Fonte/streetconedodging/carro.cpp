#include "carro.hpp"

void Carro::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  m_translation = glm::vec2(0.0f,-0.65f);

  std::array<glm::vec2, 56> positions{
      // Parachoque Traseiro
      glm::vec2{-17.4606f, -57.6671f}, glm::vec2{17.4606f, -57.6671f},
      glm::vec2{-17.4606f, -62.6671f}, glm::vec2{17.4606f, -62.6671f}, 

      // Rodas e Motorista
      glm::vec2{-20.00f, -49.6671f}, glm::vec2{-13.4606f, -49.6671f}, 
      glm::vec2{-20.00f, -59.1671f}, glm::vec2{-13.4606f, -59.1671f}, 
      glm::vec2{13.4606f, -49.6671f}, glm::vec2{20.00f, -49.6671f},
      glm::vec2{13.4606f, -59.1671f}, glm::vec2{20.00f, -59.1671f},
      glm::vec2{-20.00f, -7.50f}, glm::vec2{-15.00f, -7.50f},
      glm::vec2{-20.00f, -15.50f}, glm::vec2{-15.00f, -15.50f},
      glm::vec2{15.00f, -7.50f}, glm::vec2{20.00f, -7.50f},
      glm::vec2{15.00f, -15.50f}, glm::vec2{20.00f, -15.50f},
      glm::vec2{-2.50f, -34.6671f}, glm::vec2{2.50f, -34.6671f},
      glm::vec2{-5.0f, -40.6671f}, glm::vec2{5.00f, -40.6671f},
      glm::vec2{-2.5f, -46.6671f}, glm::vec2{2.50f, -46.6671f},

      // Compartimento Grande
      glm::vec2{-7.6001f, -27.6671f}, glm::vec2{7.6001f, -27.6671f},
      glm::vec2{-13.4606f, -37.5021f}, glm::vec2{13.4606f, -37.5021f},
      glm::vec2{-13.4606f, -57.6671f}, glm::vec2{13.4606f, -57.6671f},
      
      // Triangulo Grande
      glm::vec2{-2.00f, 1.0f}, glm::vec2{2.00, 1.0f},
      glm::vec2{-13.4606f, -49.6671f}, glm::vec2{13.4606f, -49.6671f},

      // Eixo de Rodas
      glm::vec2{-15.00f, -10.00f}, glm::vec2{15.00f, -10.00f},
      glm::vec2{-15.00f, -13.00f}, glm::vec2{15.00f, -13.00f},

      // Parachoque
      glm::vec2{-18.00f, 0.00f}, glm::vec2{18.00f, 0.00f},
      glm::vec2{-19.00f, -4.00f}, glm::vec2{19.00f, -4.00f},

      // Listras
      glm::vec2{-13.4606f, -43.5846f}, glm::vec2{13.4606f, -43.5846f},
      glm::vec2{-13.4606f, -44.5846f}, glm::vec2{13.4606f, -44.5846f},
      glm::vec2{0.0f, -53.6671f}, glm::vec2{0.0f, -54.6671f},
      glm::vec2{-13.4606f, -46.5846f}, glm::vec2{13.4606f, -46.5846f},
      glm::vec2{-13.4606f, -47.5846f}, glm::vec2{13.4606f, -47.5846f},
      glm::vec2{0.0f, -56.6671f}, glm::vec2{0.0f, -57.6671f}
      };

  // Ajusta o desenho pro tamanho da tela
  for (auto &position : positions) {
    position /= glm::vec2{20.00f, 20.00f};
  }

  const std::array indices{//Parachoque traseiro
                           0, 1, 2,
                           1, 2, 3,

                          //Rodas e Motorista
                           4, 5, 6,
                           5, 6, 7,
                           8, 9, 10,
                           9, 10, 11,
                           12, 13, 14,
                           13, 14, 15,
                           16, 17, 18,
                           17, 18, 19,
                           20, 22, 24,
                           20, 24, 25,
                           20, 21, 25,
                           21, 23, 25,
                          
                          //Listras
                           44, 46, 48,
                           44, 48, 49,
                           45, 48, 49,
                           45, 47, 49,

                           50, 52, 54,
                           50, 54, 55,
                           51, 54, 55,
                           51, 53, 55,

                          //Compartimento Grande
                           26, 28, 30,
                           26, 30, 31,
                           26, 27, 31,
                           27, 29, 31,

                          //Triangulo Grande
                           32, 30, 31,
                           32, 33, 31,

                          //Eixo das Rodas
                           36, 37, 38,
                           37, 38, 39,

                          //Parachoque
                           40, 41, 42,
                           41, 42, 43};

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

void Carro::paintGL() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  m_color = glm::vec4(0.502f, 0.0f, 0.0f, 1.0f); // parachoque -- vermelho

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 32 * 3, GL_UNSIGNED_INT, nullptr);

  m_color = glm::vec4(0.66f, 0.66f, 0.66f, 1.0f); // eixo das rodas - cinza

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 30 * 3, GL_UNSIGNED_INT, nullptr);

  m_color = glm::vec4(0.7f, 0.1f, 0.1f, 1.0f); // triangulo grande - vermelho

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 28 * 3, GL_UNSIGNED_INT, nullptr);

  m_color = glm::vec4(0.8f, 0.1f, 0.1f, 1.0f); // compartimento grande - vermelho

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 26 * 3, GL_UNSIGNED_INT, nullptr);

  m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // listras -- branco

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 22 * 3, GL_UNSIGNED_INT, nullptr);

  m_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // rodas e motorista - preto

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 14 * 3, GL_UNSIGNED_INT, nullptr);

  m_color = glm::vec4(0.502f, 0.0f, 0.0f, 1.0f); // parachoque traseiro - vermelho

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Carro::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Carro::update(const GameData &gameData, float deltaTime) {
  if (gameData.m_state == State::GameOver) return;

  // Movimento e limita o carro dentro da pista
  if(m_translation[0] <= 0.480f && m_translation[0] >= -0.480f){
    if (gameData.m_input[static_cast<size_t>(Input::Left)])
      m_translation[0] -= (3.0f/2.0f) * deltaTime;
    if (gameData.m_input[static_cast<size_t>(Input::Right)])
      m_translation[0] += (3.0f/2.0f) * deltaTime;
  } else if(m_translation[0] > 0.0f){
    if (gameData.m_input[static_cast<size_t>(Input::Left)])
      m_translation[0] -= (3.0f/2.0f) * deltaTime;
  } else if(m_translation[0] < 0.0f)
    if (gameData.m_input[static_cast<size_t>(Input::Right)])
      m_translation[0] += (3.0f/2.0f) * deltaTime;
}