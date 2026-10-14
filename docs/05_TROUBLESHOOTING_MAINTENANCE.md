# 05 — Manutenção, Restauro de Firmware e Resolução de Problemas

---

## 1. Restauro do Firmware Original com o M5Burner

Se gravou um sketch em Arduino ou ESPHome e pretende restaurar o firmware de fábrica com IA XiaoZhi:

1. **Descarregar o M5Burner**:
   * [M5Burner para Windows (x64)](https://m5burner-cdn.m5stack.com/app/M5Burner-v3-beta-win-x64.zip)
   * [M5Burner para macOS (x64)](https://m5burner-cdn.m5stack.com/app/M5Burner-v3-mac-x64.dmg)
   * [M5Burner para Linux (x64)](https://m5burner-cdn.m5stack.com/app/M5Burner-v3-beta-linux-x64.zip)
2. **Localizar o Firmware**:
   * Abra o M5Burner e certifique-se de que selecionou a aba do CoreS3 / StackChan.
   * Na barra de pesquisa, procure por `StackChan`, selecione a opção **Only Official** e clique em **Download**.
3. **Gravar no Dispositivo**:
   * Ligue o Stack-chan por USB ao computador (porta inferior da base).
   * Se a porta COM não for detetada automaticamente, force o **Modo de Download**:
     * Prima o botão **RST** durante **3 segundos** até o **LED verde acender**; depois solte-o.
   * Clique em **Burn**, selecione a porta correta e confirme com **Start**.
   * Aguarde até que a barra de progresso atinja 100%.

---

## 2. Resolução de Problemas de Emparelhamento e Rede

### A App não Encontra o Stack-chan por Bluetooth
* **Causa**: Permissões de localização/Bluetooth em falta no smartphone.
* **Solução**: Nas definições do telemóvel, conceda todas as permissões de "Dispositivos Próximos" e "Bluetooth" à app *StackChan World*.
* Reinicie a pesquisa na app enquanto o ecrã do robô mostra o ecrã de emparelhamento com o respetivo ID.

### Falha na Ligação Wi-Fi
* O Stack-chan **apenas suporta frequências de 2.4 GHz**.
* Redes de 5 GHz ou redes de hotel/empresa com ecrã de início de sessão (*Captive Portal*) não são suportadas.
* Se o seu router unificar as redes 2.4 GHz e 5 GHz no mesmo SSID (*Smart Connect*), desative temporariamente a banda de 5 GHz durante a configuração inicial.

---

## 3. Regra Crítica: Emparelhamento XiaoZhi (Original vs. Custom)

> [!WARNING]
> **Aviso de Bloqueio de Conta XiaoZhi**:
> * O Agente de IA do firmware de fábrica comunica com os servidores XiaoZhi geridos pela app **StackChan World**.
> * Se pretender utilizar firmwares comunitários XiaoZhi configurados em **`xiaozhi.me`**, **deve obrigatoriamente desemparelhar o robô (*Unbind*)** nas Definições do Stack-chan ou na app StackChan World antes de regravar o firmware.
> * O inverso é igualmente obrigatório: para regressar ao firmware oficial, desemparelhe primeiro o dispositivo na plataforma comunitária, sob pena de erro persistente de autorização.

---

## 4. Diagnóstico de Falhas de Ativação por Voz

Se ao dizer `"Hi, StackChan"` o robô não responder:
1. **Verifique o LED Superior**:
   * Se o LED acende a verde mas a resposta falha ou é truncada, a causa é **latência/instabilidade da ligação Wi-Fi** aos servidores cloud do modelo.
2. **Ativação por Toque**:
   * Toque uma vez no ecrã para forçar a escuta. Evite toques repetidos rápidos, pois isso cancela o ciclo de escuta.
3. **Volume e Microfone**:
   * No menu de definições do próprio robô (*Setup*), execute o **Microphone Test** para verificar o gráfico de nível sonoro dos dois microfones ES7210.

---

## 5. Recalibração dos Servos e Desvios de Posição

Se a cabeça do robô ficar descentrada ou não regressar à posição frontal:
1. No menu principal do robô, aceda a **Setup** -> **Servo Calibration**.
2. **Com suavidade e sem forçar** (verifique se o motor não está a travar contra a sua mão), alinhe a cabeça perfeitamente com a perpendicular da base e centrada entre os dois apoios frontais.
3. Toque em **Set Current Position As Home**.
4. Desloque ligeiramente a cabeça para o lado e prima **Move To Home** para confirmar que ela regressa com precisão ao ponto neutro.
5. Prima **Done** para guardar a calibração na memória flash.

---

## 6. Testes Independentes de Hardware do CoreS3

Se suspeitar de danos no ecrã, bateria ou no módulo ESP32-S3:
* O módulo CoreS3 (cabeça) pode ser desacoplado da base desaparafusando os parafusos M2 e M3 com as chaves hexagonais incluídas no kit.
* Grave o firmware de teste **CoreS3 UserDemo** através do M5Burner para testar isoladamente:
  * Tensão da bateria e do barramento de carga (PMIC AXP2101).
  * Câmara GC0308.
  * Ecrã e resposta tátil.
  * Barramento I2C e sensores IMU.
