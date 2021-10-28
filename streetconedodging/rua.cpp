#include "rua.hpp"

void Rua::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

 std::array<glm::vec2, 12> positions{
    // Desenho da Rua
    glm::vec2{-1.0f, 1.0f}, glm::vec2{-0.625f, 1.0f},
    glm::vec2{-0.575f, 1.0f}, glm::vec2{0.575f, 1.0f},
    glm::vec2{0.625f, 1.0f}, glm::vec2{1.0f, 1.0f},
    glm::vec2{-1.0f, -1.0f}, glm::vec2{-0.625f, -1.0f},
    glm::vec2{-0.575f, -1.0f}, glm::vec2{0.575f, -1.0f},
    glm::vec2{0.625f, -1.0f}, glm::vec2{1.0f, -1.0f}};

  const std::array indices{//Meio fio
                           1, 2, 7,
                           2, 7, 8,
                           3, 4, 9,
                           4, 9, 10,
                           
                           //Campo
                           0, 2, 6,
                           2, 6, 8,
                           3, 5, 9,
                           5, 9, 11,

                          // Asfalto
                           2, 3, 8,
                           3, 8, 9};

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

void Rua::paintGL() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  m_color = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f); // preto

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 10 * 3, GL_UNSIGNED_INT, nullptr);

  m_color = glm::vec4(0.0f, 0.46f, 0.0f, 1.0f); // verde

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_INT, nullptr);

  m_color = glm::vec4(0.613f, 0.613f, 0.613f, 1.0f); // cinza

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Rua::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}