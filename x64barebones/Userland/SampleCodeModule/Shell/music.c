#include "include/music.h"
#include "../Library/include/unistd1.h"

static void playHimnoArgentino(){
    // Oid
    beep(392, 500);  // G4
    beep(392, 500);  // G4
    
    // mortales
    beep(349, 250);  // F4
    beep(330, 250);  // E4
    beep(294, 250);  // D4
    beep(330, 250);  // E4
    
    // el grito
    beep(349, 500);  // F4
    beep(392, 500);  // G4
    beep(440, 500);  // A4
    
    // sagrado
    beep(392, 500);  // G4
    beep(349, 500);  // F4
    beep(330, 500);  // E4
}

static void startUpMusic(){
    beep(1318,250);
    beep(1245,250);
    beep(1318,250);
    beep(1245,250);
    beep(1318,250);
    beep(987,250);
    beep(1175,250);
    beep(1046,250);
    beep(880,500);
}

static void mario(){
    // Mario Theme snippet
    beep(660, 100);  // E5
    beep(660, 100);  // E5
    sleep(100);
    beep(660, 100);  // E5
    sleep(100);
    beep(523, 100);  // C5
    beep(660, 100);  // E5
    sleep(100);
    beep(784, 100);  // G5
    sleep(300);
    beep(392, 100);  // G4
    // Repeat for a loop
}

static void playHappyBirthday(){
    // Feliz
    beep(264, 250);  // C4
    beep(264, 250);  // C4
    beep(297, 500);  // D4
    beep(264, 500);  // C4
    beep(352, 500);  // F4
    beep(330, 1000); // E4
    
    // Cumpleaños
    beep(264, 250);  // C4
    beep(264, 250);  // C4
    beep(297, 500);  // D4
    beep(264, 500);  // C4
    beep(396, 500);  // G4
    beep(352, 1000); // F4
    
    // Feliz
    beep(264, 250);  // C4
    beep(264, 250);  // C4
    beep(528, 500);  // C5
    beep(440, 500);  // A4
    beep(352, 500);  // F4
    beep(330, 500);  // E4
    beep(297, 500);  // D4
    
    // Cumpleaños
    beep(466, 250);  // A#4
    beep(466, 250);  // A#4
    beep(440, 500);  // A4
    beep(352, 500);  // F4
    beep(396, 500);  // G4
    beep(352, 1000); // F4
}

static void playArrorroMiNino(){
    // Arrorró
    beep(262, 500);  // C4
    beep(294, 500);  // D4
    beep(330, 500);  // E4
    beep(262, 1000); // C4
    
    // Mi niño
    beep(262, 500);  // C4
    beep(294, 500);  // D4
    beep(330, 500);  // E4
    beep(262, 1000); // C4
    
    // Arrorró
    beep(330, 500);  // E4
    beep(349, 500);  // F4
    beep(392, 500);  // G4
    beep(330, 1000); // E4
    
    // Y duérmete
    beep(330, 500);  // E4
    beep(349, 500);  // F4
    beep(392, 500);  // G4
    beep(330, 1000); // E4
    
    // Mi niño
    beep(294, 500);  // D4
    beep(262, 500);  // C4
    beep(294, 500);  // D4
    beep(262, 1000); // C4
}

static void playSmokeOnTheWater(){
    int short_duration = 500; // Duración corta en milisegundos
    int long_duration = 1000; // Duración larga en milisegundos
    
    // Smoke on the Water riff
    beep(392, short_duration);  // G4
    beep(440, short_duration);  // A4
    beep(494, short_duration);  // B4
    beep(392, short_duration);  // G4
    beep(392, short_duration);  // G4
    beep(440, short_duration);  // A4
    beep(523, short_duration);  // C5
    beep(494, long_duration);   // B4
    
    // Repeat
    beep(392, short_duration);  // G4
    beep(440, short_duration);  // A4
    beep(494, short_duration);  // B4
    beep(392, short_duration);  // G4
    beep(392, short_duration);  // G4
    beep(440, short_duration);  // A4
    beep(523, short_duration);  // C5
    beep(494, long_duration);   // B4
    
    // Additional part
    beep(392, short_duration);  // G4
    beep(440, short_duration);  // A4
    beep(494, short_duration);  // B4
    beep(392, short_duration);  // G4
    beep(392, short_duration);  // G4
    beep(440, short_duration);  // A4
    beep(494, short_duration);  // B4
    beep(523, long_duration);   // C5
    beep(392, long_duration);   // G4
}

static void playSevenNationArmy(){
    int duration = 400; // Duración en milisegundos

    // Seven Nation Army riff
    beep(247, duration);  // B3
    beep(247, duration);  // B3
    beep(294, duration);  // D4
    beep(247, duration);  // B3
    beep(220, duration);  // A3
    beep(196, duration);  // G3
    beep(247, duration);  // B3
    beep(196, duration * 2);  // G3
    
    // Repeat
    beep(247, duration);  // B3
    beep(247, duration);  // B3
    beep(294, duration);  // D4
    beep(247, duration);  // B3
    beep(220, duration);  // A3
    beep(196, duration);  // G3
    beep(247, duration);  // B3
    beep(196, duration * 2);  // G3
}


static void gameMusic(){
    beep(523, 250); // C5
    beep(587, 250); // D5
    beep(659, 250); // E5
    beep(523, 250); // C5
    beep(784, 500); // G5
    beep(523, 500); // C5
    sleep(250); // Silencio
    beep(659, 250); // E5
    beep(784, 250); // G5
    beep(880, 500); // A5
    beep(784, 500); // G5
    sleep(250); // Silencio
    beep(659, 250); // E5
    beep(698, 250); // F5
    beep(784, 250); // G5
    beep(523, 250); // C5
}

static void (*songsArray[])() = {
    playHimnoArgentino,
    startUpMusic, 
    mario, 
    playHappyBirthday, 
    playArrorroMiNino, 
    playSmokeOnTheWater,
    playSevenNationArmy,   
    gameMusic
    };

void musicDispatcher(int song){
    if (song >= sizeof(songsArray) / sizeof(songsArray[0])){
            return;
    }
        
    songsArray[song]();
}