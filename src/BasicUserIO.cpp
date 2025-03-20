
#include <iostream>
#include <thread>
#include <optional>
#include <filesystem>
#include <raylib.h>
#include <spdlog/spdlog.h>
#include "raygui.h"
#include "BasicUserIO.hpp"
#include "GameServerDTLS.hpp"
#include "GameClientDTLS.hpp"


BasicUserIO::BasicUserIO(Rectangle boundary) {
    input_buff[0] = '\0';
    scroll_pos = 0;
    send_pressed = false;
    resize(boundary);
}

BasicUserIO::~BasicUserIO() {
    for (const auto& c_str : event_strings) {
        delete[] c_str;
    }
}

void BasicUserIO::resize(Rectangle boundary) {
    const float event_height = boundary.height - TITLE_HEIGHT - INPUT_HEIGHT - SEND_HEIGHT - 5 * PADDING;
    const float width = boundary.width - 2 * PADDING;

    if (event_height >= MIN_EVENT_HEIGHT && width > MIN_WIDTH) {
        title_rect = Rectangle{ PADDING, PADDING, width, TITLE_HEIGHT };
        event_rect = Rectangle{ PADDING, title_rect.y + title_rect.height + PADDING, width, event_height };
        input_rect = Rectangle{ PADDING, event_rect.y + event_rect.height + PADDING, width, INPUT_HEIGHT };
        send_rect = Rectangle{ PADDING, input_rect.y + input_rect.height + PADDING, width, SEND_HEIGHT };
    } else {
        std::cerr << "Warning: Window size too small for UI components" << std::endl;
    }
}

void BasicUserIO::update() {
    DrawText("Cluedorks", static_cast<int>(title_rect.x), static_cast<int>(title_rect.y), static_cast<int>(title_rect.height), BLACK);
    GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    // GuiListViewEx(event_rect, event_strings.data(), event_strings.size(), &scroll_pos, nullptr, nullptr);
    GuiListViewEx(event_rect, const_cast<const char**>(event_strings.data()), event_strings.size(), &scroll_pos, nullptr, nullptr);
    GuiTextBox(input_rect, input_buff, sizeof(input_buff), !send_pressed);
    send_pressed |= GuiButton(send_rect, "Send");
}

void BasicUserIO::send_event(std::string event) {
    char* copy = new char[event.length() + 1];
    std::strcpy(copy, event.c_str());
    event_strings.insert(event_strings.begin(), copy);
    scroll_pos = 0;
}

std::optional<std::string> BasicUserIO::get_action() {
    if (send_pressed) {
        send_pressed = false;
        return std::string(input_buff);
    }
    return std::nullopt;
}


void BasicUserIO::drawBoard(Rectangle rect){
    if (rect.width < rect.height) {
      // warning might not draw correctly
    }
    const uint32_t BOARD_CELLS = 5;
    const uint32_t cell_size_px = rect.height / (BOARD_CELLS + 1);
    const uint32_t board_size = BOARD_CELLS * cell_size_px;
    const uint32_t offset_x = rect.x + (rect.width - board_size) / 2;
    const uint32_t offset_y = rect.y + (rect.height - board_size) / 2;
 
    for (uint32_t i = 0; i < BOARD_CELLS; ++i) {
      for (uint32_t j = 0; j < BOARD_CELLS; ++j) {
        Color color = (i % 2 == 0 && j % 2 == 0)  //
                        ? GRAY
                        : (i == j || (BOARD_CELLS - 1 - i) == (j))  //
                            ? BLACK
                            : LIGHTGRAY;
        DrawRectangle(
          offset_x + i * cell_size_px,
          offset_y + j * cell_size_px,
          cell_size_px,
          cell_size_px,
          color
        );
      }
    }
    for (uint32_t i = 0; i < BOARD_CELLS; ++i) {
      DrawLine(
        offset_x + cell_size_px * i,
        offset_y,
        offset_x + cell_size_px * i,
        offset_y + board_size,
        BLACK
      );
      DrawLine(
        offset_x,
        offset_y + cell_size_px * i,
        offset_x + board_size,
        offset_y + cell_size_px * i,
        BLACK
      );
    }
    DrawLine(
      offset_x + board_size, offset_y, offset_x + board_size, offset_y + board_size, BLACK
    );
    DrawLine(
      offset_x, offset_y + board_size, offset_x + board_size, offset_y + board_size, BLACK
    );
  }
