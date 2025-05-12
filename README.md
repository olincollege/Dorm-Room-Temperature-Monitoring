## Dorm Room Sensor Server

This project is a local and remotely-accessible temperature and humidity monitoring system. It collects real-time data from a Adafruit SHT40 connected to a Raspberry Pi, stores it in an SQLite database, and serves it via a C-based HTTP server. A react frontend displays the latest room conditions to users.

### Features

- Real-time sensor data collection (temperature + humidity)
- SQLite-backed persistent storage
- Local C-based HTTP server using libmicrohttpd
- Dynamic React-based frontend dashboard
- Remote access via ngrok tunnel

### Architecture

- SHT40 wired to GPIO pins of raspi
- Sensor values grabbed using I2C protocol
- Initiliaze database using sqlite3 → database schema includes id, timestamp, temperature, humidity, sensor_id parameters
- `server` (C, libmicrohttpd) → serves `/api/sensor`
- Frontend (React, Vite) → fetches and displays live readings
- Ngrok → exposes local server to the internet

### How to Run

#### Sensor Wiring

Use the Raspberry Pi’s 40-pin header in **BCM** numbering mode:

| Sensor pin | Meaning       | Raspberry Pi pin | BCM number |
| ---------- | ------------- | ---------------- | ---------- |
| VCC        | Power (3.3 V) | Pin 1            | –          |
| GND        | Ground        | Pin 9            | –          |
| SDA        | I²C Data      | Pin 3            | GPIO2      |
| SCL        | I²C Clock     | Pin 5            | GPIO3      |

#### Enable I2C

From a terminal on a Pi:

```bash
sudo raspi-config
```

Navigate to:

**Interface Options --> I2C --> Enable**

Reboot Pi:

```bash
sudo reboot
```

Copy forwarding address and use or share.

#### Dependences

- libmicrohttpd
  Ubuntu: `bash sudo apt install libmicrohttpd-dev `
- sqlite3
  Ubuntu: `bash sudo apt install libsqlite3-dev `

#### Installing Required Packages

```bash
sudo apt update
sudo apt install build-essential libmicrohttpd-dev libsqlite3-dev
```

#### Clone Repo

```bash
git clone https://github.com/olincollege/Dorm-Room-Temperature-Monitoring.git
cd Dorm-Room-Temperature-Monitoring
```

#### Compile

If using make you will have to do it once inside /src and again in /src/Server.

```bash
make
```

You will have to run main in /src and also server in src/Server.

### CMake

Note: CMake is temperamental with this project.

If using CMake:

```bash
cmake -S . -B build && cmake --build build
```

It is recommended you clean the build before rebuilding:

```bash
cmake --fresh -B build
```

Do both of the above commands in project root.

#### Run with CMake

For the front end server:

```bash
./src/http_server
```

You should see:

```
Server listening on port 8080
Press Enter to stop.
```

For the real time data collection:

```bash
./src/sensor_logger
```

You should see sensor outputs in the form:

```
Recorded: 24.48°C, 42.76%
```

#### Run

For the front end server:

```bash
./src/Server/server
```

You should see:

```
Server listening on port 8080
Press Enter to stop.
```

For the real time data collection:

```bash
./src/main
```

You should see sensor outputs in the form:

```
Recorded: 24.48°C, 42.76%
```

#### Tunneling (Optional if you only want to run locally)

```bash
ngrok http 8080
```

##### Note

Each one of these executables will have to be run in their own terminal.

#### View the dashboard

Open a browser to:

```
http://localhost:8080
```

For a light version:

```
http://localhost:8080/api/sensor
```

Or, if sharing through ngrok:

```
https://your-ngrok-url.ngrok-free.app //This will be given by ngrok when you initialize it. Note: Its different every time.
```

Or, to view on terminal:

```bash
curl http://localhost:8080/api/sensor
```

#### Simulation without real sensor input

A sample database has been supplied in the repo located in /src. Note: The next two steps will be run in their own terminals.

1. You can simulate a live feed by running the below:

```bash
gcc src/Server/sensor_feeder.c -lsqlite3 -o src/feeder
./src/Server/feeder
```

2. Run the server:

```bash
cd src/Server
make
./server
```

If ngrok is desired use the same steps as above.

#### Interesting challenges we encountered

1. We got our code for each section of this project done in one function. Refactoring that code became a challenge down the line.
2. Trying to optimize the Database schema to use up as little memory as possible was challenging.
3. Version control got confusing when multiple of us sshed into the Raspberry Pi at the same time.
