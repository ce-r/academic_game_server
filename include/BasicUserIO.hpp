#ifndef BASIC_USER_IO_HPP
#define BASIC_USER_IO_HPP

#include "raylib.h"
#include "raygui.h"
#include <vector>
#include <string>
#include <optional>


constexpr float TITLE_HEIGHT = 40.0f;  // Height of the title bar
constexpr float INPUT_HEIGHT = 30.0f;  // Height of the input box
constexpr float SEND_HEIGHT = 30.0f;   // Height of the send button
constexpr float PADDING = 10.0f;       // Padding between UI elements
constexpr float MIN_EVENT_HEIGHT = 50.0f; // Minimum height for event list
constexpr float MIN_WIDTH = 150.0f;    // Minimum width for UI components

class BasicUserIO {
public:
    BasicUserIO(Rectangle boundary);
    ~BasicUserIO();
    void resize(Rectangle boundary);
    void update();
    void send_event(std::string event);
    std::optional<std::string> get_action();
    void drawBoard(Rectangle rect);

private:
    Rectangle title_rect, event_rect, input_rect, send_rect;
    std::vector<char*> event_strings;
    char input_buff[256];
    int scroll_pos;
    bool send_pressed;
};

#endif