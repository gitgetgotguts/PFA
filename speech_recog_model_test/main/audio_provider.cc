#include "audio_provider.h"
#include <cstdlib>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "ringbuf.h"
#include "micro_model_settings.h"

// --- DATA IMPORTS ---
#include "yes_audio_data.h" 
#include "no_audio_data.h"
// --------------------

using namespace std;

static const char* TAG = "TF_LITE_AUDIO_PROVIDER";
ringbuf_t* g_audio_capture_buffer;
volatile int32_t g_latest_audio_timestamp = 0;

constexpr int32_t history_samples_to_keep =
    ((kFeatureDurationMs - kFeatureStrideMs) *
     (kAudioSampleFrequency / 1000));
constexpr int32_t new_samples_to_get =
    (kFeatureStrideMs * (kAudioSampleFrequency / 1000));
const int32_t kAudioCaptureBufferSize = 8000;

namespace {
int16_t g_audio_output_buffer[kMaxAudioSampleSize * 32];
bool g_is_audio_initialized = false;
int16_t g_history_buffer[history_samples_to_keep];
}  // namespace

// --- THE MOCK TASK WITH SWITCHING & STATS ---
static void CaptureSamples(void* arg) {
  ESP_LOGI(TAG, "Starting Mock Audio Capture (Alternating Yes/No)...");

  // 1. Setup Pointers for Switching
  const unsigned char* current_source = yes_raw; // Start with YES
  int total_samples_in_file = yes_raw_len / 2;
  bool is_playing_yes = true;
  
  int playback_index = 0;
  const int chunk_size_samples = 1600; // 100ms

  // Keep static to prevent stack overflow
  static int16_t temp_buffer[chunk_size_samples];

  while (1) {
    // 2. Fill buffer from the *current* source array
    for (int i = 0; i < chunk_size_samples; i++) {
        uint8_t low_byte = current_source[playback_index * 2];
        uint8_t high_byte = current_source[playback_index * 2 + 1];
        temp_buffer[i] = (int16_t)((high_byte << 8) | low_byte);

        playback_index++;

        // 3. End of File Reached? Switch!
                if (playback_index >= total_samples_in_file) {
            playback_index = 0; // Reset index
            
            ESP_LOGI(TAG, "--- SWITCHING SOURCE ---");
            ESP_LOGI(TAG, "RAM: Free Heap: %lu bytes", (unsigned long)esp_get_free_heap_size());

            // === NEW: INJECT SILENCE ===
            // We feed 1 second of silence (zeros) to flush the model's "Memory"
            // This stops the "Yes" from bleeding into the "No"
            int16_t silence_buffer[1600] = {0}; // Chunk of zeros
            
            // Send silence 10 times (10 * 100ms = 1 second)
            for(int s=0; s<10; s++) {
               rb_write(g_audio_capture_buffer, (uint8_t*)silence_buffer, 
                        1600 * sizeof(int16_t), pdMS_TO_TICKS(100));
               
               // Advance the timestamp so the model processes the silence
               g_latest_audio_timestamp += (1600 * 1000) / kAudioSampleFrequency;
               vTaskDelay(pdMS_TO_TICKS(100));
            }
            // ===========================

            // --- TOGGLE AUDIO ---
            if (is_playing_yes) {
                current_source = no_raw;
                total_samples_in_file = no_raw_len / 2;
                is_playing_yes = false;
                ESP_LOGI(TAG, "Now Playing: NO");
            } else {
                current_source = yes_raw;
                total_samples_in_file = yes_raw_len / 2;
                is_playing_yes = true;
                ESP_LOGI(TAG, "Now Playing: YES");
            }
        }
    }

    rb_write(g_audio_capture_buffer, (uint8_t*)temp_buffer, 
             chunk_size_samples * sizeof(int16_t), pdMS_TO_TICKS(100));

    g_latest_audio_timestamp += (chunk_size_samples * 1000) / kAudioSampleFrequency;
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  vTaskDelete(NULL);
}

TfLiteStatus InitAudioRecording() {
  g_audio_capture_buffer = rb_init("tf_ringbuffer", kAudioCaptureBufferSize);
  if (!g_audio_capture_buffer) {
    ESP_LOGE(TAG, "Error creating ring buffer");
    return kTfLiteError;
  }
  // Increased stack size to 8KB just to be safe
  xTaskCreate(CaptureSamples, "CaptureSamples", 1024 * 8, NULL, 10, NULL);
  
  while (!g_latest_audio_timestamp) {
    vTaskDelay(1); 
  }
  return kTfLiteOk;
}

TfLiteStatus GetAudioSamples(int start_ms, int duration_ms,
                             int* audio_samples_size, int16_t** audio_samples) {
  if (!g_is_audio_initialized) {
    InitAudioRecording();
    g_is_audio_initialized = true;
  }
  
  // Standard ringbuffer glue logic
  memcpy((void*)(g_audio_output_buffer), (void*)(g_history_buffer),
         history_samples_to_keep * sizeof(int16_t));

  int bytes_read = rb_read(g_audio_capture_buffer,
              ((uint8_t*)(g_audio_output_buffer + history_samples_to_keep)),
              new_samples_to_get * sizeof(int16_t), pdMS_TO_TICKS(200));

  if (bytes_read < 0) ESP_LOGE(TAG, "Ring Buffer Read Error");

  memcpy((void*)(g_history_buffer),
         (void*)(g_audio_output_buffer + new_samples_to_get),
         history_samples_to_keep * sizeof(int16_t));

  *audio_samples_size = kMaxAudioSampleSize;
  *audio_samples = g_audio_output_buffer;
  return kTfLiteOk;
}

int32_t LatestAudioTimestamp() { return g_latest_audio_timestamp; }