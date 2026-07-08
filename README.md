# ESPHome Husqvarna Automower® 220 AC and 230 AC

This is a beta ESPHome firmware for the following Husqvarna Automower® models:

- Husqvarna Automower® 220 AC
- Husqvarna Automower® 230 AC

It may be compatible with other models from the same generation.

## Native Home Assistant lawn mower entity (optional, over MQTT)

ESPHome's native API has **no** lawn mower entity type, so the device cannot
advertise itself as a Home Assistant `lawn_mower` over the API. Home Assistant's
`template` integration also has no `lawn_mower` platform — only the **MQTT**
integration provides the real `lawn_mower` domain.

This firmware therefore offers an **opt-in MQTT bridge**. The native API keeps
serving every sensor, button and select as before; MQTT is used *only* for the
lawn mower entity. Mower status is mapped to MQTT's four accepted activities
(`mowing`, `paused`, `docked`, `error`) and the `start_mowing` / `pause` / `dock`
actions are routed to the mower.

To enable it:

1. Add these to your ESPHome `secrets.yaml`:

   ```yaml
   mqtt_broker: "192.168.x.x"
   mqtt_username: "esphome"
   mqtt_password: "..."
   ```

2. Uncomment `components/confs/lawn_mower_mqtt.yaml` in the `packages:` list of
   your device YAML (see `hqam-esphome.yaml`) and re-flash.

3. Add the Home Assistant side from [`ha/lawn_mower.yaml`](ha/lawn_mower.yaml)
   under `mqtt:` in your `configuration.yaml` (the MQTT integration must be set
   up and pointed at the same broker).

The MQTT topic prefix defaults to `automower`; override the `mqtt_topic_prefix`
substitution if you run more than one mower.
