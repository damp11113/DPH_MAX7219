/*
 * Example: Using MultiAnimationPlayer with multiple animations
 * 
 * This example shows how to:
 * 1. Store multiple animations
 * 2. Create playlists
 * 3. Switch between animations
 * 4. Use interactive controls
 */

#include "DPH_MAX7219_Matrix.h"
#include "DPH_MAX7219_Matrix_Player.h"

// Include your exported animation headers
#include "animation_smile.h"     // Generated from your animator
#include "animation_heart.h"     // Generated from your animator
#include "animation_wave.h"      // Generated from your animator
#include "animation_spinner.h"   // Generated from your animator

MAX7219_LED_MATRIX display(0, 2, 1, 1);

// Define animation collection
// Note: Each animation header should export its data as shown below
const AnimationInfo animations[] = {
  {
    .data = animation_smile_data,
    .totalFrames = SMILE_TOTAL_FRAMES,
    .dataSize = SMILE_DATA_SIZE,
    .fps = SMILE_FPS,
    .frameDelay = SMILE_FRAME_DELAY,
    .loop = true,
    .name = "Smile"
  },
  {
    .data = animation_heart_data,
    .totalFrames = HEART_TOTAL_FRAMES,
    .dataSize = HEART_DATA_SIZE,
    .fps = HEART_FPS,
    .frameDelay = HEART_FRAME_DELAY,
    .loop = true,
    .name = "Heart Beat"
  },
  {
    .data = animation_wave_data,
    .totalFrames = WAVE_TOTAL_FRAMES,
    .dataSize = WAVE_DATA_SIZE,
    .fps = WAVE_FPS,
    .frameDelay = WAVE_FRAME_DELAY,
    .loop = false,
    .name = "Wave"
  },
  {
    .data = animation_spinner_data,
    .totalFrames = SPINNER_TOTAL_FRAMES,
    .dataSize = SPINNER_DATA_SIZE,
    .fps = SPINNER_FPS,
    .frameDelay = SPINNER_FRAME_DELAY,
    .loop = true,
    .name = "Spinner"
  }
};

const uint8_t ANIMATION_COUNT = sizeof(animations) / sizeof(AnimationInfo);

MultiAnimationPlayer player(&display, animations, ANIMATION_COUNT);

void setup() {
  Serial.begin(115200);
  
  display.begin();
  display.clear();
  display.setIntensity(8);
  
  player.begin();
  
  // List available animations
  player.listAnimations();
  
  // Example 1: Play a single animation
  Serial.println("\n=== Playing single animation ===");
  player.play(0); // Play "Smile"
  
  // Example 2: Create a playlist
  // Uncomment to use playlist instead:
  /*
  uint8_t myPlaylist[] = {0, 1, 2, 3}; // Smile -> Heart -> Wave -> Spinner
  player.setPlaylist(myPlaylist, 4, true); // Loop the playlist
  player.play(myPlaylist[0]); // Start playlist
  Serial.println("\n=== Playing playlist ===");
  */
}

void loop() {
  // Update animation player
  player.update();
  
  // Serial control example
  if (Serial.available()) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case '0':
      case '1':
      case '2':
      case '3':
        // Play animation by number
        player.play(cmd - '0');
        Serial.print("Playing: ");
        Serial.println(player.getCurrentAnimationName());
        break;
        
      case 'p':
        // Pause/Resume
        if (player.isAnimationPlaying()) {
          player.pause();
          Serial.println("Paused");
        } else {
          player.resume();
          Serial.println("Resumed");
        }
        break;
        
      case 's':
        // Stop
        player.stop();
        Serial.println("Stopped");
        break;
        
      case 'n':
        // Next in playlist
        player.nextInPlaylist();
        Serial.print("Next: ");
        Serial.println(player.getCurrentAnimationName());
        break;
        
      case 'b':
        // Previous in playlist
        player.prevInPlaylist();
        Serial.print("Previous: ");
        Serial.println(player.getCurrentAnimationName());
        break;
        
      case 'l':
        // List animations
        player.listAnimations();
        break;
        
      case 'i':
        // Show info
        Serial.print("Current: ");
        Serial.print(player.getCurrentAnimationName());
        Serial.print(" - Frame ");
        Serial.print(player.getCurrentFrame());
        Serial.print("/");
        Serial.print(player.getTotalFrames());
        Serial.print(" (");
        Serial.print(player.getProgress() * 100, 1);
        Serial.println("%)");
        break;
    }
  }
  
  // Button control example (if you have buttons)
  /*
  static unsigned long lastButtonCheck = 0;
  if (millis() - lastButtonCheck > 50) { // Debounce
    lastButtonCheck = millis();
    
    if (digitalRead(BUTTON_NEXT) == LOW) {
      player.nextInPlaylist();
    }
    if (digitalRead(BUTTON_PREV) == LOW) {
      player.prevInPlaylist();
    }
    if (digitalRead(BUTTON_PAUSE) == LOW) {
      if (player.isAnimationPlaying()) {
        player.pause();
      } else {
        player.resume();
      }
    }
  }
  */
}

/*
 * ADVANCED EXAMPLES
 */

// Example: Random animation player
void playRandomAnimation() {
  uint8_t randomAnim = random(0, ANIMATION_COUNT);
  player.play(randomAnim);
}

// Example: Playlist with custom sequence
void setupCustomPlaylist() {
  // Play animations in custom order
  uint8_t customOrder[] = {3, 1, 0, 2, 1}; // Spinner, Heart, Smile, Wave, Heart
  player.setPlaylist(customOrder, 5, true);
  player.play(customOrder[0]);
}

// Example: Auto-switch every N seconds
unsigned long lastSwitch = 0;
const unsigned long SWITCH_INTERVAL = 10000; // 10 seconds

void autoSwitchAnimations() {
  if (millis() - lastSwitch > SWITCH_INTERVAL) {
    lastSwitch = millis();
    player.nextInPlaylist();
  }
}

// Example: Play animation based on sensor input
void sensorTriggeredAnimation() {
  int sensorValue = analogRead(A0);
  
  if (sensorValue < 256) {
    player.play(0);
  } else if (sensorValue < 512) {
    player.play(1);
  } else if (sensorValue < 768) {
    player.play(2);
  } else {
    player.play(3);
  }
}