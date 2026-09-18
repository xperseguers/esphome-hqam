# Feasibility Analysis: Native Home Assistant Integration for Husqvarna Automower

> **Date:** 2026-09-18
> **Author:** Analysis based on current `main` branch (commit `5286a9a`)
> **Reference:** Previous attempt at MQTT-based integration — commit `6c87105`
> **Discussion:** https://community.home-assistant.io/t/tutorial-on-how-to-use-the-ha-lawn-mower-integration/1000736

---

## 1. Executive Summary

**Verdict: A truly native ESPHome → Home Assistant `lawn_mower` entity over the native API is currently not feasible.**

ESPHome's native API does not support a `lawn_mower` entity type. Home Assistant's `template` integration also lacks a `lawn_mower` platform. The **only** built-in path to a real `lawn_mower` entity in HA is through the **MQTT** integration.

This document analyses the situation, reviews the previous MQTT approach, and proposes alternative strategies — including a **template-based workaround** that avoids MQTT entirely — so you can choose the best trade-off for your setup.

---

## 2. Background: The Previous MQTT Approach (Commit 6c87105)

In July 2026, an opt-in MQTT bridge was created (`components/confs/lawn_mower_mqtt.yaml` + `ha/lawn_mower.yaml`) with the following design:

### ESPHome Side
- Minimal MQTT config with `discovery: false` (to avoid duplicate entities)
- Birth/last-will messages for availability
- `on_message` lambda that routes HA commands (`start_mowing`, `pause`, `dock`) to the mower's `set_mode()` / `set_stop()` methods
- Periodic (10s) retained publish of the mower's activity state, mapped to MQTT's four accepted values: `mowing`, `paused`, `docked`, `error`

### Home Assistant Side
- MQTT `lawn_mower` platform configuration
- Activity state topic, command topics, availability topic
- Device metadata (manufacturer, model)

### Why It Was Removed
The files were deleted from the repo after the commit. The stated motivation is that **MQTT is complex to set up** — requiring a broker, credentials, network configuration on both sides, and matching topic prefixes.

---

## 3. Current State of the Codebase (Main Branch)

### What Exists Today
| Component | Description |
|-----------|-------------|
| `mower::Automower` (C++) | Custom ESPHome component, communicates via UART (9600 baud) with the Husqvarna service port |
| Sensors | Battery level/temp/voltage/used/capacity, blade motor speed, charging/mowing/cutting time, loop quality, wheel speeds, firmware version |
| Text Sensors | Mode, status, status message, last code received |
| Buttons | Key 0-9, Key A-C, Key HOME, Key MAN/AUTO, Key Cancel, Key Up, Key Down, Key Yes, Stop, Reset Stop |
| Selects | Mode selection (AUTO, MAN, etc.) |
| Datetime | Timer start/stop times |
| Switches | Timer active (commented out) |

### What Does NOT Exist
- **No `lawn_mower` entity type** in ESPHome's native API
- **No `lawn_mower_mqtt.yaml`** (deleted)
- **No `ha/lawn_mower.yaml`** (deleted)
- **No MQTT configuration** in the default setup

### Available Mower Control Methods in `Automower` Class
```cpp
void setMode(uint8_t mode);       // 0x00=MAN, 0x01=AUTO, 0x03=HOME, 0x04=DEMO
void setStop(bool stop);          // true = pause, false = resume
void key_num(uint8_t key);        // Simulate keypad press (0x00-0x12)
void write_register(uint16_t addr, uint16_t val);  // Direct register write
uint16_t get_register(uint16_t addr, bool is_signed); // Read register
```

---

## 4. The Core Problem

### ESPHome Native API Entity Types
ESPHome's native API supports these entity types that map to Home Assistant:

| ESPHome Entity | HA Entity Domain |
|---------------|-----------------|
| `binary_sensor` | `binary_sensor.*` |
| `button` | `button.*` |
| `cover` | `cover.*` |
| `climate` | `climate.*` |
| `fan` | `fan.*` |
| `light` | `light.*` |
| `sensor` | `sensor.*` |
| `switch` | `switch.*` |
| `text_sensor` | `text_sensor.*` |
| `number` | `number.*` |
| `select` | `select.*` |
| `datetime` | `datetime.*` |
| `alarm_control_panel` | `alarm_control_panel.*` |
| `camera` | `camera.*` |
| `valve` | `valve.*` |
| `siren` | `siren.*` |
| `event` | `event.*` |
| `api.action` | Custom actions |

