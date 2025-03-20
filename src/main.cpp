

#include <iostream>       // Standard input/output
#include <thread>         // For threading
#include <optional>       // For std::optional
#include <filesystem>     // File system operations
#include <spdlog/spdlog.h> // Logging library
#include <raylib.h>       // Raylib graphics library
#include <fmt/core.h>
#include "BasicUserIO.hpp" // UI class
#include "GameServerDTLS.hpp" // Game server logic
#include "GameClientDTLS.hpp" // Game client logic


const uint32_t WINDOW_W = 800;
const uint32_t WINDOW_H = 450;

int main(int argc, char* argv[]) {
    // Logging Configuration
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%H:%M:%S %z] [thread %t]%$ %v");
    SetTraceLogLevel(LOG_ERROR);

    // Game server initialization (if no arguments)
    std::optional<GameServerDTLS> server;
    std::optional<std::thread> server_thread;

    if (argc == 1) {
        server.emplace("server.crt", "server.key", AF_INET, IPPROTO_UDP, 5555, INADDR_ANY, 10);
        spdlog::trace("{}: Starting game server", std::filesystem::path(__FILE__).filename().string());
        // server_thread.emplace(&GameServerDTLS::handle_dtls_game, &server.value());
        server_thread.emplace([&]() {
            SSL_CTX* dtls_ctx = nullptr; // Ensure this is properly initialized
            int udp_sock = server->get_udp_socket(); // Assuming you have a method to get the UDP socket
            sockaddr_in client_addr = {}; // Initialize client address structure
        
            server->handle_dtls_game(dtls_ctx, udp_sock, client_addr);
        });
    }

    // Initialize DTLS game client
    GameClientDTLS client(AF_INET, IPPROTO_UDP, 5555, INADDR_LOOPBACK);
   
    // Raylib initialization
    InitWindow(WINDOW_W, WINDOW_H, "Cluedorks");
    SetTargetFPS(60);

    // GUI layout
    Rectangle basic_io_rect{0, 0, static_cast<float>(WINDOW_W) / 3, WINDOW_H};
    Rectangle board_rect{basic_io_rect.x + basic_io_rect.width, 0, 2 * static_cast<float>(WINDOW_W) / 3, WINDOW_H};
   
    BasicUserIO user_io(basic_io_rect);

    // Main Event Loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        user_io.drawBoard(board_rect);
        user_io.update();
        EndDrawing();

        // Handle user input and send actions via DTLS
        auto action = user_io.get_action();
        if (action) {
            client.send_game_action(*action);
        }

        // Receive and display server updates
        if (client.pending_message()) {
            std::string server_message = client.receive_message();
            user_io.send_event(fmt::format("From server: {}", server_message));
        }

        // Handle window resizing
        if (IsWindowResized()) {
            float width = GetScreenWidth();
            float height = GetScreenHeight();
            basic_io_rect = Rectangle{10, 10, static_cast<float>(width) / 3, height};
            board_rect = Rectangle{basic_io_rect.x + basic_io_rect.width, 10, 2 * static_cast<float>(width) / 3, height};
            user_io.resize(basic_io_rect);
        }
    }

    // Cleanup
    CloseWindow();
    if (server.has_value()) {
        server->set_running(false);
        server_thread->join();
    }

    return 0;
}