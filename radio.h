#ifndef __RADIO_H_
#define __RADIO_H_

#include <Arduino.h>
#include <vector>
#include <WiFi.h>
#include "src/generated/gui_guider.h"
#include "src/custom/custom.h"

typedef struct RadioInfo {
    const char* name;
    const char* url;
};

class Radio {
public:
    static Radio& GetInstance();
    void Init();
    void PlayStation(size_t index);
    void StopPlay();
    void FillListUI();

    size_t GetCurrentIndex() const { return current_index_; }
    void radio_screen_load(SCREEN_LOAD_STAUS status);
    
private:
    Radio() = default;
    ~Radio() = default;
    Radio(const Radio&) = delete;
    Radio& operator=(const Radio&) = delete;

    size_t current_index_ = 0;
    bool playing_ = false;
    static void ListItemClickHandler(lv_event_t* e);
    
  
};

void radio_win_load(SCREEN_LOAD_STAUS status);

#endif