**`lawn_mower` is NOT in this list.**

### Home Assistant Lawn Mower Integration
The `lawn_mower` domain in Home Assistant is a **building block integration** — it's not a standalone integration you add directly. It's provided by other integrations (currently only MQTT). The lawn mower entity supports:

**States:** `mowing`, `docked`, `paused`, `returning`, `error`, `unavailable`, `unknown`

**Actions:** `start_mowing`, `pause`, `dock`

**Triggers:** `started_mowing`, `paused_mowing`, `returned_to_dock`, `errored`, `started_returning`

**Conditions:** `is_mowing`, `is_paused`, `is_docked`, `is_returning`, `is_encountering_an_error`

### The Gap
| Requirement | MQTT | Native API | Template |
|------------|------|------------|----------|
| `lawn_mower` entity | Yes | No | No |
| State tracking | Yes | No | Manual |
| Actions (start/pause/dock) | Yes | No | Manual |
| Triggers/conditions | Yes | No | Manual |
| Zero extra infrastructure | No | Yes | Yes |

---

## 5. Alternative Approaches

### Option A: Reintroduce MQTT Bridge (The Original Approach)

**Pros:**
- Full `lawn_mower` entity with all states, actions, triggers, and conditions
- Native HA automations work out of the box
- Proven design (already implemented in commit 6c87105)

**Cons:**
- Requires MQTT broker setup (Mosquitto, Home Assistant MQTT add-on, etc.)
- Requires network configuration on both ESPHome and HA sides
- Additional infrastructure to maintain
- Credentials management
- Your stated concern: "MQTT looks complex to set up"

**Feasibility:** High (already works), but **usability** is the barrier.

---

### Option B: Template Lawn Mower (No MQTT, No Extra Infrastructure)

Create a `template:` lawn mower in Home Assistant that reads state from ESPHome sensors and sends commands via ESPHome actions or service calls.

#### How It Would Work

**State reading:** The template lawn mower reads the `status` text sensor from ESPHome (which already publishes `mowing`, `paused`, `docked`, `idle`, `returning`, `error` via the native API).

**Commands:** Use ESPHome's `api.action` or Home Assistant's `esphome.<device>_set_mode` / `esphome.<device>_set_stop` service calls to send commands.

#### Implementation Sketch

```yaml
# Home Assistant configuration.yaml
template:
  lawn_mower:
    - name: "Automower"
      unique_id: automower_template_mower

      # State from ESPHome's status text sensor
      activity_template: "{{ states('sensor.automower_status') }}"

      # Actions via ESPHome service calls
      start_mowing:
        - service: esphome.automower_set_mode
          data:
            mode: "AUTO"
        - service: esphome.automower_set_stop
          data:
            stop: false

      pause:
        - service: esphome.automower_set_stop
          data:
            stop: true

      dock:
        - service: esphome.automower_set_mode
          data:
            mode: "HOME"

      # Availability from ESPHome device state
      availability:
        - entity_id: sensor.automower_status
          state:
            - "unavailable"
```

#### ESPHome Side Changes Needed

1. **Expose `set_mode` and `set_stop` as API actions** so HA can call them:
   ```cpp
   // In automower.h
   void setup() override {
     register_service(&Automower::api_set_mode, "set_mode", {"mode"});
     register_service(&Automower::api_set_stop, "set_stop", {"stop"});
   }

   void api_set_mode(const std::string& mode) {
     uint8_t m = 0x00;
     if (mode == "AUTO") m = 0x01;
     else if (mode == "HOME") m = 0x03;
     else if (mode == "DEMO") m = 0x04;
     else if (mode == "MAN") m = 0x00;
     setMode(m);
   }

   void api_set_stop(bool stop) {
     setStopStatusFromCode(stop ? 0x0414 : 0x0410);
   }
   ```

2. **Map status text sensor values** to HA lawn mower activity values in the template.

#### Pros
- **Zero extra infrastructure** — uses only the existing ESPHome native API
- No MQTT broker needed
- Simple configuration
- Works with the current firmware architecture

