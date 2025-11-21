# PFA

# Project Roadmap: Astro-Math Defender

## Step 0: Toolchain & Hardware Verification
*Start here to ensure your development environment is working.*
- [x] **Set up ESP-IDF:** Install tools and verify `idf.py` runs.
- [x] **Test "Hello World":** Flash the basic example to your ESP32 to confirm the board is alive and communicating.

## Phase 1: AI Model & Voice Recognition 
*First, we prove the "Ear" works using simulation, then real hardware.*

- [x] **Test Baseline Example:** Compile the standard `micro_speech` (Yes/No) example to ensure libraries build correctly.
- [x] **Verify with "Mock" Data (Yes/No):** Modify the code to inject a pre-recorded "Yes" array instead of using a microphone. Confirm the ESP32 detects it.

 ### Step 2: The "Numbers" Upgrade 
*Goal: Replace the "Yes/No" model with your custom "0-9" model, still using mock data.*

- [x] **Dataset:** Get audio files for "One", "Two", "Three"... "Nine".
- [ ] **Train Model:** Run a Python script (Colab) to train a generic 2D CNN on this data.
- [ ] **Quantize & Convert:** Convert the trained model to a `int8` `.tflite` file, then to a C array (`model_data.h`).
- [ ] **Swap Model:** Replace the original `model_data.h` in the ESP project with your new one.
- [ ] **Update Labels:** Update the category labels code to: `{"silence", "unknown", "one", "two"...}`.
- [ ] **Swap Mock Data:** Create a new `one_audio.h` from a recording of "One". Inject *this* file instead of "Yes".
- [ ] **Final Test:** Flash and Monitor.
    *   *Pass Criteria:* The ESP32 logs `Heard one` when fed the "One" audio array.
## Phase 2: Hardware Drivers (Visuals & Input)
*Setting up the physical console interface.*
- [ ] **Enable Microphone:** Connect the I2S Mic, write the driver, and test real-time voice recognition.
- [ ] **TFT Display Setup:** Get the main screen drawing colors and shapes via SPI.
- [ ] **Touch Input Setup:** calibrate the touch sensor so taps register on the correct pixels.
- [ ] **OLED HUD Setup:** Get the secondary I2C screen displaying simple text (Score/Lives).

