# Tank Battle

## 🎮 Giới thiệu

Tank Battle là một trò chơi bắn xe tăng 2D theo phong cách cổ điển được phát triển bằng thư viện SDL2. Người chơi điều khiển xe tăng trong một mê cung, chiến đấu chống lại kẻ địch để giành chiến thắng.

## ✨ Tính năng

- **Hai chế độ chơi**: Chơi đơn hoặc chơi cùng bạn bè (2 người chơi)
- **Xe tăng địch thông minh**: Di chuyển ngẫu nhiên và tấn công người chơi
- **Bản đồ có tường**: Có thể phá hủy để tạo đường đi
- **Hiệu ứng hình ảnh**: Hoạt ảnh nổ cho xe tăng và tường
- **Âm thanh sống động**: Nhạc nền và hiệu ứng âm thanh
- **Giao diện thân thiện**: Menu chọn chế độ chơi và thông báo thắng/thua

## 🔧 Yêu cầu hệ thống

- C++ Compiler (GCC, Visual C++)
- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf

## 📁 Cấu trúc thư mục

```
/
├── main.cpp            # Mã nguồn chính
├── font/               # Thư mục chứa font
│   └── arial.ttf
├── img/                # Thư mục chứa hình ảnh
│   ├── 1.png           # Texture xe tăng người chơi 1
│   ├── 2.png           # Texture xe tăng người chơi 2
│   ├── enemy.png       # Texture xe tăng địch
│   ├── wall.png        # Texture tường
│   ├── explosion.png   # Hiệu ứng nổ xe tăng
│   └── wall_explosion.png # Hiệu ứng nổ tường
└── sound/              # Thư mục chứa âm thanh
    ├── music.mp3       # Nhạc nền
    ├── explosion.wav   # Âm thanh nổ xe tăng
    └── wall_explosion.wav # Âm thanh nổ tường
```

## 🎮 Cách chơi

1. **Khởi động trò chơi**: Chạy tệp thực thi
2. **Menu chính**: Sử dụng phím mũi tên lên/xuống để chọn chế độ chơi
   - 1 PLAYER: Chế độ một người chơi
   - 2 PLAYERS: Chế độ hai người chơi
3. **Bắt đầu chơi**: Nhấn ENTER để bắt đầu
4. **Mục tiêu**: Tiêu diệt tất cả xe tăng địch trước khi bị tiêu diệt

## 🕹️ Điều khiển

### Trong menu:
- **↑ / ↓**: Di chuyển lựa chọn
- **Enter**: Xác nhận lựa chọn

### Trong trò chơi:

#### Người chơi 1:
- **↑**: Di chuyển lên
- **↓**: Di chuyển xuống
- **←**: Di chuyển trái
- **→**: Di chuyển phải
- **Phím 6 (numpad)**: Bắn đạn

#### Người chơi 2 (chỉ trong chế độ 2 người chơi):
- **W**: Di chuyển lên
- **S**: Di chuyển xuống
- **A**: Di chuyển trái
- **D**: Di chuyển phải
- **Space**: Bắn đạn

#### Khác:
- **ESC**: Thoát trò chơi hoặc quay lại menu
- **F5**: Chơi lại (khi game over)

## 🔄 Cơ chế trò chơi

- Xe tăng di chuyển trong mê cung với các bức tường có thể phá hủy
- Bắn đạn để tiêu diệt xe tăng địch và phá vỡ tường
- Xe tăng địch di chuyển ngẫu nhiên và bắn đạn định kỳ
- Thắng khi tiêu diệt tất cả xe tăng địch
- Thua khi xe tăng người chơi bị tiêu diệt
- Trong chế độ 2 người chơi, thua khi cả hai người chơi đều bị tiêu diệt

## 🧩 Cấu trúc mã nguồn

Trò chơi được tổ chức thành các lớp chính:
- **Wall**: Quản lý các bức tường và hiệu ứng nổ
- **Bullet**: Xử lý đạn và di chuyển
- **EnemyTank**: Quản lý AI xe tăng địch
- **PlayerTank**: Xử lý xe tăng người chơi
- **Game**: Điều phối toàn bộ trò chơi, bao gồm render, xử lý sự kiện và logic

## 📝 Tác giả

- Tên tác giả: Tlight21
- Lưu ý: Một số tài nguyên âm thanh và hình ảnh được lấy từ bên ngoài, mọi quyền sở hữu thuộc về các tác giả gốc
---
