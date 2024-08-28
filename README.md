# MQTT Rotary Sensor

## Project Overview
The MQTT Rotary Sensor is a versatile project designed to integrate a rotary encoder with an ESP32 microcontroller and an OLED display, providing an intuitive interface for controlling various smart home devices via Home Assistant. The rotary encoder can be used to adjust parameters such as brightness, color, and temperature of lights or other devices, with all changes communicated over MQTT to Home Assistant.

## Features
- **Rotary Encoder Control:** Utilize a rotary encoder with a built-in push button to navigate through a menu and select various smart home devices or lighting options.
- **OLED Display:** Provides a clear and concise visual interface to display the current selection and status of the controlled device.
- **MQTT Communication:** Seamlessly integrates with Home Assistant via MQTT, allowing real-time control and feedback for automations and smart home devices.
- **Wi-Fi Configuration Portal:** In the event of a Wi-Fi disconnection or initial setup, the device automatically enters Access Point mode, allowing users to easily connect and configure Wi-Fi settings through a user-friendly web interface.
- **Menu Navigation:** Navigate through different control options (e.g., adjust brightness, color, or temperature) using the rotary encoder, with each selection displayed on the OLED screen.
- **Robust Error Handling:** Includes logic to handle Wi-Fi disconnections and retries, ensuring that the device remains operational and reconfigurable even in unstable network conditions.
- **Home Assistant Automations:** Easily set up automations in Home Assistant to respond to changes in the MQTT messages sent by the rotary encoder, such as adjusting light brightness or color based on user input.
  
## Images

![IMG_2995](images/IMG_2995.png)
*IMG_2995.png*

![IMG_2996](images/IMG_2996.png)
*IMG_2996.png*

![IMG_2997](images/IMG_2997.png)
*IMG_2997.png*

![IMG_2998](images/IMG_2998.png)
*IMG_2998.png*


## Components Used
- **ESP32 Microcontroller:** The brain of the operation, handling Wi-Fi connectivity, MQTT communication, and rotary encoder input.
- **Rotary Encoder:** Provides user input through rotation and button press, used for selecting and adjusting smart home settings.
- **0.96" OLED Display:** Displays the current menu options and settings, allowing for a clear and interactive user experience.
- **MQTT Protocol:** Used to communicate with Home Assistant, sending updates whenever the user adjusts a setting.

## Getting Started
1. **Hardware Setup:** Connect the rotary encoder and OLED display to the ESP32 according to the specified pin configurations.
2. **Upload the Code:** Flash the provided Arduino code to the ESP32 using the Arduino IDE or another compatible platform.
3. **Configure Wi-Fi:** On the first boot or when the Wi-Fi connection is lost, the ESP32 will enter AP mode. Connect to the `ESP32_Config` network and navigate to `192.168.4.1` in your web browser to configure Wi-Fi.
4. **Integrate with Home Assistant:** Set up MQTT sensors and automations in Home Assistant to interact with the rotary encoder, enabling control over your smart home devices.

## Example Home Assistant Automations
Below are examples of how to use the MQTT messages sent by the rotary encoder to control lights or other devices in Home Assistant.


Certainly! Here's the formatted section of the README.md file for "Adjust Lamp Color" and the rest:

markdown
Copy code
## Example Home Assistant Automations


**Adjust Lamp Brightness:**
```yaml
alias: Adjust Lamp Brightness
trigger:
  - platform: state
    entity_id: sensor.smart_rotary_switch_rotary_encoder_brightness
action:
  - service: light.turn_on
    target:
      entity_id: light.lamp
    data:
      brightness_pct: "{{ states('sensor.smart_rotary_switch_rotary_encoder_brightness') | int * 4 }}"
```

**Adjust Lamp Color:**
```yaml
alias: Adjust Lamp Color
trigger:
  - platform: state
    entity_id: sensor.smart_rotary_switch_rotary_encoder_color
action:
  - service: light.turn_on
    target:
      entity_id: light.lamp
    data:
      hs_color: "{{ states('sensor.smart_rotary_switch_rotary_encoder_color') | int }}"
```

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing
Contributions are welcome! Feel free to submit issues or pull requests to improve this project.

## Acknowledgments
Special thanks to the Home Assistant community for providing inspiration and support. This project was developed using the Arduino framework and various open-source libraries for ESP32 and MQTT.


