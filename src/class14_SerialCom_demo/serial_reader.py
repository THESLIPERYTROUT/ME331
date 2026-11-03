import time
import serial
import matplotlib.pyplot as plt

SERIAL_PORT = 'COM3'  # Change to your port
BAUD_RATE = 9600
LINES_PER_SAMPLE = 4  # Photocell reading, resistance, system time, fade value


def is_resistance(line):
    # Resistance is the only value printed as a float, so it marks our place in
    # the repeating four lines. Arduino prints "inf"/"ovf" when it is out of range.
    return '.' in line or line in ('inf', 'ovf')


def create_plot():
    plt.ion()  # Turn on interactive mode
    _, ax_fade = plt.subplots()
    ax_photo = ax_fade.twinx()  # Second y axis that shares the same x axis

    fade_line, = ax_fade.plot([], [], color='tab:red', label='LED fade value')
    photo_line, = ax_photo.plot([], [], color='tab:blue', label='Photocell reading')

    ax_fade.set_title('LED Fade and Photocell Reading')
    ax_fade.set_xlabel('System time (s)')
    ax_fade.set_ylabel('LED fade value (0-255)', color='tab:red')
    ax_fade.set_ylim(0, 260)
    ax_photo.set_ylabel('Photocell reading (0-1023)', color='tab:blue')
    ax_fade.legend(handles=[fade_line, photo_line], loc='upper left')

    return ax_fade, ax_photo, fade_line, photo_line


def update_plot(plot, times, fade_values, photo_readings):
    ax_fade, ax_photo, fade_line, photo_line = plot
    fade_line.set_data(times, fade_values)
    photo_line.set_data(times, photo_readings)

    # Rescale each axis to fit the new data
    for ax in (ax_fade, ax_photo):
        ax.relim()
        ax.autoscale_view()

    plt.pause(0.1)  # Pause to update the plot


try:
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
        print("Connecting...")
        time.sleep(2)  # Wait for Arduino reset

        plot = create_plot()
        times = []
        fade_values = []
        photo_readings = []
        recent_lines = []

        while True:
            if ser.in_waiting > 0:
                # 1. Read raw bytes: b'452\r\n'
                raw_data = ser.readline()

                try:
                    # 2. Decode and strip:
                    clean_string = raw_data.decode('utf-8').strip()

                    # 3. Keep only the most recent lines
                    recent_lines.append(clean_string)
                    recent_lines = recent_lines[-LINES_PER_SAMPLE:]

                    # 4. Wait until we have a full sample, in the right order
                    if len(recent_lines) < LINES_PER_SAMPLE or not is_resistance(recent_lines[1]):
                        continue

                    # 5. Convert to numbers:
                    photo_reading = int(recent_lines[0])
                    system_time = int(recent_lines[2]) / 1000  # ms to s
                    fade_value = int(recent_lines[3])
                    recent_lines = []

                    print(f"Time: {system_time:.1f} s | Fade: {fade_value} | Photocell: {photo_reading}")

                    times.append(system_time)
                    fade_values.append(fade_value)
                    photo_readings.append(photo_reading)
                    update_plot(plot, times, fade_values, photo_readings)

                except ValueError:
                    # Catches cases where data is partial/corrupted (e.g. on startup)
                    print(f"Skipping malformed data: {raw_data}")
                    recent_lines = []


except serial.SerialException as e:
    print(f"Serial error: {e}")
except KeyboardInterrupt:
    print("\nStopped.")
