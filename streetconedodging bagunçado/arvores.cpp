#include "arvores.hpp"

void Arvores::initializeGL(GLuint program, int quantity) {
  terminateGL();
  
  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Quantidade de arvores
  m_arvores.clear();
  m_arvores.resize(quantity);

  float j = 0, k = 0;
  int i = 0;
  for (auto &arvore : m_arvores) { // Define a posição exata de cada arvore na lista
   arvore = createArvore();

   if(i < quantity/2) {
     arvore.m_translation = {-0.8125, 0.75 + j}; 
     j -= 0.5;  
     if(i%2) arvore.m_translation[0] -= 0.035625;
     else arvore.m_translation[0] += 0.035625;
     i++;
   } else {
       arvore.m_translation = {0.8125, 0.75 + k};
       k -= 0.5;
       if(i%2) arvore.m_translation[0] -= 0.035625;
       else arvore.m_translation[0] += 0.035625; 
       i++;
     }
  }
}

void Arvores::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &arvore : m_arvores) {
   abcg::glBindVertexArray(arvore.m_vao);
   abcg::glUniform1f(m_scaleLoc, arvore.m_scale);
   abcg::glUniform2fv(m_translationLoc, 1, &arvore.m_translation.x);
   abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);

   for (auto i : {-2, 0, 2}) {
     abcg::glUniform2f(m_translationLoc, arvore.m_translation.x, arvore.m_translation.y + i);

     m_color = glm::vec4(0.0f, 0.350f, 0.0f, 1.0f); // verde
     abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
     abcg::glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_INT, nullptr);

     m_color = glm::vec4(0.543f, 0.269f, 0.074f, 1.0f); // marrom
     abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
     abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);
    }
  abcg::glBindVertexArray(0);
  }
  abcg::glUseProgram(0);
}

void Arvores::terminateGL() {
  for(auto &arvore : m_arvores) {
  abcg::glDeleteBuffers(1, &arvore.m_vbo);
  abcg::glDeleteBuffers(1, &arvore.m_ebo);
  abcg::glDeleteVertexArrays(1, &arvore.m_vao);
  }
}

void Arvores::update(const GameData &gameData, float deltaTime) {
   for (auto &arvore : m_arvores) { 
     if (gameData.m_state == State::GameOver) { // arvore parada quando jogo acaba
       arvore.m_velocity = {0.0f, 0.0f};
    }

    arvore.m_translation -= arvore.m_velocity * deltaTime;
    
   // Trocar de lado quando sai da tela em y
    if (arvore.m_translation.y < -1.0f) arvore.m_translation.y += 2.0f;
    if (arvore.m_translation.y > +1.0f) arvore.m_translation.y -= 2.0f;
  }
}

Arvores::Arvore Arvores::createArvore() {

  Arvore arvore;

  std::array<glm::vec2, 10> positions{
    // Arvore
    glm::vec2{-1.0f, -10.0f}, glm::vec2{1.0f, -10.0f},
    glm::vec2{-1.0f, -13.0f}, glm::vec2{1.0f, -13.0f},  
    glm::vec2{0.0f, 0.0f}, glm::vec2{-5.0f, -10.0f},
    glm::vec2{5.0f, -10.0f}, glm::vec2{0.0f, 4.0f},
    glm::vec2{-4.0f, -5.0f}, glm::vec2{4.0f, -5.0f}
    };

  // Normaliza o desenho pro tamanho da tela 
  for (auto &position : positions) {
     position /= glm::vec2{5.0f, 5.0f};
  }

  const std::array indices{// Arvore
                           0, 1, 2,
                           1, 2, 3,
                           4, 5, 6,
                           7, 8, 9};

  // Generate VBO
  abcg::glGenBuffers(1, &arvore.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, arvore.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &arvore.m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arvore.m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &arvore.m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(arvore.m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, arvore.m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arvore.m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

return arvore;
}