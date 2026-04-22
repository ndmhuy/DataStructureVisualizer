#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "View/Slider.h"

void Slider::init(float* value, const Theme& theme) {
    this->theme = theme;
    speed = value;
    isDragging = false;
    trackColor = theme.sliderTrackColor;
    knobColor = theme.sliderKnobColor;
    track.setFillColor(trackColor);
    semiCircle1.setFillColor(trackColor);
    semiCircle2.setFillColor(trackColor);
    knob.setFillColor(knobColor);
    //shown text
    *speed = std::round(10 * (*speed)) / 10;
    if (!font.openFromFile(theme.fontPath)) {
        std::cerr << "Warning: Slider::init failed to load font from '"
                  << theme.fontPath << "'." << std::endl;
        return;
    }
    text.setFont(font);
    text.setFillColor(theme.sliderTextColor);
    // set initial string
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << *speed << "x";
    text.setString(ss.str());
}

void Slider::resize(const sf::RenderWindow& window) {
    //calculate radius, points..
    int x = window.getSize().x;
    int y = window.getSize().y;
    radius = std::round(y / theme.sliderRadiusYDivisor);
    start = theme.sliderStartXRatio * x;
    end = theme.sliderEndXRatio * x;

    //set size
    track.setSize(sf::Vector2f{(end - start), radius * 2});
    semiCircle1.setRadius(radius);
    semiCircle2.setRadius(radius);
    knob.setRadius(radius);
    text.setCharacterSize(radius * 2);

    //set origin
    track.setOrigin(sf::Vector2f{(end - start) / 2.f, radius});
    semiCircle1.setOrigin(sf::Vector2f{radius, radius});
    semiCircle2.setOrigin(sf::Vector2f{radius, radius});
    knob.setOrigin(sf::Vector2f{radius, radius});

    // set origin for text
    sf::FloatRect textRect = text.getLocalBounds();

    text.setOrigin(sf::Vector2f{
        textRect.position.x + textRect.size.x / 2.0f,
        textRect.position.y + textRect.size.y / 2.0f
    });

    //set position
    track.setPosition(sf::Vector2f{theme.sliderCenterXRatio * x, theme.sliderCenterYRatio * y});
    semiCircle1.setPosition(sf::Vector2f{start, theme.sliderCenterYRatio * y});
    semiCircle2.setPosition(sf::Vector2f{end, theme.sliderCenterYRatio * y});
    text.setPosition(sf::Vector2f{theme.sliderTextXRatio * x, theme.sliderCenterYRatio * y});
    // update position of knob based on speed
    knobpos.y = theme.sliderCenterYRatio * y;
    float speedRange = theme.sliderMaxSpeed - theme.sliderMinSpeed;
    knobpos.x = start + (*speed - theme.sliderMinSpeed) / speedRange * (end - start);
    knob.setPosition(knobpos);
}

void Slider::handleEvent(const sf::RenderWindow& window, const sf::Event& event) {
    //when released the mouse
    if (isDragging && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        isDragging = false;
    }
    
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos);
    if (isDragging) {
        if (event.getIf<sf::Event::MouseMoved>()) {
            //logic moving mouse
            knobpos.x = mouseWorldPos.x;
            if (knobpos.x > end) {
                knobpos.x = end;
            } else if (knobpos.x < start) {
                knobpos.x = start;
            }
            //update position of knob
            knob.setPosition(knobpos);
            //logic update speed
            float speedRange = theme.sliderMaxSpeed - theme.sliderMinSpeed;
            *speed = theme.sliderMinSpeed + (knobpos.x - start) / (end - start) * speedRange;
            //round 1 decimal place
            *speed = std::round(10 * (*speed)) / 10;
            update(*speed,text);
        }
    } else {
        float dx = mouseWorldPos.x - knobpos.x;
        float dy = mouseWorldPos.y - knobpos.y;
        if (dx * dx + dy * dy <= radius * radius) {
            const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
                isDragging = true;
            }
        }
    }
}

void Slider::setValue(float val) {
    if (!speed) {
        return;
    }

    const float minSpeed = theme.sliderMinSpeed;
    const float maxSpeed = theme.sliderMaxSpeed;
    *speed = std::clamp(val, minSpeed, maxSpeed);

    // Keep display precision consistent with drag behavior.
    *speed = std::round(10 * (*speed)) / 10;

    float speedRange = theme.sliderMaxSpeed - theme.sliderMinSpeed;
    if (speedRange <= 0.0f || end <= start) {
        update(*speed, text);
        return;
    }

    // Tính toán lại toạ độ x của cục knob dựa trên speed mới
    knobpos.x = start + (*speed - theme.sliderMinSpeed) / speedRange * (end - start);
    knobpos.x = std::clamp(knobpos.x, start, end);
    knob.setPosition(knobpos);
    update(*speed, text); // Cập nhật lại text hiển thị (ví dụ: "1.0x")
}

void Slider::applyTheme(const Theme& selectedTheme) {
    this->theme = selectedTheme;
    trackColor = theme.sliderTrackColor;
    knobColor = theme.sliderKnobColor;
    track.setFillColor(trackColor);
    semiCircle1.setFillColor(trackColor);
    semiCircle2.setFillColor(trackColor);
    knob.setFillColor(knobColor);
    text.setFillColor(theme.sliderTextColor);
}

void Slider::render(sf::RenderWindow& window) {
    window.draw(semiCircle1);
    window.draw(semiCircle2);
    window.draw(track);
    window.draw(knob);
    window.draw(text);
}

// when the speed is changed-> used this to change the text
void update(float speed, sf::Text& text) {
    sf::Vector2f pos = text.getPosition();

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << speed << "x";
    
    text.setString(ss.str());

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f{
        textRect.position.x + textRect.size.x / 2.0f,
        textRect.position.y + textRect.size.y / 2.0f
    });
    text.setPosition(pos);
}