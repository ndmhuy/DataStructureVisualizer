# Technical Documentation: UIManager API Specification

Tài liệu này cung cấp các đặc tả kỹ thuật chi tiết về lớp `UIManager`, bao gồm các phương thức giao tiếp và luồng hoạt động (workflow) tiêu chuẩn để phối hợp với `AppEngine` trong dự án DataStructureVisualizer.

---

## 1. Nhóm phương thức Vòng đời Cơ bản (Core Lifecycle)

Các phương thức này quản lý quá trình khởi tạo, cập nhật và giải phóng tài nguyên của giao diện người dùng (ImGui/SFML).

### 1.1. Khởi tạo

- **`bool init(sf::RenderWindow& window, const Theme& theme)`**  
  Thiết lập hệ thống UI với chủ đề mặc định.  
  Gọi ngay sau khi tạo RenderWindow.

---

### 1.2. Xử lý Sự kiện & Cập nhật

- **`void processEvent(sf::RenderWindow& window, const sf::Event& event)`**  
  Tiếp nhận các thao tác chuột và bàn phím.  
  Đặt ở đầu vòng lặp sự kiện.

- **`void resize(const sf::RenderWindow& window)`**  
  Điều chỉnh kích thước các nút bấm và thanh trượt khi cửa sổ thay đổi kích cỡ.
  Gọi trong vòng lặp sự kiện (nếu có sự kiện resize)

- **`void update(sf::RenderWindow& window, const sf::Time& deltaTime)`**  
  Cập nhật logic frame cho ImGui.  
  Truyền vào `deltaTime` từ đồng hồ hệ thống.

---

### 1.3. Kết xuất & Giải phóng

- **`void render(sf::RenderWindow& window)`**  
  Vẽ giao diện lên màn hình.  
  Đặt giữa `window.clear()` và `window.display()`.

- **`void shutdown()`**  
  Giải phóng toàn bộ tài nguyên.  
  Gọi trước khi thoát ứng dụng.

---
**📌 Mã giả Vòng đời tiêu chuẩn:**
```cpp
// 1. Khởi tạo
sf::RenderWindow window(sf::VideoMode({1280, 720}), "Visualizer");
UIManager ui;
ui.init(window, Theme::getDefaultTheme());

sf::Clock deltaClock;
while (window.isOpen()) {
    // 2. Xử lý sự kiện
    sf::Event event;
    while (window.pollEvent(event)) {
        ui.processEvent(window, event);
    }
    
    // 3. Cập nhật và Kết xuất
    ui.update(window, deltaClock.restart());
    window.clear();
    ui.render(window);
    window.display();
}
ui.shutdown(); // 4. Dọn dẹp
```

---

## 2. Nhóm phương thức Quản lý Chủ đề (Theme Management)

- **`bool applyTheme(const Theme& selectedTheme)`**  
  Áp dụng bộ màu sắc và cấu hình mới.

- **`bool consumeThemeToggleRequest()`**  
  Kiểm tra yêu cầu chuyển đổi Light/Dark mode.

- **`const Theme& getTheme() const`**  
  Truy xuất tham chiếu đến chủ đề hiện tại.

---

## 3. Nhóm phương thức Trình đơn Điều hướng (Navigation Menu)

- **`int getSelectedDS() const`**  
  Trả về ID cấu trúc dữ liệu đang chọn  
  `(-1: None, 0: SLL, 1: Heap, 2: AVL, 3: SPA)`

- **`void resetDSSelection()`**  
  Khôi phục cờ chọn về `-1` để tránh lỗi lặp lệnh.

- **`void setShowMainMenu(bool show)`**  
  Chuyển đổi hiển thị giữa Menu chính và Workspace.
  true: MainMenu
  false: Menu làm việc

- **`bool checkBackToMenuClicked()`**  
  Kiểm tra nếu người dùng nhấn nút quay lại Menu chính.