#### Cons
- **No native `lawn_mower` entity** — it's a template entity, so:
  - No built-in `lawn_mower.*` triggers in automations
  - No `lawn_mower.dock` service (must use template actions)
  - No `lawn_mower.is_mowing` condition (must use template conditions)
  - No device-level lawn mower features in HA (e.g., mowing maps from compatible mowers)
  - State updates are polling-based (not push), so there's a delay
  - No built-in `returning` activity state (must be inferred)
- More complex HA configuration
- Fragile — depends on text sensor state matching expected values

**Feasibility:** Medium-High. The ESPHome side requires adding API actions. The HA side requires template configuration. It works but is not as clean as MQTT.

---

### Option C: Custom ESPHome Component with Lawn Mower Entity (Long-term)

Contribute a `lawn_mower` component to ESPHome itself, or maintain a fork with one.

#### What Would Be Needed
1. Create an ESPHome component that inherits from a hypothetical `lawn_mower` base class (or the existing `Component` base with custom entity registration)
2. Register the entity with the ESPHome native API protocol buffer
3. Home Assistant would automatically discover it as a `lawn_mower` entity

#### Pros
- True native integration
- Full HA lawn mower features
- No MQTT, no templates, no workarounds

#### Cons
- Requires changes to ESPHome core (upstream contribution)
- Long timeline — ESPHome core changes are slow
- Must maintain fork until merged upstream
- Uncertain if ESPHome maintainers would accept it

**Feasibility:** Low in the short term, but this is the **ideal end state**.

---

### Option D: Hybrid — Native API for Everything, MQTT Only for Lawn Mower (Refined)

A refined version of Option A that minimizes MQTT complexity:

1. Use **Home Assistant's built-in Mosquitto broker** (the official add-on) — zero external setup
2. ESPHome connects to `core-mosquitto` on the HA host
3. Only the lawn mower entity uses MQTT; everything else stays on native API
4. Use ESPHome's `mqtt:` component with `discovery: false`

#### Pros
- Minimal MQTT footprint
- Full lawn mower entity
- Uses HA's own broker (no separate infrastructure)

#### Cons
- Still requires the Mosquitto add-on
- Still requires MQTT credentials
- Still a second protocol in the mix

**Feasibility:** High. This is the most practical "real lawn mower entity" option if you're willing to accept the Mosquitto add-on.

---

## 6. Comparison Matrix

| Criterion | A: MQTT Bridge | B: Template | C: ESPHome Core | D: Hybrid |
|-----------|---------------|-------------|-----------------|-----------|
| Real `lawn_mower` entity | Yes | No (template) | Yes | Yes |
| Full HA triggers/conditions | Yes | No | Yes | Yes |
| Zero extra infrastructure | No | Yes | Yes | No |
| MQTT broker required | Yes | No | No | Yes (HA add-on) |
| ESPHome code changes | Minimal | Moderate (API actions) | Major (core) | Minimal |
| HA config complexity | Low | Medium | Low | Low |
| Setup complexity | Medium-High | Low | N/A | Low-Medium |
| Maintenance burden | Low | Medium | High | Low |
| State update latency | ~10s (polling) | ~1-5s (polling) | Real-time | ~10s (polling) |
| `returning` state support | Collapsed to mowing | Manual mapping | Full | Collapsed |

---

## 7. Status Mapping Considerations

The Automower's status text sensor publishes values like:
- `mowing`, `paused`, `docked`, `idle`, `returning`, `error`

MQTT lawn mower activity accepts only:
- `mowing`, `paused`, `docked`, `error`

**Mapping challenge:** There is no `returning` activity in MQTT's lawn mower. The previous implementation collapsed `returning` -> `mowing`. This is a known limitation.

**Template approach advantage:** A template lawn mower can use the `activity` state (not `activity_template`) and manually set it, potentially preserving `returning` — but this requires custom logic.

---

## 8. Recommendation

### For Immediate Use (No MQTT)
**Go with Option B (Template).** It's the only approach that:
- Requires no extra infrastructure
- Works with the current firmware
- Provides a usable lawn mower entity in HA dashboards
- Allows basic automations (dock on rain, notify when done, etc.)

