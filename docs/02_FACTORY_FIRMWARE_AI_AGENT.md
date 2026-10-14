# 02 — Firmware de Fábrica e Agente de IA

O firmware oficial de fábrica do **M5Stack Stack-chan** transforma o dispositivo num assistente robótico de secretária completo com Inteligência Artificial, baseado no motor conversacional **XiaoZhi**, com controlo de movimentos, visão por câmara e integração domótica.

---

## 1. Instalação da Aplicação Móvel (*StackChan World*)

Para configurar a rede Wi-Fi, gerir os modelos de linguagem (LLM) e emparelhar o robô, utilize a app oficial:
* **iOS**: [StackChan World na App Store](https://apps.apple.com/us/app/stackchan-world/id6756086326)
* **Android**: [StackChan World na Google Play Store](https://play.google.com/store/apps/details?id=com.m5stack.stackchan)
* **Android (Instalador Direto APK)**: [Download StackChan World APK](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1205/StackChan_World_apk.zip)

Crie ou utilize a sua conta **M5Stack Community** (a mesma utilizada no UiFlow e M5Burner).

---

## 2. Primeiro Arranque e Emparelhamento

1. **Ligação e Teste Mecânico**:
   * Ligue o cabo USB à porta inferior da base ou prima o botão de ligar.
   * O ecrã mostrará a mensagem de boas-vindas. Toque em **Start**.
   * Coloque o robô sobre uma superfície plana e estável sem lhe tocar. Toque novamente em **Start** para iniciar o teste automático dos servos.
   * O robô mover-se-á para a esquerda, direita e para cima, regressando à posição neutra (*Home*).
2. **Emparelhamento Bluetooth**:
   * No ecrã do robô, avance até ver o código identificador (ID / Endereço MAC).
   * Abra a app **StackChan World** no smartphone com o **Bluetooth ativado** e permissões concedidas.
   * Clique em **Add a new StackChan** e selecione o dispositivo da lista.
3. **Configurações Iniciais**:
   * **Nome do Robô**: Nome de registo na sua conta.
   * **Configuração da IA**:
     * Nome de apresentação da IA.
     * Idioma de conversação.
     * Modelo de IA (LLM).
     * Tipo de voz, velocidade e tom.
     * Personalidade e memória conversacional.
   * **Rede Wi-Fi**: Selecione a sua rede **2.4 GHz** e introduza a palavra-passe. (Redes 5 GHz e redes com portal de autenticação *captive portal* não são suportadas).
   * Ao finalizar, o robô reinicia automaticamente com a nova configuração.

---

## 3. Funcionalidades do Agente de IA (XiaoZhi)

### Ativação e Indicadores Luminosos
* **Palavra de Ativação (*Wake Word*)**: `"Hi, StackChan"` (também pode ser acordado com um toque no ecrã).
* **LED RGB Superior (Indicador de Estado)**:
  * 🟢 **Verde Fixo**: A escutar a voz do utilizador (*Listening*).
  * 🔵 **Azul Intermitente/Fixo**: A responder/falar (*Speaking*).
  * ⚫ **Apagado**: Em repouso (*Idle*).

### Expressões Faciais e Reações Físicas
* **Em repouso**: Pisca os olhos aleatoriamente e faz movimentos subtis com a cabeça.
* **Toque no topo (Swipe vertical)**: Desencadeia uma expressão alegre 🥰 e uma dança de celebração.
* **Agitar o robô**: Desencadeia uma expressão tonta/tonta 😵‍💫 com oscilação dos olhos.
* **Inatividade prolongada**: Entra em modo de sono 😴.

### Exemplos de Comandos de Voz Suportados
* **Comandos Gerais**:
  * *"Que horas são?"*
  * *"Qual é a previsão do tempo para Lisboa amanhã?"*
  * *"Conta-me uma piada sobre robótica."*
  * *"Lembra-me de beber água daqui a 15 minutos."*
* **Controlo de Hardware Interno**:
  * *"Coloca o volume a 70%."*
  * *"Vira a cabeça para a esquerda."*
  * *"Muda a luz para azul."*
  * *"O que estás a ver agora?"* (utiliza a câmara integrada).
  * *"Qual é a percentagem de bateria?"*

---

## 4. Integração com Home Assistant (MCP)

O Stack-chan suporta o protocolo **MCP (Model Context Protocol)** através do plugin de integração XiaoZhi para o Home Assistant:
* Permite controlar diretamente entidades da casa inteligente por voz:
  * *"Desliga as luzes do quarto."*
  * *"Regula o ar condicionado para 22 graus."*
  * *"Qual é a temperatura na sala de estar?"*
* Repositório do conetor: [ha-mcp-for-xiaozhi](https://github.com/c1pher-cn/ha-mcp-for-xiaozhi).

---

## 5. Modos de Operação Adicionais

### Modo Avatar (Telepresença e Espelho)
* Ao selecionar **Avatar** no robô e na aplicação móvel:
  * **Espelho Facial**: O robô mimetiza a posição dos olhos, boca e movimentos de cabeça da pessoa em frente à câmara do telemóvel.
  * **Câmara de Vigilância**: Permite ver o feed de vídeo ao vivo no telemóvel e rodar o robô através de um joystick virtual.
  * **Choreography Dance**: Crie sequências de movimentos sincronizados com música e efeitos de cor nos 12 LEDs RGB.

### Modo ESP-NOW (Comando Remoto Sem Fios)
No kit `K151-R`, o controlo é efetuado através do telecomando **StickC-Plus** com **Hat Mini JoyC**:
* **Ligar Comando**: Premir o botão lateral durante 2s.
* **Modos do Comando**:
  * **BtnB**: Alterna entre canal Wi-Fi e ID do recetor (utilize o joystick para alterar os valores).
  * **BtnA**: Alterna entre controlo por Joystick e controlo por inclinação física (IMU).
* **Espelho de Movimento**: Ao colocar um Stack-chan como emissor e outro como recetor, **rodar manualmente a cabeça do primeiro fará o segundo rodar exatamente para o mesmo ângulo**.

### App Center e Jogos
* Permite descarregar minijogos e utilitários adicionais:
  * Jogo dos Canos (*Flappy Stack*).
  * Estabilizador de horizonte com giroscópio IMU.
  * Telecomando universal de aprendizagem de Infravermelhos (IR Learning Remote).