- **`void reset()`**  
  Khôi phục toàn bộ trạng thái UI (Navigation Menu ,Slider, InputMenu, CodePanel, Playback) về mặc định và quay lại Main Menu. Cần gọi khi bắt được sự kiện `checkBackToMenuClicked`.

---
**📌 Mã giả Xử lý Điều hướng:**
```cpp
// 1. Khi đang ở Main Menu
if (ui.getSelectedDS() != -1) {
    // a. Thiết lập dữ liệu cho Data Structure mới tương ứng
    ui.setShowMainMenu(false);   // b. Chuyển giao diện sang Workspace
    // ...
}

// 2. Khi đang ở trong vòng lặp xử lý Workspace Data Structure
if (ui.checkBackToMenuClicked()) {
    // a. Xóa/Giải phóng dữ liệu của Data Structure hiện tại
    ui.reset();                  // b. Khôi phục toàn bộ UI về trạng thái mặc định
    // ...
}

## 4. Nhóm phương thức Trình đơn Nhập liệu (Input Menu)

Dùng để trích xuất lệnh và dữ liệu từ bảng điều khiển. Giao diện và các lệnh trả về (Action, Mode, Strings) sẽ thay đổi tùy thuộc vào Data Structure (DS) đang được chọn.

### Các phương thức lấy dữ liệu

- `int getInputAction() const;`: Lấy mã lệnh (Action ID). Trả về 0 nếu chưa có lệnh nào.
- `int getInputMode() const;`: Lấy chế độ của lệnh (Ví dụ: Nhập từ mảng, random, file,...).
- `std::string getInputString1() const;`: Lấy tham số chuỗi 1.
- `std::string getInputString2() const;`: Lấy tham số chuỗi 2.
- `std::string getInputString3() const;`: Lấy tham số chuỗi 3.
- `std::string getInputString4() const;`: Lấy tham số chuỗi 4.
- `void resetInputAction();`: **Bắt buộc gọi** sau khi xử lý xong lệnh để tránh lặp lệnh.

### Bảng Ánh xạ Tham số Nhập liệu theo Cấu trúc dữ liệu

#### 1. SINGLY LINKED LIST (0)
| Action ID | Chức năng     | Mode | Chi tiết Mode | String 1                              | String 2         | String 3      | String 4 |
| :-------: | :------------ | :--: | :------------ | :------------------------------------ | :--------------- | :------------ | :------- |
| **1**     | **Init**      |  0   | Array         | Chuỗi số (cách nhau bởi khoảng trắng) | *N/A*            | *N/A*         | *N/A*    |
| **1**     | **Init**      |  1   | Random        | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |
| **1**     | **Init**      |  2   | File          | Đường dẫn file                        | *N/A*            | *N/A*         | *N/A*    |
| **2**     | **Insert**    |  0   | Single        | Giá trị node                          | *N/A*            | *N/A*         | *N/A*    |
| **2**     | **Insert**    |  1   | Random        | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |
| **3**     | **Search**    |  0   | -             | Giá trị cần tìm                       | *N/A*            | *N/A*         | *N/A*    |
| **4**     | **Delete**    |  0   | -             | Giá trị cần xoá                       | *N/A*            | *N/A*         | *N/A*    |
| **5**     | **Update**    |  0   | -             | Giá trị cũ (From)                     | Giá trị mới (To) | *N/A*         | *N/A*    |
| **6**     | **Clear**     |  0   | Confirm       | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |

#### 2. MIN HEAP (1) & MAX HEAP (2)
| Action ID | Chức năng     | Mode | Chi tiết Mode | String 1                              | String 2         | String 3      | String 4 |
| :-------: | :------------ | :--: | :------------ | :------------------------------------ | :--------------- | :------------ | :------- |
| **1**     | **Init**      |  0   | Array         | Chuỗi số (cách nhau bởi khoảng trắng) | *N/A*            | *N/A*         | *N/A*    |
| **1**     | **Init**      |  1   | Random        | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |
| **1**     | **Init**      |  2   | File          | Đường dẫn file                        | *N/A*            | *N/A*         | *N/A*    |
| **2**     | **Insert**    |  0   | Single        | Giá trị node                          | *N/A*            | *N/A*         | *N/A*    |
| **2**     | **Insert**    |  1   | Random        | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |
| **3**     |**ExtractTop** |  0   | -             | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |
| **4**     | **Peek**      |  0   | -             | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |
| **5**     | **Search**    |  0   | -             | Giá trị cần tìm                       | *N/A*            | *N/A*         | *N/A*    |
| **6**     | **Delete**    |  0   | -             | Giá trị cần xoá                       | *N/A*            | *N/A*         | *N/A*    |
| **7**     | **Update**    |  0   | -             | Giá trị cũ (From)                     | Giá trị mới (To) | *N/A*         | *N/A*    |
| **8**     | **Clear**     |  0   | Confirm       | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |

#### 3. AVL TREE (3)
| Action ID | Chức năng     | Mode | Chi tiết Mode | String 1                              | String 2         | String 3      | String 4 |
| :-------: | :------------ | :--: | :------------ | :------------------------------------ | :--------------- | :------------ | :------- |
| **1**     | **Init**      |  0   | Array         | Chuỗi số (cách nhau bởi khoảng trắng) | *N/A*            | *N/A*         | *N/A*    |
| **1**     | **Init**      |  1   | Random        | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |
| **1**     | **Init**      |  2   | File          | Đường dẫn file                        | *N/A*            | *N/A*         | *N/A*    |
| **2**     | **Insert**    |  0   | Single        | Giá trị node                          | *N/A*            | *N/A*         | *N/A*    |
| **2**     | **Insert**    |  1   | Random        | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |
| **3**     | **Search**    |  0   | -             | Giá trị cần tìm                       | *N/A*            | *N/A*         | *N/A*    |
| **4**     | **Delete**    |  0   | -             | Giá trị cần xoá                       | *N/A*            | *N/A*         | *N/A*    |
| **5**     | **Update**    |  0   | -             | Giá trị cũ (From)                     | Giá trị mới (To) | *N/A*         | *N/A*    |
| **6**     | **Clear**     |  0   | Confirm       | *N/A*                                 | *N/A*            | *N/A*         | *N/A*    |

#### 4. GRID (4)
| Action ID | Chức năng         | Mode | Chi tiết Mode | String 1       | String 2 | String 3 | String 4 |
| :-------: | :---------------- | :--: | :------------ | :------------- | :------- | :------- | :------- |
| **1**     | **Init**          |  0   | Empty         | N (Số hàng)    |M (Số cột)| *N/A*    | *N/A*    |
| **1**     | **Init**          |  1   | File          | Đường dẫn file | *N/A*    | *N/A*    | *N/A*    |
| **2**     | **Random**        |  0   | -             | N              | M        | *N/A*    | *N/A*    |
| **3**     | **Set status**    |  0   | Empty         | Tọa độ i       | Tọa độ j | *N/A*    | *N/A*    |
| **3**     | **Set status**    |  1   | Wall          | Tọa độ i       | Tọa độ j | *N/A*    | *N/A*    |
| **4**     | **Shortest Path** |  0   | A*            | x1 (Từ)        | y1       | x2 (Đến) | y2       |
| **4**     | **Shortest Path** |  1   | BFS           | x1 (Từ)        | y1       | x2 (Đến) | y2       |
| **5**     | **Clear**         |  0   | Confirm       | *N/A*          | *N/A*    | *N/A*    | *N/A*    |

#### 5. SHORTEST PATH ALGORITHM: DIRECTED GRAPH (5)
| Action ID | Chức năng       | Mode | Chi tiết Mode | String 1               | String 2    | String 3       | String 4 |
| :-------: | :-------------- | :--: | :------------ | :--------------------- | :---------- | :------------- | :------- |
| **1**     | **Init**        |  0   | Adj Matrix    | Đường dẫn file         | *N/A*       | *N/A*          | *N/A*    |
| **1**     | **Init**        |  1   | Adj List      | Đường dẫn file         | *N/A*       | *N/A*          | *N/A*    |
| **2**     | **Create Node** |  0   | -             | *N/A*                  | *N/A*       | *N/A*          | *N/A*    |
| **3**     | **Create Edge** |  0   | -             | Đỉnh u                 | Đỉnh v      | Trọng số (u,v) | *N/A*    |
| **4**     | **OPSP**        |  0   | -             | Đỉnh u (From)          | Đỉnh v (To) | *N/A*          | *N/A*    |
| **5**     | **SPSP**        |  0   | DAG           | Đỉnh bắt đầu           | *N/A        | *N/A*          | *N/A*    |
| **5**     | **SPSP**        |  1   | Dijkstra      | Đỉnh bắt đầu           | *N/A        | *N/A*          | *N/A*    |
| **5**     | **SPSP**        |  2   | Bellman       | Đỉnh bắt đầu           | *N/A        | *N/A*          | *N/A*    |
| **6**     | **APSP**        |  0   | Floyd         | *N/A*                  | *N/A*       | *N/A*          | *N/A*    |
| **6**     | **APSP**        |  1   | Johnson       | *N/A*                  | *N/A*       | *N/A*          | *N/A*    |
| **7**     | **Clear**       |  0   | Confirm       | *N/A*                  | *N/A*       | *N/A*          | *N/A*    |
| **8**     | **Random**      |  0   | DAG           | min Weight             | max Weight  | *N/A*          | *N/A*    |
| **8**     | **Random**      |  1   | Graph         | min Weight             | max Weight  | *N/A*          | *N/A*    |
UNDIRECTED GRAPH(6)
| Action ID | Chức năng       | Mode | Chi tiết Mode | String 1               | String 2    | String 3       | String 4 |
| :-------: | :-------------- | :--: | :------------ | :--------------------- | :---------- | :------------- | :------- |
| **1**     | **Init**        |  0   | Adj Matrix    | Đường dẫn file         | *N/A*       | *N/A*          | *N/A*    |
| **1**     | **Init**        |  1   | Adj List      | Đường dẫn file         | *N/A*       | *N/A*          | *N/A*    |
| **2**     | **Create Node** |  0   | -             | *N/A*                  | *N/A*       | *N/A*          | *N/A*    |
| **3**     | **Create Edge** |  0   | -             | Đỉnh u                 | Đỉnh v      | Trọng số (u,v) | *N/A*    |
| **4**     | **OPSP**        |  0   | -             | Đỉnh u (From)          | Đỉnh v (To) | *N/A*          | *N/A*    |
| **5**     | **SPSP**        |  0   | Dijkstra      | Đỉnh bắt đầu           | *N/A        | *N/A*          | *N/A*    |
| **5**     | **SPSP**        |  1   | Bellman       | Đỉnh bắt đầu           | *N/A        | *N/A*          | *N/A*    |
| **6**     | **APSP**        |  0   | Floyd         | *N/A*                  | *N/A*       | *N/A*          | *N/A*    |
| **6**     | **APSP**        |  1   | Johnson       | *N/A*                  | *N/A*       | *N/A*          | *N/A*    |
| **7**     | **Clear**       |  0   | Confirm       | *N/A*                  | *N/A*       | *N/A*          | *N/A*    |
| **8**     | **Random**      |  0   | Graph         | min Weight             | max Weight  | *N/A*          | *N/A*    |

**📌 Mã giả Xử lý Nhập liệu:**
```cpp
int action = ui.getInputAction();
if (action != 0) { // Nếu người dùng vừa nhấn OK/Confirm một thao tác
    int mode = ui.getInputMode();
    std::string str1 = ui.getInputString1();
    
    if (ui.getSelectedDS() == 0) { // Đang ở Workspace của Singly Linked List
        if (action == 1) { // Lệnh Init
            if (mode == 0) initFromArray(str1); // Khởi tạo từ chuỗi
            else if (mode == 2) initFromFile(str1); // Khởi tạo từ file
        }
        else if (action == 2) { // Lệnh Insert
            insertNode(std::stoi(str1));
        }
    }
    
    ui.resetInputAction(); // BẮT BUỘC: Đánh dấu đã xử lý xong lệnh để không bị lặp
}
```


## 5. Nhóm phương thức Bảng Mã giả (Code Panel)

- **`void setCodePanelCode(std::vector<std::string>& code)`**  
  Nạp mã giả thuật toán.

- **`void setCodePanelHighlightedLine(int line)`**  
  Bôi đậm dòng mã đang thực thi (0-based).

- **`void clearCodePanel()`**  
  Làm sạch bảng mã.

---
**📌 Mã giả Tương tác Mã giả:**
```cpp
// Khi bắt đầu chạy một thuật toán
std::vector<std::string> code = {
    "Node* newNode = new Node(v);",
    "if (head == nullptr) head = newNode;",
    "else { ... }"
};
ui.setCodePanelCode(code); // Đẩy mã lên màn hình