The trade-off is that you lose the rich `lawn_mower.*` triggers and conditions, but you can replicate most of them with template conditions and ESPHome sensor state checks.

### For Full Lawn Mower Features (Accept MQTT)
**Go with Option D (Hybrid).** Use Home Assistant's Mosquitto add-on — it's a one-click install in the HA UI. The MQTT setup then becomes:
1. Install Mosquitto add-on in HA
2. Add broker URL, username, password to ESPHome secrets
3. Uncomment the MQTT lawn mower package
4. Add the HA lawn mower config

This is the simplest path to a **real** lawn mower entity without running a separate MQTT broker.

### Long-term Goal
**Pursue Option C (ESPHome Core).** File an issue/PR on the ESPHome repository requesting a `lawn_mower` component. The `Automower` component already exists and has all the necessary state and control methods — it just needs to be wrapped in a lawn mower entity type. This would benefit the entire community.

---

## 9. Implementation Plan: Option B (Template) — Step by Step

### ESPHome Side (`automower.h` / `automower.cpp`)
1. Add `register_service()` calls for `set_mode` and `set_stop`
2. Implement `api_set_mode()` and `api_set_stop()` methods
3. Ensure `status` text sensor publishes values compatible with template mapping

### Home Assistant Side (`configuration.yaml` or package)
1. Create a `template:` lawn mower entity
2. Map `sensor.automower_status` values to lawn mower activities
3. Wire up `start_mowing`, `pause`, `dock` actions to ESPHome service calls
4. Set up availability based on ESPHome device state

### Example HA Configuration
```yaml
# packages/automower_template.yaml
template:
  lawn_mower:
    - name: "Automower"
      unique_id: automower_template_mower
      icon: "mdi:robot-mower"

      # Map status text sensor to activity
      activity_template: >
        {% set s = states('sensor.automower_status') %}
        {% if s in ['mowing', 'mowing/returning'] %} mowing
        {% elif s == 'returning' %} returning
        {% elif s == 'paused' %} paused
        {% elif s in ['docked', 'idle'] %} docked
        {% elif s == 'error' %} error
        {% else %} unavailable {% endif %}

      start_mowing:
        - service: esphome.automower_set_mode
          data: { mode: "AUTO" }
        - service: esphome.automower_set_stop
          data: { stop: false }

      pause:
        - service: esphome.automower_set_stop
          data: { stop: true }

      dock:
        - service: esphome.automower_set_mode
          data: { mode: "HOME" }

      availability:
        - entity_id: sensor.automower_status
          state:
            - "unavailable"

      # Custom actions for lawn mower triggers
      # (HA doesn't natively support lawn_mower.* triggers on template entities,
      #  so we create helper binary sensors instead)

binary_sensor:
  - platform: template
    name: "Automower Mowing"
    device_class: running
    state_template: >
      {% set s = states('sensor.automower_status') %}
      {{ s in ['mowing', 'mowing/returning'] }}

  - platform: template
    name: "Automower Docked"
    device_class: plug
    state_template: >
      {{ states('sensor.automower_status') in ['docked', 'idle'] }}

  - platform: template
    name: "Automower Error"
    device_class: problem
    state_template: >
      {{ states('sensor.automower_status') == 'error' }}
```

### Example Automations (Template-based)
```yaml
# Dock when rain starts
automation:
  - alias: "Dock on rain"
    trigger:
      - platform: state
        entity_id: binary_sensor.rain_sensor
        to: "on"
    condition:
      - condition: template
        value_template: "{{ is_state('sensor.automower_status', 'mowing') }}"
    action:
      - service: esphome.automower_set_mode
        data: { mode: "HOME" }

# Notify when mowing is done
  - alias: "Mowing done notification"
    trigger:
      - platform: state
        entity_id: sensor.automower_status
        from: "mowing"
        to: "docked"
    action:
      - service: notify.mobile_app_xavier_phone
        data:
          message: "The Automower is back at the dock."
```

---

## 10. Risks and Caveats

1. **State polling delay:** The template approach polls the status sensor, which is updated every ~1s by the ESPHome polling component. There may be a 1-5 second delay between the mower changing state and HA reflecting it.

2. **Race conditions:** If the user presses a button on the mower's keypad while HA is trying to control it, the states may diverge. The template reads the last known state, which may be stale.

