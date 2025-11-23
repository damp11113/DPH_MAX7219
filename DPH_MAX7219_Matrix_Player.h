#ifndef MULTI_ANIMATION_PLAYER_H
#define MULTI_ANIMATION_PLAYER_H

#include <Arduino.h>
#include "DPH_MAX7219_Matrix.h"

// Animation metadata structure
struct AnimationInfo {
  const uint16_t* data;      // Pointer to animation data in PROGMEM
  uint16_t totalFrames;      // Total number of frames
  uint16_t dataSize;         // Size of data array
  uint8_t fps;               // Frames per second
  uint16_t frameDelay;       // Delay between frames in ms
  bool loop;                 // Should this animation loop?
  const char* name;          // Animation name (optional)
};

// Transition effects enum
enum TransitionEffect {
  TRANSITION_NONE,
  TRANSITION_FADE,
  TRANSITION_WIPE_LEFT,
  TRANSITION_WIPE_RIGHT,
  TRANSITION_WIPE_UP,
  TRANSITION_WIPE_DOWN
};

class MultiAnimationPlayer {
private:
  MAX7219_LED_MATRIX* display;
  
  // Animation management
  const AnimationInfo* animations;
  uint8_t animationCount;
  uint8_t currentAnimation;
  
  // Playback state
  uint16_t dataIndex;
  uint16_t currentFrame;
  unsigned long lastUpdate;
  bool isPlaying;
  bool isPaused;
  
  // Playlist
  uint8_t* playlist;
  uint8_t playlistSize;
  uint8_t playlistIndex;
  bool playlistLoop;
  
  // Transition
  TransitionEffect transitionEffect;
  uint16_t transitionDuration;
  
  // Helper methods
  void processFrame();
  void applyTransition(uint8_t progress);
  
public:
  MultiAnimationPlayer(MAX7219_LED_MATRIX* disp, 
                      const AnimationInfo* anims, 
                      uint8_t count);
  ~MultiAnimationPlayer();
  
  // Playback control
  void begin();
  void update();
  void play(uint8_t animIndex);
  void pause();
  void resume();
  void stop();
  bool isAnimationPlaying() const { return isPlaying; }
  
  // Animation selection
  void setAnimation(uint8_t animIndex);
  uint8_t getCurrentAnimation() const { return currentAnimation; }
  const char* getCurrentAnimationName() const;
  uint16_t getCurrentFrame() const { return currentFrame; }
  uint16_t getTotalFrames() const;
  
  // Playlist management
  void setPlaylist(uint8_t* list, uint8_t size, bool loop = true);
  void clearPlaylist();
  void nextInPlaylist();
  void prevInPlaylist();
  
  // Transition effects
  void setTransition(TransitionEffect effect, uint16_t duration = 500);
  
  // Utility
  void listAnimations();
  float getProgress() const;
};
#endif // MULTI_ANIMATION_PLAYER_H