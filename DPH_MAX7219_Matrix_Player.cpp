#include "DPH_MAX7219_Matrix_Player.h"

MultiAnimationPlayer::MultiAnimationPlayer(MAX7219_LED_MATRIX* disp,
                                          const AnimationInfo* anims,
                                          uint8_t count)
  : display(disp),
    animations(anims),
    animationCount(count),
    currentAnimation(0),
    dataIndex(0),
    currentFrame(0),
    lastUpdate(0),
    isPlaying(false),
    isPaused(false),
    playlist(nullptr),
    playlistSize(0),
    playlistIndex(0),
    playlistLoop(true),
    transitionEffect(TRANSITION_NONE),
    transitionDuration(0) {
}

MultiAnimationPlayer::~MultiAnimationPlayer() {
  if (playlist != nullptr) {
    delete[] playlist;
  }
}

void MultiAnimationPlayer::begin() {
  dataIndex = 0;
  currentFrame = 0;
  lastUpdate = millis();
  isPlaying = false;
  isPaused = false;
}

void MultiAnimationPlayer::update() {
  if (!isPlaying || isPaused) return;
  
  unsigned long now = millis();
  const AnimationInfo& anim = animations[currentAnimation];
  
  if (now - lastUpdate < anim.frameDelay) return;
  
  lastUpdate = now;
  processFrame();
  
  currentFrame++;
  
  // Check if animation completed
  if (currentFrame >= anim.totalFrames) {
    if (anim.loop) {
      // Loop current animation
      currentFrame = 0;
      dataIndex = 0;
    } else {
      // Move to next in playlist or stop
      if (playlist != nullptr && playlistSize > 0) {
        nextInPlaylist();
      } else {
        stop();
      }
    }
  }
}

void MultiAnimationPlayer::processFrame() {
  const AnimationInfo& anim = animations[currentAnimation];
  
  // Process all actions for current frame
  while (dataIndex < anim.dataSize) {
    uint16_t frame = pgm_read_word(&anim.data[dataIndex++]);
    
    if (frame > currentFrame) {
      dataIndex--; // Push back
      break;
    }
    
    uint8_t device = pgm_read_word(&anim.data[dataIndex++]);
    uint8_t action = pgm_read_word(&anim.data[dataIndex++]);
    uint8_t count = pgm_read_word(&anim.data[dataIndex++]);
    
    if (action == 0) { // Clear
      display->clearDevice(device);
    } else if (action == 1) { // Intensity
      uint8_t intensity = pgm_read_word(&anim.data[dataIndex++]);
      display->setIntensityDevice(device, intensity);
    } else if (action == 2) { // Pixels (row data)
      for (uint8_t i = 0; i < count; i++) {
        uint8_t row = pgm_read_word(&anim.data[dataIndex++]);
        uint8_t value = pgm_read_word(&anim.data[dataIndex++]);
        display->setRow(device, row, value);
      }
    }
  }
}

void MultiAnimationPlayer::play(uint8_t animIndex) {
  if (animIndex >= animationCount) return;
  
  setAnimation(animIndex);
  isPlaying = true;
  isPaused = false;
  lastUpdate = millis();
}

void MultiAnimationPlayer::pause() {
  isPaused = true;
}

void MultiAnimationPlayer::resume() {
  if (isPlaying) {
    isPaused = false;
    lastUpdate = millis();
  }
}

void MultiAnimationPlayer::stop() {
  isPlaying = false;
  isPaused = false;
  display->clear();
}

void MultiAnimationPlayer::setAnimation(uint8_t animIndex) {
  if (animIndex >= animationCount) return;
  
  currentAnimation = animIndex;
  currentFrame = 0;
  dataIndex = 0;
  display->clear();
}

const char* MultiAnimationPlayer::getCurrentAnimationName() const {
  return animations[currentAnimation].name;
}

uint16_t MultiAnimationPlayer::getTotalFrames() const {
  return animations[currentAnimation].totalFrames;
}

void MultiAnimationPlayer::setPlaylist(uint8_t* list, uint8_t size, bool loop) {
  // Clear old playlist
  if (playlist != nullptr) {
    delete[] playlist;
  }
  
  // Create new playlist
  playlistSize = size;
  playlistLoop = loop;
  playlist = new uint8_t[size];
  
  for (uint8_t i = 0; i < size; i++) {
    playlist[i] = list[i];
  }
  
  playlistIndex = 0;
}

void MultiAnimationPlayer::clearPlaylist() {
  if (playlist != nullptr) {
    delete[] playlist;
    playlist = nullptr;
  }
  playlistSize = 0;
  playlistIndex = 0;
}

void MultiAnimationPlayer::nextInPlaylist() {
  if (playlist == nullptr || playlistSize == 0) return;
  
  playlistIndex++;
  
  if (playlistIndex >= playlistSize) {
    if (playlistLoop) {
      playlistIndex = 0;
    } else {
      stop();
      return;
    }
  }
  
  play(playlist[playlistIndex]);
}

void MultiAnimationPlayer::prevInPlaylist() {
  if (playlist == nullptr || playlistSize == 0) return;
  
  if (playlistIndex == 0) {
    if (playlistLoop) {
      playlistIndex = playlistSize - 1;
    } else {
      return;
    }
  } else {
    playlistIndex--;
  }
  
  play(playlist[playlistIndex]);
}

void MultiAnimationPlayer::setTransition(TransitionEffect effect, uint16_t duration) {
  transitionEffect = effect;
  transitionDuration = duration;
}

void MultiAnimationPlayer::listAnimations() {
  Serial.println("=== Available Animations ===");
  for (uint8_t i = 0; i < animationCount; i++) {
    Serial.print(i);
    Serial.print(": ");
    if (animations[i].name != nullptr) {
      Serial.print(animations[i].name);
    } else {
      Serial.print("Animation ");
      Serial.print(i);
    }
    Serial.print(" (");
    Serial.print(animations[i].totalFrames);
    Serial.print(" frames @ ");
    Serial.print(animations[i].fps);
    Serial.println(" fps)");
  }
}

float MultiAnimationPlayer::getProgress() const {
  if (!isPlaying) return 0.0;
  const AnimationInfo& anim = animations[currentAnimation];
  return (float)currentFrame / (float)anim.totalFrames;
}