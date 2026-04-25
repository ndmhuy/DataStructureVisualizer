#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <optional>

#include "View/UIManager.h"
#include "View/Theme.h"

int main() {
    // Khởi tạo cửa sổ theo chuẩn SFML 3.x
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Test SPA Menu - SFML 3");
    window.setFramerateLimit(60);

    UIManager ui;
    ui.init(window, Theme::getDefaultTheme());
    ui.setShowMainMenu(false); // Bỏ qua Main Menu, vào thẳng Workspace để test

    sf::Clock deltaClock;
    while (window.isOpen()) {
        // Vòng lặp sự kiện chuẩn SFML 3.x sử dụng std::optional
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            ui.processEvent(window, *event);
        }

        // (Tạm ẩn) Giả lập logic của AppEngine
        // int selectedDS = ui.getSelectedDS();
        // if (selectedDS == 5 || selectedDS == 6) {
        //     ui.setShowMainMenu(false);
        // }

        // Nhận lệnh từ UI và in ra console để test 
        int action = ui.getInputAction();
        if (action != 0) {
            std::cout << "[InputMenu Event] Action: " << action 
                      << " | Mode: " << ui.getInputMode() 
                      << " | str1: '" << ui.getInputString1() << "'"
                      << " | str2: '" << ui.getInputString2() << "'"
                      << " | str3: '" << ui.getInputString3() << "'"
                      << " | str4: '" << ui.getInputString4() << "'\n";
            ui.resetInputAction(); // Bắt buộc gọi để đánh dấu đã xử lý
        }

        // Xử lý nút Home để quay lại
        if (ui.checkBackToMenuClicked()) {
            // ui.reset(); // Tạm tắt chức năng Reset để không bị quay lại Main Menu
        }

        ui.update(window, deltaClock.restart());

        window.clear(sf::Color(30, 30, 30));
        ui.render(window);
        window.display();
    }

    ui.shutdown();
    return 0;
}
