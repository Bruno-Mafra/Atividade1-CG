# Atividade 1 - Computação Gráfica 

Esse repositório contém o código-fonte de um jogo feito para a disciplina de Computação Gráfica da UFABC (DA1MCTA008-17SA) ministrada por Bruno Marques (2021).

## Street Cone Dodging

O jogo consiste em desviar dos obstáculos (cones) com o carro e permanecer correndo na pista para obter pontos, quanto mais tempo o carro desviar maior será a pontuação. Dessa forma, o jogo só acaba quando ocorre uma colisão. Você pode jogar através da github page associada a esse repositório ou no link: https://bruno-mafra.github.io/Atividade1-CG/.

### Como jogar
Utilize as setas direcionais ou as teclas 'A' e 'D' para controlar o carro para esquerda e direita respectivamente. Como uma alternativa, também é possível utilizar o botão esquerdo do mouse clickando e posicionando o ponteiro na tela sobre o lado que deseja que o carrinho vá, assim é possível jogar até mesmo através de um celular que acesse o jogo pelo navegador com a opção de visualização "Versão para Desktop".

Como já foi dito, o objetivo é desviar dos cones pela maior quantidade de tempo possível, para isso vale lembrar que a colisão entre cone e carrinho é **apenas frontal**, isso significa que uma vez que o carrinho ultrapassa a frente do cone ele pode se movimentar para se sobrepor com o cone. A ideia é que o carrinho fica atrás do cone porque já o ultrapassou.   
  
### Como compilar
Para compilar o código-fonte disponibilizado nesse repositório diretamente no seu computador é necessário fazer a configuração do ambiente apresentada no inicio da disciplina e adicionar a pasta "streetconedodging" (que está na pasta 'Código-Fonte' desse repositório) no diretório examples do abcg. Também é necessário adicionar o arquivo CMakeLists.txt nesse mesmo diretório. Depois disso é so compilar no terminal para gerar o executável.

## Conteudo e Técnicas utilizadas

O jogo foi constrúido em cima do 'asteroids', mas basicamente tudo foi alterado/modificado. Além do openglwindow.cpp, existem novas classes nesse código responsáveis por cada elemento que aparece na tela, são elas: arvores.cpp, rua.cpp, ruamov.cpp, nuvens.cpp, carro.cpp e cones.cpp.

### carro.cpp
Responsável pela exibição do desenho do carro na tela e as condições de movimento dele na pista.

### nuvens.cpp
Responsável pela exibição do desenho de cada nuvem na tela e as condições de posição e movimento delas como velocidade.

### rua.cpp
Responsável pelo "plano de fundo" do jogo que seria uma pista delimitada por uma calçada e uma região de campo nas laterais.

### ruamov.cpp
Responsável pelos elementos da pista que se movimentam como as faixas na rua e as divisões de calçada.

### arvore.cpp
Responsável pela exibição do desenho de cada árvore na tela e as condições de posição e movimento delas como velocidade.

### cones.cpp
Responsável pela exibição do desenho dos cones na tela e as condições de surgimento, para isso foi implementado uma lógica de geração aleátoria de cones na pista. Eles sempre aparecem a cada 1 segundo sozinhos ou em dupla podendo aparecer no centro ou nas laterais da pista. Quando saem para fora dos limites da tela são destrúidos.

### openglwindow
Classe principal que gerencia instâncias das demais classes. Nessa classe também foram feitas muitas alterações como:

- Lógica para a movimentação atráves do uso do mouse
- Utilização de uma nova fonte em tamanhos e cores diferentes
- Implementação de um contador de pontos e marcação de record
- Implementação de um contador de tempo até recomeçar o jogo
- Mudança no método que detecta colisões (agora é considerado retângulos e não círculos)
- Método responsável por centralização de texto do ImGui

<center>

O resultado final ficou como a seguir:

![Imagem](https://i.imgur.com/gQNGOeT.png)
  
</center>
