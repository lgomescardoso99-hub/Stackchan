#include "avatar_display.h"
#include <M5StackChan.h>

AvatarDisplay::AvatarDisplay() {}

void AvatarDisplay::init() {
    auto& d = M5StackChan.Display();
    d.setRotation(1);
    d.fillScreen(TFT_BLACK);
    d.setTextWrap(true);
    drawFace();
}

void AvatarDisplay::setEmotion(FaceEmotion emotion) {
    _currentEmotion = emotion;
    drawFace();
}

void AvatarDisplay::setEmotion(const String& emotionStr) {
    if (emotionStr == "HAPPY") {
        setEmotion(FaceEmotion::HAPPY);
    } else if (emotionStr == "THINKING") {
        setEmotion(FaceEmotion::THINKING);
    } else if (emotionStr == "SURPRISED") {
        setEmotion(FaceEmotion::SURPRISED);
    } else {
        setEmotion(FaceEmotion::NEUTRAL);
    }
}

void AvatarDisplay::drawFace() {
    auto& d = M5StackChan.Display();
    d.startWrite();
    d.fillRect(0, 0, 320, 160, TFT_BLACK); // Limpa área superior dos olhos

    const int eyeY = 80;
    const int eyeLeftX = 100;
    const int eyeRightX = 220;

    switch (_currentEmotion) {
        case FaceEmotion::HAPPY:
            // Olhos felizes em arco (^ ^)
            d.drawArc(eyeLeftX, eyeY + 10, 25, 20, 200, 340, TFT_GREEN);
            d.drawArc(eyeRightX, eyeY + 10, 25, 20, 200, 340, TFT_GREEN);
            // Boca a sorrir
            d.fillCircle(160, 130, 14, TFT_RED);
            break;

        case FaceEmotion::THINKING:
            // Olhos a olhar para o canto superior direito
            d.fillCircle(eyeLeftX + 8, eyeY - 8, 20, TFT_CYAN);
            d.fillCircle(eyeRightX + 8, eyeY - 8, 20, TFT_CYAN);
            d.fillCircle(eyeLeftX + 14, eyeY - 12, 8, TFT_WHITE);
            d.fillCircle(eyeRightX + 14, eyeY - 12, 8, TFT_WHITE);
            // Boca pensativa reta
            d.fillRect(145, 130, 30, 6, TFT_WHITE);
            break;

        case FaceEmotion::SURPRISED:
            // Olhos grandes arregalados (O O)
            d.fillCircle(eyeLeftX, eyeY, 28, TFT_YELLOW);
            d.fillCircle(eyeRightX, eyeY, 28, TFT_YELLOW);
            d.fillCircle(eyeLeftX, eyeY, 12, TFT_BLACK);
            d.fillCircle(eyeRightX, eyeY, 12, TFT_BLACK);
            // Boca redonda aberta
            d.fillCircle(160, 135, 18, TFT_RED);
            break;

        case FaceEmotion::NEUTRAL:
        default:
            if (_isBlinking) {
                // Linha de pestana a piscar
                d.fillRect(eyeLeftX - 22, eyeY, 44, 6, TFT_CYAN);
                d.fillRect(eyeRightX - 22, eyeY, 44, 6, TFT_CYAN);
            } else {
                // Olhos ovais fofos
                d.fillEllipse(eyeLeftX, eyeY, 22, 32, TFT_CYAN);
                d.fillEllipse(eyeRightX, eyeY, 22, 32, TFT_CYAN);
                d.fillCircle(eyeLeftX + 6, eyeY - 8, 8, TFT_WHITE);
                d.fillCircle(eyeRightX + 6, eyeY - 8, 8, TFT_WHITE);
            }
            // Boca pequena neutra
            d.fillCircle(160, 130, 8, TFT_WHITE);
            break;
    }
    d.endWrite();
}

void AvatarDisplay::showMessage(const String& message) {
    auto& d = M5StackChan.Display();
    d.startWrite();
    // Limpa a área de legendas inferior
    d.fillRect(0, 160, 320, 80, 0x18C3); // Fundo cinzento escuro
    d.drawRect(0, 160, 320, 80, TFT_CYAN);
    d.setTextColor(TFT_WHITE);
    d.setTextSize(2);
    d.setCursor(8, 170);
    d.print(message);
    d.endWrite();
}

void AvatarDisplay::updateAnimation() {
    // Piscar de olhos a cada ~4 segundos em modo neutro
    if (_currentEmotion == FaceEmotion::NEUTRAL) {
        unsigned long now = millis();
        if (!_isBlinking && (now - _lastBlinkTime > 3800)) {
            _isBlinking = true;
            _lastBlinkTime = now;
            drawFace();
        } else if (_isBlinking && (now - _lastBlinkTime > 180)) {
            _isBlinking = false;
            _lastBlinkTime = now;
            drawFace();
        }
    }
}
