# Chase-the-Flag Game

Welcome to the Chase-the-Flag game, a simple multiplayer game implemented in C++ using WebSocket++.

## Gameplay

- **Objective:** Be the first to capture the flag in your room!
- Each room hosts a separate game.
- At the start, one player is randomly selected as the target player.
- The first player to reach the same location as the target player wins the round.
- After a player wins, the target player is randomly changed, and the game continues.

## Getting Started

### Prerequisites

- C++17 compiler.
- WebSocket++ library.

### Installation

1. **Clone the repository:**

   ```sh
   git clone https://github.com/your-username/ChaseTheFlagGame.git
2. **Build and run the game:**

   ```sh
   cd chase-the-flag
   mkdir build && cd build
   cmake ..
   make
3. **Run the server:**

   ```sh
   cd server
   ./Server
4. **Run the client:**

   ```sh
   cd client
   ./Client
