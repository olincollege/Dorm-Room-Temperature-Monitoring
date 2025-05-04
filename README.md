## Dorm Room Sensor Server

This project is a local and remotely-accessible temperature and humidity monitoring system. It collects real-time data from a Adafruit SHT40 connected to a Raspberry Pi, stores it in an SQLite database, and serves it via a C-based HTTP server. A react frontend displays the latest room conditions to users.

### Features

- Real-time sensor data collection (temperature + humidity)
- SQLite-backed persistent storage
- Local C-based HTTP server using libmicrohttpd
- Dynamic React-based frontend dashboard
- Remote access via ngrok tunnel

### Architecture

- TODO explain sensor stuff
- TODO explain DB stuff
- `server` (C, libmicrohttpd) → serves `/api/sensor`
- Frontend (React, Vite) → fetches and displays live readings
- Ngrok → exposes local server to the internet

### How to Run

#### Server

```bash
make
./server
```

Using curl in terminal or a webrowser enter http://localhost:8080

### Tunneling (Optional if you only want to run locally)

```bash
ngrok http 8080
```

Copy forwarding address and use or share.

### Dependences

- libmicrohttpd
  Ubuntu: `bash sudo apt install libmicrohttpd-dev `
- sqlite3
  Ubuntu: `bash sudo apt install libsqlite3-dev `

### Installing Required Packages

```bash
sudo apt update
sudo apt install build-essential libmicrohttpd-dev libsqlite3-dev
```

### Clone Repo

```bash
git clone https://github.com/olincollege/Dorm-Room-Temperature-Monitoring.git
cd Dorm-Room-Temperature-Monitoring
```

### Compile

```bash
make
```

### Run

```bash
./server
```

You should see

```
Server listening on port 8080
Press Enter to stop.
```

### View the dashboard

Open a browser to:

```
http://localhost:8080
```

Or, if sharing through ngrok:

```
https://your-ngrok-url.ngrok-free.app
```
