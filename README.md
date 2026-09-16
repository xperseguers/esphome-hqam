# ESPHome Husqvarna Automower® 220 AC and 230 AC

This is a beta ESPHome firmware for the following Husqvarna Automower® models:

- Husqvarna Automower® 220 AC
- Husqvarna Automower® 230 AC

It may be compatible with other models from the same generation.

## Hardware

| Part             | Value                                                   |
| ---------------- | --------------------------------------------------------- |
| Board            | ESP32-DevKitC V4 with WROOM-32U, external u.FL antenna    |
| Power            | 5 V from service port, to board 5V pin                    |
| Fuse             | PTC 0.75 A (RXEF075) in series on 5V line                |
| 5 V decoupling   | 470 µF low ESR + 100 nF in parallel across 5V and GND    |
| 3.3 V decoupling | 10 µF + 100 nF in parallel across 3V3 and GND at module  |
| Connector        | 10-pin Molex KK, pre-crimped cable, unused pins trimmed  |
| UART             | UART2, GPIO16 = RX, GPIO17 = TX, 9600 8N1                |
| Enclosure        | Ventilation holes on two opposite sides                   |

The service port carries Rx, Tx, GND, 5V, 3V3 and 18V, verified with a
multimeter.

Pin numbering is not documented here. The order in the diagram below is
grouped by function and is **not** the pin order. Read pin 1 from the
board's silkscreen — square solder pad, or a one or triangle — and
confirm each conductor with a multimeter before connecting. The 18V
conductor sits on the same connector and will destroy the board if
misconnected.

## Wiring Diagram

```
 Motherboard, service port                          ESP32-DevKitC V4
 (10-pin Molex KK)
                        PTC 0.75 A
      5V  ────────────────[ ~~~ ]───────────┬───────────  5V
                                            │
                                     470 µF ┴ 100 nF
                                            │
      GND ──────────────────────────────────┴───────────  GND
                                                    │
                                            10 µF ──┴── 100 nF
                                            (across 3V3–GND at module)

      Tx  ──────────────────────────────────────────────  GPIO16  (RX)

      Rx  ◄─────────────────────────────────────────────  GPIO17  (TX)

      3V3 ──  not used
      18V ──  trimmed and insulated
```

## Why the components are placed where they are

**5 V instead of 3.3 V.** The board's LDO needs voltage headroom to
regulate. The robot's 3.3 V line is also under-dimensioned.

**PTC fuse on 5V line.** Resettable fuse that protects the robot's
power supply if something shorts in the ESP end. 0.75 A is above the
board's normal consumption including WiFi peaks, but below what the
trace can handle.

**470 µF + 100 nF on 5 V.** The electrolytic handles transmission spikes
when the WiFi radio turns on, so the supply doesn't dip. The ceramic
handles the fast high-frequency content the electrolytic is too slow for.
The cable from the motherboard is long enough to have its own inductance,
making the buffer necessary.

**10 µF + 100 nF on 3.3 V at the module.** Same principle near the
WROOM module, where current spikes occur.

**18V line trimmed.** Not needed and would destroy the board if
misconnected.

**Ventilation holes.** The board sits in an enclosed box on a machine
that runs in the sun.

**UART2, not UART0.** GPIO1/GPIO3 are shared with the USB-serial chip.
The bootloader writes text on TX0 at every boot, which would go straight
into the robot's diagnostic port.

**External u.FL antenna.** The robot is practically a grounded sheet-metal
and plastic box that dampens a short-mounted antenna.

## Configuration

See [`hqam-esphome.yaml`](hqam-esphome.yaml). The device's own yaml
fetches the component and packages from this repo, so only that file is
needed.

`logger:` must be declared explicitly — `components/confs/button.yaml`
uses `logger.log`, and without it validation fails with
`Couldn't find any component that can be used for 'logger::Logger'`.

`wifi.output_power: 12dB` is used in the working configuration. Reduced
transmit power lowers current peaks, which is relevant on a fused supply.

`packages.files` is an explicit list. New files in the repo are not
fetched until added there.

Requires ESPHome 2024.6 or later for the `datetime` platform and
`ota:` list syntax.

## Home Assistant
