## Phase 3: Game Logic Core
*Building the "Astro-Math" gameplay loop (without voice initially).*
- [ ] **Math Engine:** Create logic to generate random equations (e.g., "2 + 3").
- [ ] **Asteroid Physics:** Make objects spawn at the top and fall to the bottom.
- [ ] **Targeting System:** Implement touch logic to "Select/Lock-on" to a falling asteroid.

## Phase 4: System Integration
*Combining the AI and Game loops.*
- [ ] **Multicore Setup:** Assign Game Logic to Core 1 and AI Inference to Core 0.
- [ ] **Data Piping:** Create a queue so the AI can send the number it heard ("5") to the Game Logic to destroy the locked asteroid.
- [ ] **Performance Tuning:** Ensure the game runs smoothly while the AI is listening.

## Phase 5: Polish & Experience
*Making it a real game.*
- [ ] **Visual Effects:** Add laser beams and explosion animations.
- [ ] **Game States:** Add Start Menu, Game Over screen, and Restart functionality.
- [ ] **HUD Logic:** Update score and health on the OLED screen.

## Phase 6: Future Scalability
*Adding the multiplayer aspect.*
- [ ] **ESP-NOW Integration:** Implement direct communication between two devices.
- [ ] **Duel Logic:** Create the mechanism to send attacks to the opponent.