// Trong lúc chạy Playback/Animation
if (currentStep == 0) {
    ui.setCodePanelHighlightedLine(0); // Bôi sáng dòng "Node* newNode = new Node(v);"
}

// Kết thúc sử dụng mã giả
ui.clearCodePanel();
```

---

## 6. Nhóm phương thức Điều khiển Mô phỏng (Playback Controls)

- `checkPlayClicked()` / `checkPauseClicked()`  
  Kiểm tra tương tác nút chạy/dừng.

- `checkStepForwardClicked()` / `checkStepBackwardClicked()`  
  Kiểm tra tương tác bước tới/lui.

- **`void syncPlaybackUI(bool currentIsPlaying, bool isAtBeginning, bool isAtEnd, bool isEmpty)`**  
  Đồng bộ trạng thái mờ/sáng của các nút dựa trên logic của `AppEngine`.
  currentIsPlaying: đang chạy/ dừng
  isAtBeginning: đang ở đầu list playback không?
  isAtEnd: đang ở cuối list playback không?
  isEmpty: list playback có đang rỗng không?
  -> Gọi trong mỗi vòng lặp (sau khi kiểm tra tương tác 4 lệnh trên) để hiển thị nút

---
**📌 Mã giả Xử lý Playback:**
```cpp
// 1. Bắt sự kiện người dùng bấm nút ở frame hiện tại
if (ui.checkPlayClicked()) isPlaying = true;
if (ui.checkPauseClicked()) isPlaying = false;
if (ui.checkStepForwardClicked()) goNextStep();

