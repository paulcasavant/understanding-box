
# Understanding Box

**Real-time classroom feedback system using ESP32 + WebSockets**

Understanding Box is an interactive classroom tool designed to help instructors gauge student understanding in real time. Each student has a physical button (powered by an ESP32), which they can press if they are confused. The status is immediately transmitted to a server via WebSockets, and the instructor sees a live display showing the number of students who are struggling.

---

## Overview

- **Student Side**: Each ESP32 device has a button and an LED. When pressed, the device sends a signal to the server indicating the student's current understanding.
- **Server Side**: A Node.js server accepts WebSocket connections from ESP32 clients and a web dashboard. The dashboard displays the aggregate level of understanding in real time.
- **Instructor View**: A responsive web interface shows a visualization of how many students are currently confused.

---

## Technologies Used

### Hardware
- ESP32 microcontrollers
- Physical push button + onboard LED

### Firmware (ESP32)
- Written in C++ using PlatformIO
- Libraries:
  - `WiFi.h`
  - `WebSocketsClient.h`
  - `ArduinoJson.h`

### Backend
- Node.js with WebSocket support (`ws`)
- Express for serving static files
- UUID for client identification

### Frontend
- HTML/CSS (`bootstrap`)
- WebSockets for real-time updates

---

## Project Structure

```
understanding-box/
├── button/                 # ESP32 firmware (PlatformIO)
│   └── src/
│       └── main.cpp        # Button press detection + WebSocket client
├── server/                 # Node.js WebSocket server + frontend
│   ├── src/
|   │   ├── js/
|   │   │   ├── server.js   # WebSocket logic
│   │   └── index.html      # Instructor dashboard
└── README.md
```

---

## Getting Started

### ESP32 Setup

1. Install [PlatformIO](https://platformio.org/) in VSCode.
2. Navigate to the `button/` directory.
3. Update Wi-Fi credentials in `WiFiLogin.h`.
4. Upload the code to your ESP32 using:

```bash
platformio run --target upload
```

### Server Setup

1. Navigate to the `server/` directory.
2. Install dependencies:

```bash
npm install
```

3. Run the server:

```bash
node src/server.js
```

4. Open a browser and go to `http://localhost:8080`.

---

## Features

- Real-time toggle updates from each ESP32 device
- Simple web dashboard for instructors
- Easily scalable for large classrooms
- Visual feedback helps instructors pace their lectures

---