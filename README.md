# ESPHome Husqvarna Automower® 220 AC and 230 AC

This is a beta ESPHome firmware for the following Husqvarna Automower® models:

- Husqvarna Automower® 220 AC
- Husqvarna Automower® 230 AC

It may be compatible with other models from the same generation.

## Hardware

### Minimal setup (fine enough)

A simple ESP8266 board wired directly to the service port works fine.
One user has run this for over a year without issues using:

| Part             | Value                                  |
| ---------------- | -------------------------------------- |
| Board            | D1 Mini (ESP8266)                      |
| Mower RX pin     | GPIO2 (D4)                             |
| Mower TX pin     | GPIO0 (D3)                             |
| GPS RX pin       | GPIO3 (D0)                             |
| Power            | 5 V from service port, to board 5V pin |
| GND              | Common ground                          |

No capacitors, no PTC fuse, no level shifters. Just three wires to the
robot and one wire to the GPS module.

## Wiring Diagrams

### Minimal (D1 Mini)

```
 Service port                              D1 Mini
      5V  ────────────────────────────────────  5V
      GND ────────────────────────────────────  GND
      Tx  ────────────────────────────────────  D3 (GPIO0)  [RX]
      Rx  ◄───────────────────────────────────  D4 (GPIO2)  [TX]

      3V3 ──  not used
      18V ──  not connected

 GPS module (NEO-6M)                          D1 Mini
      VCC ────────────────────────────────────  5V (or 3.3V)
      GND ────────────────────────────────────  GND
      TX  ────────────────────────────────────  D0 (GPIO3)  [RX]
```

## Branch "GPS"

This branch adds support for a GPS module to loosely track the position of the
Automower® in your garden.

### GPS hardware

Successfully tested with a NEO-6M GPS Module GY-NEO6MV2.

| GPS pin | Connection                              |
| ------- | --------------------------------------- |
| VCC     | 5 V (or 3.3 V — the module accepts both) |
| GND     | Common ground                           |
| TX      | ESP8266 RX pin (e.g. GPIO3 / D0)       |
| RX      | not connected                           |

The GPS module transmits NMEA sentences on its TX line at 9600 baud.
The ESP8266 receives on a second UART (`uart_gps`). Only the RX pin is
needed — the GPS RX pin is left unconnected.

### GPS configuration

The GPS package (`components/confs/gps.yaml`) exposes the following sensors:

- GPS Latitude
- GPS Longitude
- GPS Altitude
- GPS Speed
- GPS Satellites
- GPS Course
- GPS HDOP

Time is sourced from the GPS module instead of Home Assistant:

```yaml
time:
  - platform: gps
    id: gps_time
```

The GPS update interval defaults to 1500 ms. Adjust in `gps.yaml` if needed.

### ESP8266 pin notes

On ESP8266 the second UART can use software serial. The GPS RX pin is
configurable via the `gps_rxpin` substitution variable. Common choices:

- GPIO3 (D0) — free on most D1 Mini layouts
- GPIO15 — available but has boot-mode implications; use with caution