// 2. Thông báo trạng thái Playback của AppEngine cho UIManager (Gọi liên tục)
bool isAtStart = (currentStep == 0);
bool isAtEnd = (currentStep == totalSteps - 1);
bool isEmpty = (totalSteps == 0);
ui.syncPlaybackUI(isPlaying, isAtStart, isAtEnd, isEmpty);
```

---

## 7. Thanh trượt & Tiện ích

- **`float getSpeed() const`**  
  Lấy hệ số tốc độ `(0.5 - 4.0)`.

- **`void resetSpeed()`**  
  Khôi phục tốc độ mặc định `(1.0f)`.
  Gọi sau khi hoàn thành một DS

---

## 8. Đặc tả Lớp Thành phần: Button

Lớp `Button` quản lý logic tương tác vật lý (Hover, Click) và đồ họa:

- Tự động thay đổi màu sắc dựa trên trạng thái:
  `Normal`, `Hover`, `Pressed`, `Inactive`
- Cơ chế dự phòng (Fallback) nếu thiếu file hình ảnh

---

## 9. Hướng dẫn Tích hợp (AppEngine Pattern)

Sử dụng mô hình đồng bộ trạng thái (**Declarative Sync**) thay vì ra lệnh rời rạc:

1. `AppEngine` cập nhật trạng thái hoạt ảnh  
2. `AppEngine` gọi `ui.syncPlaybackUI()` truyền các cờ trạng thái  
3. `UIManager` tự động cập nhật giao diện hiển thị cho người dùng

---