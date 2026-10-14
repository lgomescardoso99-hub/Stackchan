#pragma once

#include <Arduino.h>

enum class FaceEmotion {
    NEUTRAL,
    HAPPY,
    THINKING,
    SURPRISED
};

class AvatarDisplay {
public:
    AvatarDisplay();
    void init();
    void setEmotion(FaceEmotion emotion);
    void setEmotion(const String& emotionStr);
    void showMessage(const String& message);
    void updateAnimation();

private:
    FaceEmotion _currentEmotion = FaceEmotion::NEUTRAL;
    unsigned long _lastBlinkTime = 0;
    bool _isBlinking = false;
    void drawFace();
};