3. **No `returning` in MQTT:** The MQTT approach collapses `returning` -> `mowing`. The template approach can preserve `returning` but at the cost of losing the native `lawn_mower` entity semantics.

4. **ESPHome API actions:** Adding custom API actions requires C++ changes to the mower component. This is straightforward but needs testing.

5. **Firmware updates:** Any future changes to the mower component's API must be reflected in the HA template configuration.

---

## 11. Conclusion

| Goal | Recommended Approach |
|------|---------------------|
| Lawn mower entity **now**, no MQTT | **Option B: Template** |
| Lawn mower entity **now**, accept MQTT | **Option D: Hybrid (HA Mosquitto)** |
| Lawn mower entity **properly** (long-term) | **Option C: ESPHome core PR** |

The template approach (Option B) is the most pragmatic immediate solution. It provides a functional lawn mower entity in Home Assistant using only the existing ESPHome native API, with minimal configuration and zero additional infrastructure. The main trade-off is the loss of native `lawn_mower.*` triggers and conditions, which can be partially replicated with template binary sensors and state-based automations.

For users who want the full lawn mower experience (triggers, conditions, mowing maps, etc.), the hybrid approach (Option D) using Home Assistant's built-in Mosquitto add-on is the simplest path to a real lawn mower entity without running a separate MQTT broker.

---

## Appendix A: ESPHome MQTT Bridge (`lawn_mower_mqtt.yaml`)

> Full implementation from commit `6c87105`. This file was deleted from main but remains
> accessible at `git show 6c87105:components/confs/lawn_mower_mqtt.yaml`.

```yaml
# Home Assistant "lawn_mower" domain bridge over MQTT (Option A).
#
# ESPHome's native API has no lawn_mower entity type, so the device cannot
# advertise itself as a native HA mower over the API. This optional package
# adds a *minimal* MQTT link that is used ONLY for the lawn_mower entity:
# every sensor/button/select keeps talking to HA over the native API as before.
#
# To enable:
#   1. Add this file to the `packages:` -> `files:` list in your device YAML
#      (see hqam-esphome.yaml for the commented entry).
#   2. Provide these secrets in your ESPHome `secrets.yaml`:
#        mqtt_broker: "192.168.x.x"
#        mqtt_username: "esphome"
#        mqtt_password: "..."
#   3. Add the matching Home Assistant configuration from ha/lawn_mower.yaml.
#
# The MQTT topic prefix defaults to "automower"; override it via substitutions
# if you run more than one mower.

substitutions:
  mqtt_topic_prefix: "automower"

mqtt:
  broker: !secret mqtt_broker
  username: !secret mqtt_username
  password: !secret mqtt_password

  # We keep the native API for everything else, so do NOT let MQTT auto-
  # discovery create a duplicate set of entities in Home Assistant.
  discovery: false

  # Drives the HA entity's availability (online/offline) via birth/last-will.
  birth_message:
    topic: "${mqtt_topic_prefix}/status"
    payload: "online"
    retain: true
  will_message:
    topic: "${mqtt_topic_prefix}/status"
    payload: "offline"
    retain: true

  # Commands published by the HA lawn_mower actions (start_mowing / pause / dock).
  on_message:
    - topic: "${mqtt_topic_prefix}/command"
      then:
        - lambda: |-
            auto *m = (esphome::mower::Automower *) id(mower_id);
            if (x == "start_mowing") {
              m->set_stop(false);      // clear a previous STOP latch before resuming
              m->set_mode("AUTO");
            } else if (x == "dock") {
              m->set_mode("HOME");
            } else if (x == "pause") {
              m->set_stop(true);
            }

# Publish the current HA lawn_mower activity, mapped from the mower status to
# one of MQTT's four accepted values: mowing | paused | docked | error.
# Retained + periodic so the state is re-asserted after a broker reconnect.
interval:
  - interval: 10s
    then:
      - mqtt.publish:
          topic: "${mqtt_topic_prefix}/activity"
          retain: true
          payload: !lambda |-
            auto *m = (esphome::mower::Automower *) id(mower_id);
            std::string s = m->get_status_text_sensor()->state;
            if (s == "error") return std::string("error");
            if (s == "paused") return std::string("paused");
            if (s == "docked" || s == "idle") return std::string("docked");
            // mowing, returning, "mowing/returning", ... -> the mower is active.
            // MQTT lawn_mower has no "returning" activity, so it collapses to "mowing".
            return std::string("mowing");
```

