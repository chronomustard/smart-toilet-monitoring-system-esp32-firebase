# Smart Toilet Monitoring System using HSC Ultrasonic Sensor, Piezoelectric Sensor, and Firebase

This project aims to develop a smart toilet monitoring system utilizing an HSC ultrasonic sensor, a piezoelectric sensor, and Firebase for real-time data storage and analysis. The system detects and counts flush events and monitors toilet occupancy, providing valuable insights for facility management.

## Components Used

- HSC Ultrasonic Sensor
- Piezoelectric Sensor
- ESP32 microcontroller
- Firebase for real-time data storage and retrieval

## Features

- Flush detection and count using piezoelectric sensor.
- Toilet occupancy monitoring using HSC ultrasonic sensor.
- Real-time data transmission and storage on Firebase.
- Web-based dashboard for monitoring and analysis.

## Installation

1. Clone this repository.
2. Set up the ESP32 development environment using PlatformIO.
3. Install the necessary libraries, including Firebase Arduino Library.

## Firebase Setup

1. Install Firebase CLI by following the instructions [here](https://firebase.google.com/docs/hosting/quickstart).
2. Initialize Firebase in your project: `firebase init`.
3. Log in to Firebase: `firebase login`.
4. Deploy your project to Firebase: `firebase deploy`.

## Usage

1. Connect the HSC ultrasonic sensor and piezoelectric sensor to the ESP32 following the provided schematic.
2. Configure the ESP32 to connect to your Wi-Fi network using PlatformIO.
3. Update the Firebase credentials in the code.
4. Upload the code to the ESP32.
5. Power on the system and monitor toilet usage data on the Firebase dashboard.

## Project Status

This project is a work in progress. Current functionalities include flush detection, occupancy monitoring, and real-time data transmission. Future updates may include additional features and improvements for enhanced functionality and usability.

## Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request.

## License

This project is licensed under [MIT License](link_to_license).