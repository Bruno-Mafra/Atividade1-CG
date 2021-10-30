#include "nuvens.hpp"
 
void Nuvens::initializeGL(GLuint program, int quantity) {
  terminateGL();

  // permite a transparência da nuvem
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  
  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Quantidade de nuvens
  m_nuvens.clear();
  m_nuvens.resize(quantity);
  
  for (auto &nuvem : m_nuvens) {
   nuvem = createNuvem();
  }
  float i=0, j=0;
  
  for (auto &nuvem : m_nuvens) {
   // gera nuvens aleatoriamente longe uma das outras
   nuvem.m_translation = {m_randomDist(m_randomEngine) + i, m_randomDist(m_randomEngine) + j}; 
   do {
 	i = m_randomDist(m_randomEngine);} while(i < nuvem.m_translation.x*0.5f); 
   do {
 	j = m_randomDist(m_randomEngine);} while(j < nuvem.m_translation.x*0.5f); 
  }
}

void Nuvens::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &nuvem : m_nuvens) {
   abcg::glBindVertexArray(nuvem.m_vao);
   abcg::glUniform1f(m_scaleLoc, nuvem.m_scale);
   abcg::glUniform2fv(m_translationLoc, 1, &nuvem.m_translation.x);
   abcg::glUniform4fv(m_colorLoc, 1, &nuvem.m_color.r);

   for (auto i : {-2, 0, 2}) {
      for (auto j : {-2, 0, 2}) {
        abcg::glUniform2f(m_translationLoc, nuvem.m_translation.x + j,
                          nuvem.m_translation.y + i);
    abcg::glDrawElements(GL_TRIANGLES, 29 * 3, GL_UNSIGNED_INT, nullptr);
      }
   }
   abcg::glBindVertexArray(0);
  }
  abcg::glUseProgram(0);
}

void Nuvens::terminateGL() {
  for(auto &nuvem : m_nuvens) {
  abcg::glDeleteBuffers(1, &nuvem.m_vbo);
  abcg::glDeleteBuffers(1, &nuvem.m_ebo);
  abcg::glDeleteVertexArrays(1, &nuvem.m_vao);
  }
}

void Nuvens::update(const GameData &gameData, float deltaTime) {
   for (auto &nuvem : m_nuvens) {
    if (gameData.m_state == State::GameOver) { // quando jogo acaba nuvens se movem mais devagar
      nuvem.m_velocity = {0.05f, 0.05f};
    }

    nuvem.m_translation -= nuvem.m_velocity * deltaTime;
   
   // Nuvens trocam de lado quando saem da tela
    if (nuvem.m_translation.x < -1.0f) nuvem.m_translation.x += 2.0f;
    if (nuvem.m_translation.x > +1.0f) nuvem.m_translation.x -= 2.0f;
    if (nuvem.m_translation.y < -1.0f) nuvem.m_translation.y += 2.0f;
    if (nuvem.m_translation.y > +1.0f) nuvem.m_translation.y -= 2.0f;
  }
}

Nuvens::Nuvem Nuvens::createNuvem() {

  Nuvem nuvem;

   std::array<glm::vec2, 34> positions{
      // Nuvem
      glm::vec2{-57.7745f, 42.5157f}, glm::vec2{-40.9917f, 42.5157f},   
      glm::vec2{-40.9917f, 51.3431f}, glm::vec2{-13.1556f, 51.3431f},
      glm::vec2{-13.1556f, 42.5157f}, glm::vec2{00.8277f, 42.5157f},
      glm::vec2{00.8277f, 37.3873f}, glm::vec2{12.3611f, 37.3873f},
      glm::vec2{12.3611f, 30.8630f}, glm::vec2{36.5519f, 30.8630f},
      glm::vec2{36.5519f, 36.9894f}, glm::vec2{54.7781f, 36.9894f},
      glm::vec2{54.7781f, 28.7105f}, glm::vec2{66.0451f, 28.7105f},
      glm::vec2{66.0451f, 10.4221f}, glm::vec2{75.9841f, 10.4221f},
      glm::vec2{75.9841f, -06.7234f}, glm::vec2{71.0159f, -06.7234f},
      glm::vec2{71.0159f, -15.7158f}, glm::vec2{52.6426f, -15.7158f},
      glm::vec2{52.6426f, -27.2552f}, glm::vec2{29.3011f, -27.2552f},
      glm::vec2{29.3011f, -37.2218f}, glm::vec2{-10.0268f, -37.2218f},
      glm::vec2{-10.0268f, -31.8407f}, glm::vec2{-43.1067f, -31.8407f},
      glm::vec2{-43.1067f, -18.5496f}, glm::vec2{-57.7745f, -18.5496f},
      glm::vec2{-57.7745f, -10.7530f}, glm::vec2{-75.9841f, -10.7530f},
      glm::vec2{-75.9841f, 10.7530f}, glm::vec2{-66.8793f, 10.7530f},
      glm::vec2{-66.8793f, 25.3990f}, glm::vec2{-57.7745f, 25.3990f}};
      
  // Normaliza o tamanho da nuvem para caber na tela
  for (auto &position : positions) {
    position /= glm::vec2{75.9841f, 75.9841f};
  }

  const std::array indices{//Nuvem
                           1, 2, 3,
                           1, 3, 4,
                           0, 26, 4,
                           0, 26, 27,
                           28, 31, 33,
                           31, 32, 33,
                           29, 30, 31,
                           28, 29, 31,
                           4, 5, 26,
                           5, 6, 26,
                           6, 7, 26,
                           7, 8, 26,
                           8, 25, 26,
                           8, 24, 25,
                           8, 21, 24,
                           8, 9, 21,
                           21, 22, 24,
                           22, 23, 24,
                           9, 20, 21,
                           9, 19, 20,
                           9, 12, 19,
                           9, 10, 12,
                           10, 11, 12,
                           12, 13, 14,
                           12, 14, 19,
                           14, 17, 19,
                           17, 18, 19,
                           14, 15, 17,
                           15, 16, 17};

  // Generate VBO
  abcg::glGenBuffers(1, &nuvem.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, nuvem.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &nuvem.m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nuvem.m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &nuvem.m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(nuvem.m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, nuvem.m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nuvem.m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return nuvem;
}