---

## Appendix B: Home Assistant MQTT Config (`ha/lawn_mower.yaml`)

> Full implementation from commit `6c87105`. This file was deleted from main but remains
> accessible at `git show 6c87105:ha/lawn_mower.yaml`.

```yaml
# Home Assistant configuration for the native lawn_mower entity (Option A).
#
# This is the Home Assistant side of components/confs/lawn_mower_mqtt.yaml.
# Add it under `mqtt:` in your configuration.yaml (or include it as an MQTT
# package). Requires the MQTT integration to be set up in Home Assistant and
# pointed at the same broker the ESP publishes to.
#
# The topic prefix must match `mqtt_topic_prefix` on the ESPHome side
# (default: "automower").

mqtt:
  - lawn_mower:
      name: Automower
      unique_id: automower_mower

      # State: the ESP publishes mowing | paused | docked | error here.
      activity_state_topic: "automower/activity"
      activity_value_template: "{{ value }}"

      # Actions: HA publishes the fixed payloads below, which the ESP routes
      # to set_mode()/set_stop() (see the on_message block in the bridge).
      start_mowing_command_topic: "automower/command"
      start_mowing_command_template: "start_mowing"
      pause_command_topic: "automower/command"
      pause_command_template: "pause"
      dock_command_topic: "automower/command"
      dock_command_template: "dock"

      # Availability driven by the ESP's MQTT birth/last-will message.
      availability:
        - topic: "automower/status"
          payload_available: "online"
          payload_not_available: "offline"

      device:
        identifiers: automower_esp
        name: Automower
        manufacturer: Husqvarna
        model: "220/230 AC"
```

---

## Appendix C: ESPHome Main Config Change (`hqam-esphome.yaml`)

> The change from commit `6c87105` that adds the commented-out MQTT package entry.
> This was the opt-in mechanism — users uncommented the line to enable the MQTT bridge.

```yaml
# In the packages -> remote_package -> files list:
packages:
  remote_package:
    url: https://github.com/xperseguers/esphome-hqam
    ref: main
    refresh: always
    files:
      [
        components/boards/esp32dev.yaml,
        components/confs/base.yaml,
        components/confs/button.yaml,
        components/confs/select.yaml,
        components/confs/sensor.yaml,
        components/confs/textsensor.yaml,
        components/confs/switch.yaml,
        # Optional: expose a native Home Assistant lawn_mower entity over MQTT.
        # Uncomment the next line, add mqtt_broker/mqtt_username/mqtt_password to
        # your secrets.yaml, and apply ha/lawn_mower.yaml in Home Assistant.
        # components/confs/lawn_mower_mqtt.yaml,
      ]
```

---

## Appendix D: Original README Section (from commit 6c87105)

> The README section that was added in commit `6c87105` and later removed when the
> README was restructured. Included here for historical reference.

```markdown
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
```

---

## Appendix E: Source Commit Metadata

> Preserving the original commit information for traceability.

```
commit 6c87105ce0afd16050b4f8655dd7f40230cec92b
Author: Xavier Perseguers <xavier@causal.ch>
Date:   Wed Jul 8 10:54:33 2026 +0200

    [TASK] Prepare a native HomeAssistant integration

    Discussion: https://community.home-assistant.io/t/tutorial-on-how-to-use-the-ha-lawn-mower-integration/1000736
```

**Files changed in that commit:**
| File | Lines | Type |
|------|-------|------|
| `README.md` | +33 | Documentation (later removed) |
| `components/confs/lawn_mower_mqtt.yaml` | +74 | New file (ESPHome MQTT bridge) |
| `ha/lawn_mower.yaml` | +39 | New file (HA MQTT config) |
| `hqam-esphome.yaml` | +4 | Config change (opt-in package) |

**Total: 4 files changed, 150 insertions(+)**

**Home Assistant Discussion:**
https://community.home-assistant.io/t/tutorial-on-how-to-use-the-ha-lawn-mower-integration/1000736
