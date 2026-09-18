import serial
import pygame
import time

# ==========================================
# SETTINGS
# ==========================================

COM_PORT = "COM16"
BAUD_RATE = 115200
SONG_FILE = "horror.mp3"

# ==========================================
# SERIAL
# ==========================================

ser = serial.Serial(
    COM_PORT,
    BAUD_RATE,
    timeout=1
)

time.sleep(2)

# ==========================================
# AUDIO
# ==========================================

pygame.mixer.init()
pygame.mixer.music.load(SONG_FILE)

playing = False

print("================================")
print("   HAUNTED BOX AUDIO SYSTEM")
print("================================")
print("Waiting for ESP32...")
print()

# ==========================================
# MAIN LOOP
# ==========================================

while True:

    if ser.in_waiting:

        data = ser.readline().decode(
            "utf-8",
            errors="ignore"
        ).strip()

        print(data)

        # ------------------------------
        # PLAY HORROR SOUND
        # ------------------------------

        if data == "PLAY":

            if not playing:

                print(">>> HORROR SOUND PLAYING!")

                pygame.mixer.music.play()

                playing = True

        # ------------------------------
        # STOP HORROR SOUND
        # ------------------------------

        elif data == "STOP":

            if playing:

                print(">>> HORROR SOUND STOP")

                pygame.mixer.music.stop()

                playing = False