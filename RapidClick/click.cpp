#include "click.h"

#include <thread>
#include <mutex>

#include <QDebug>

#include <Windows.h>
#include "winuser.h"

namespace {
  enum ClickerMode {
    dead = 0
  , paused
  , hold
  , toggle
  };

  constexpr INPUT keyboard_up() {
    INPUT result = {0};
    result.type = INPUT_KEYBOARD;
    result.ki.dwFlags = KEYEVENTF_KEYUP;
    return result;
  }
  constexpr INPUT keyboard_down() {
    INPUT result = {0};
    result.type = INPUT_KEYBOARD;
    result.ki.dwFlags = KEYEVENTF_KEYUP;
    return result;
  }
  constexpr INPUT mouse_up() {
    INPUT result = {0};
    result.type = INPUT_MOUSE;
    result.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    return result;
  }
  constexpr INPUT mouse_down() {
    INPUT result = {0};
    result.type = INPUT_MOUSE;
    result.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    return result;
  }
};

class Clicker::Data {

  std::thread monitor;

  std::mutex lock;

  bool update = true;
  ClickerMode mode = ClickerMode::paused;
  char activate = VK_LEFT;
  char output = VK_LBUTTON;
  
  void run() {
    INPUT up = {0};
    INPUT down = {0};

    ClickerMode current_mode;
    char current_activate;
    char current_output;

    bool toggle = false;
    bool held = false;

    while(true) {
      {
        const std::lock_guard<std::mutex> guard(lock);
        if(update) {
          qDebug() << "Updated";
          if(mode == ClickerMode::dead)
            break;

          current_mode = mode;
          current_activate = activate;
          current_output = output;

          if(current_output == VK_LBUTTON || current_output == VK_RBUTTON) {
            up = mouse_up();
            down = mouse_down();
            if(current_output == VK_RBUTTON) {
              up.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
              down.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            }
          }
          else {
            up = keyboard_up();
            down = keyboard_down();
            up.ki.wVk = current_output;
            down.ki.wVk = current_output;
          }
          update = false;
        }
      }

      if(current_mode == ClickerMode::paused) {
        toggle = false;
        held = false;
      }
      else {
        const bool pressed = (GetKeyState(current_activate) & 0x80) != 0;
        if(!pressed && held)
          qDebug() << "Button Up";
        if(pressed && !held) {
          qDebug() << "Button Down";
          toggle = !toggle;
        }
        held = pressed;
        

        if( (current_mode == ClickerMode::hold && held) 
          ||(current_mode == ClickerMode::toggle && toggle) ){

          SendInput(1,&down,sizeof(INPUT));
          SendInput(1,&up,sizeof(INPUT));
        }
      }

      Sleep(50);
    }
    qDebug() << "Killed";
  }
  

public:
  Data() {
    monitor = std::move(std::thread(&Data::run,this));
  }
  ~Data() {
    setMode(ClickerMode::dead);
    monitor.join(); 
  }

  void setActivate(char key) {
    const std::lock_guard<std::mutex> guard(lock);
    activate = key;
    update = true;
  }
  void setOutput(char key) {
    const std::lock_guard<std::mutex> guard(lock);
    output = key;
    update = true;
  }
  void setMode(ClickerMode _mode) {
    const std::lock_guard<std::mutex> guard(lock);
    mode = _mode;
    update = true;
  }

  bool isEnabled() {
    const std::lock_guard<std::mutex> guard(lock);
    return mode == ClickerMode::hold || mode == ClickerMode::toggle;
  }
};



Clicker::Clicker() 
: data( std::make_unique<Data>() ){
}


Clicker::~Clicker() {
}

void Clicker::setActivate(int key) {
  d().setActivate(key);
}
void Clicker::setOutput(int key) {
  d().setOutput(key);
}
void Clicker::pause() {
  d().setMode(ClickerMode::paused);
}
void Clicker::enableHold() {
  d().setMode(ClickerMode::hold);
}
void Clicker::enableToggle() {
  d().setMode(ClickerMode::toggle);
}

bool Clicker::isEnabled() {
  return d().isEnabled();
}