# 📍OpenHaystack on Zephyr
Implementation of OpenHaystack firmware on [Zephyr RTOS](https://zephyrproject.org).

## Requirements
[PlatformIO](https://platformio.org) for building and flashing targets, [Node.js](https://nodejs.org) to generate code snippets with advertising key included. 

## Install
```shell
# Clone this repo
git clone https://github.com/b4ck5p4c3/openhaystack-zephyr

# Add a new accessory in OpenHaystack, then get the advertising key in Base64.
# Convert it to payload snippet using convert-key script
node convert-key.js YOUR-BASE64-ADVERTISING-KEY

# Paste payload snippet to the corresponding place in src/main.cpp file
nano src/main.cpp

# Adjust the Device Name in Zephyr configuration
nano zephyr/prj.conf

# Compile and upload
pio run --target=upload --environment=nrf51-jlink # nRF51 connected with J-Link
pio run --target=upload --environment=nrf51-stlink # nRF51 connected with ST-Link
pio run --target=upload --environment=nrf52-stlink # nRF52 connected with ST-Link
```

## Full erase
If you experience weird issues on nRF51, you may want to re-flash SoftDevice using commands below.
```shell
# Download and extract S110 8.0.0 SoftDevice somewhere.
# https://www.nordicsemi.com/Products/Development-software/S110/Download?lang=en#infotabs

# Go to directory with extracted S110, then erase and upload S110 to target
nrfjprog_bin --eraseall
nrfjprog_bin --program s110_nrf51_8.0.0_softdevice.hex
```
