# Socket | 1 Channel

> This product configuration targets the **ESP32-C6_Relay_X1** board.

## Description

A smart socket featuring relay control, status indication via a single-color LED, and button-based user interactions:

* **Relay Control**: Controls power via a GPIO-connected relay.
* **User Input**:
  * Single button press toggles the socket state.
  * Long press triggers a factory reset.
* **Device Status Indication**: An active-high LED displays socket state and system events.
* **Matter Data Model Specification**:
  * **Device Type** : `On/Off Plug`

## Hardware Configuration

<img src="../../docs/images/product_socket.png" alt="Socket Single Channel" width="500"/>

The following hardware components are used for this product:

* **Board**: ESP32-C6_Relay_X1
* **Power Relay**: On-board single-channel relay
* **Indicator**: On-board active-high, single-color LED
* **Button**: On-board active-low button

### Pin Assignment

| Peripheral      | GPIO Pin | Function                |
|-----------------|----------|-------------------------|
| Relay Control   | GPIO19   | Active-high power switching |
| Button          | GPIO9    | Active-low user input   |
| LED             | GPIO2    | Active-high status indication |

> **Note**: GPIO assignments can be customized by modifying the following macros in **app_driver.cpp**:
> `RELAY_GPIO_NUM`, `BUTTON_GPIO_NUM`, `INDICATOR_GPIO_NUM`

## Matter over Thread Configuration

Select **ESP32-C6 / Thread** when preparing the device. When generating the
commissioning data from a terminal, pass `thread` as the connection type so the
Thread data model is used:

```sh
./tools/mfg/mfg_low_code.sh products/socket esp32c6 <MAC_ADDRESS> thread
```

The socket is exposed as a Matter On/Off Plug device and requires a Matter
controller with a Thread border router for commissioning.

## Understanding Code

### Initialization Sequence

The `app_driver_init()` function, called from `setup()` in `app_main.cpp`, performs the following:

* Configures the relay GPIO as output.
* Configures the relay and LED through the HP GPIO API and drives both low so the socket always starts off.
* Initializes the button with debounce handling and registers the following callbacks:
  * **Single-click**: Toggles the socket state.
  * **Long-press**: Initiates factory reset.
* Initializes the single-color LED for status indication.

### Core Functions

* **Power Control**:
  * `app_driver_toggle_socket_state_button_callback` is invoked on a single-click event.
  * It toggles the socket state using `app_driver_set_socket_state`, updates the LED accordingly, and reports the new state to the system.
  * Every on command starts a 500 ms one-shot timer. When it expires, the relay and LED turn off and the off state is reported to Matter.
  * Set `AUTO_OFF_TIMEOUT_MS` to `0` in `main/app_driver.cpp` to disable automatic turn-off; the default remains 500 ms.

* **Visual Indicators**:
  * `LOW_CODE_EVENT_SETUP_MODE_START`: turns the indicator on during setup.
  * `LOW_CODE_EVENT_SETUP_MODE_END`: restores the indicator to the socket power state.
  * `LOW_CODE_EVENT_READY`: displays full brightness white light to indicate device is ready

### Extending Functionality

To add a second relay channel to the system, implement the following changes:

* **Matter Data Model Extension**:
  * Add a second On/Off Plug Device Type endpoint to the Matter cluster configuration.
  * Run `Upload Configuration` command to upload the updated data model on the device.

* **Configure an Additional Button Input**:
  * Initialize an additional GPIO button.
  * Register a **single-click event callback** using `button_driver_register_cb`.
  * Inside the callback:
    * Toggle the state of the second relay.
    * Report the new relay state to the system using `low_code_feature_update_to_system`.

## Related Documentation

* [Socket | 2 Channel](../socket_2_channel/README.md)
* [Programmer's Model](../../docs/programmer_model.md)
* [Components](../../components/README.md)
* [Drivers](../../drivers/README.md)
* [Products](../README.md)
