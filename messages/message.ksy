meta:
  id: message
  bit-endian: le
  endian: le
doc: Dongle message.
seq:
  - id: start
    type: u1
    valid: 0xAA
    doc: Start byte

  - id: len_message
    type: u1
    valid:
      min: 10 # Minimum message length with empty body
    doc: Message length without start byte

  - id: app_id
    type: u1
    enum: app
    doc: Appliance ID

  - id: sync
    type: u1
    doc: Sync byte

  - id: reserved1
    type: u1

  - id: reserved2
    type: u1

  - id: id
    type: u1
    doc: ID

  - id: reserved3
    type: u1

  - id: prot_id
    type: u1
    doc: Appliance protocol identifier

  - id: type_id
    type: u1
    doc: Message type

  - id: body
    size: len_message - 10
    type:
      switch-on: type_id
      cases:
        0x02: midea_transparent_data
        0x03: midea_transparent_data

  - id: checksum
    type: u1
    doc: Checksum

enums:
  app:
    0xAC:
      id: 'air_conditioner'
    0xFF:
      id: 'broadcast'

  mode:
    0x01:
      id: 'auto'
      doc: AUTO operation mode
    0x02:
      id: 'cool'
      doc: COOL operation mode
    0x03:
      id: 'dry'
      doc: DRY operation mode
    0x04:
      id: 'heat'
      doc: HEAT operation mode
    0x05:
      id: 'fan'
      doc: FAN ONLY operation mode
    0x06:
      id: 'dry_custom'
      doc: DRY CUSTOM operation mode

  swing:
    0b0000:
      id: 'off'
    0b0011:
      id: 'horizontal'
    0b1100:
      id: 'vertical'
    0b1111:
      id: 'both'

  fan:
    40:
      id: 'low'
    60:
      id: 'medium'
    80:
      id: 'high'
    102:
      id: 'auto'

types:
  midea_transparent_data:
    doc: Transparent Midea protocol data
    seq:
      - id: type_id
        type: u1
        doc: Midea message type ID

      - id: data
        size: _io.size - 2
        type:
          switch-on: type_id
          cases:
            0xC0: midea_status_c0

      - id: checksum
        type: u1
        doc: Standard CRC-8-MAXIM checksum

  midea_status_c0:
    doc: Midea Air Conditioner status data
    -orig-id: DeviceStatus
    seq:
      # Byte 0

      - id: power
        type: b1
        doc: Power status
        -orig-id: powerStatus
      - id: unused1
        type: b1
      - id: imode_resume
        type: b1
        -orig-id: imodeResume
      - id: unused2
        type: b1
      - id: timer_mode
        type: b1
        -orig-id: timerMode
      - id: test2
        type: b1
        -orig-id: test2
      - id: unused3
        type: b1
      - id: err_mark
        type: b1
        -orig-id: errMark

      # Byte 1

      - id: old_temp
        type: b4
        doc: Target temperature minus 16
        -orig-id: setTemperature
      - id: dot_temp
        type: b1
        doc: Target temperature decimal .5 part
        -orig-id: setTemperature_dot
      - id: mode
        doc: Operation mode
        type: b3
        enum: mode
        -orig-id: mode

      # Byte 2

      - id: fan_speed
        type: b7
        enum: fan
        doc: Fan speed
        -orig-id: fanSpeed
      - id: unused4
        type: b1

      # Byte 3,4,5

      - id: timers
        doc: ON/OFF timers
        type: midea_timers

      # Byte 6

      - id: swing
        type: b4
        enum: swing
        doc: Swing mode
        -orig-id: updownFan | leftRightFan
      - id: unused5
        type: b4

      # Byte 7

      - id: cosy_sleep
        type: b2
        -orig-id: cosySleep
      - id: unused6
        type: b1
      - id: save
        type: b1
        -orig-id: save
      - id: low_freq_fan
        type: b1
        -orig-id: lowFerqFan
      - id: turbo1
        type: b1
        doc: TURBO preset
        -orig-id: tubro
      - id: unused7
        type: b1
      - id: feel_own
        type: b1
        -orig-id: feelOwn

      # Byte 8

      - id: child_sleep_mode
        type: b1
        -orig-id: childSleepMode
      - id: natural_fan
        type: b1
        -orig-id: naturalFan
      - id: dry_clean
        type: b1
        -orig-id: dryClean
      - id: ptc_assis
        doc: PTC Assist heater
        type: b1
        -orig-id: ptcAssis
      - id: eco
        doc: ECO preset
        type: b1
        -orig-id: eco
      - id: clean_up
        type: b1
        -orig-id: cleanUp
      - id: self_cosy_sleep
        type: b1
        -orig-id: slefCosySleep
      - id: self_feel_own
        type: b1
        -orig-id: selfFeelOwn

      # Byte 9

      - id: sleep_preset
        type: b1
        doc: SLEEP preset
        -orig-id: sleepFunc
      - id: turbo2
        type: b1
        doc: TURBO preset
        -orig-id: tubro
      - id: fahrenheits
        type: b1
        -orig-id: tempUnit
      - id: exchange_air
        type: b1
        -orig-id: exchangeAir
      - id: night_light
        type: b1
        -orig-id: nightLight
      - id: catch_cold
        type: b1
        -orig-id: catchCold
      - id: peak_elec
        type: b1
        -orig-id: peakElec
      - id: cool_fan
        type: b1
        -orig-id: coolFan

      # Byte 10

      - id: indoor_temp
        type: u1
        doc: |
          Indoor temperature minus 25°C. Unit : 0.5°C.
          Temperature below -25°C cannot be represented.
          Precision may be increased to 0.1°C using indoor_temp_decimal field.
        -orig-id: indoor_temp

      # Byte 11

      - id: outdoor_temp
        type: u1
        doc: |
          Outdoor temperature minus 25°C. Unit : 0.5°C.
          Temperature below -25°C cannot be represented.
          Precision may be increased to 0.1°C using outdoor_temp_decimal field.
        -orig-id: outdoor_temp

      # Byte 12

      - id: new_temp
        type: b5
        doc: |
          New models target temperature minus 12°C field. Unit : 1°C.
          Not supported by old models. If unsupported, value is always 0.
          In that case, use old_temp field instead.
        -orig-id: setNewTemperature
      - id: dus_full
        type: b1
        doc: Air filter maintenance reminder
        -orig-id: dusFull
      - id: unused9
        type: b2

      # Byte 13

      - id: pwm_mode
        type: b4
        -orig-id: pwmMode
      - id: light
        type: b3
        doc: |
          Display light status (0: ON, 7: OFF). Supported by new models only with display light UART control.
        -orig-id: light
      - id: unused10
        type: b1

      # Byte 14

      - id: indoor_temp_decimal
        type: b4
        doc: |
          Indoor temperature decimal BCD part. Unit : 0.1°C.
          May increase measurement precision from 0.5°C to 0.1°C.
          If unsupported, value is always 0.
        -orig-id: T1_dot
      - id: outdoor_temp_decimal
        type: b4
        doc: |
          Outdoor temperature decimal BCD part. Unit : 0.1°C.
          May increase measurement precision from 0.5°C to 0.1°C.
          If unsupported, value is always 0.
        -orig-id: T4_dot

      # Byte 15

      - id: error_code
        type: u1
        doc: Error code
        -orig-id: errInfo

      # Byte 16,17

      - id: reserved1
        type: u2

      # Byte 18

      - id: humidity
        type: b7
        doc: |
          Humidity percentage setpoint in DRY CUSTOM mode.
        -orig-id: humidity
      - id: unused11
        type: b1

      # Byte 19

      - id: reserved2
        type: u1

      # Byte 20

      - id: set_expand
        type: b6
        doc: |
          Unknown temperature?? Unit : 0.5°C.
        -orig-id: setExpand
      - id: double_temp
        type: b1
        -orig-id: double_temp
      - id: eight_hot
        type: b1
        doc: |
          `FROST PROTECTION` preset (8°C).
        -orig-id: Eight_Hot

      # Tail

      - id: tail
        doc: Message tail
        size-eos: true

    instances:
      target_temperature:
        doc: Target temperature in °C with 0.5°C precision
        value: '((new_temp != 0) ? (new_temp + 12) : (old_temp + 16)) + dot_temp.to_i * 0.5'
      indoor_temperature_half:
        doc: Indoor temperature in °C with 0.5°C precision
        value: '(indoor_temp - 50) / 2.0'
      indoor_temperature:
        doc: Indoor temperature in °C with 0.1°C precision
        value: 'indoor_temperature_half + ((indoor_temp_decimal >= 5) ? (indoor_temp_decimal - 5) : indoor_temp_decimal) / ((indoor_temperature_half < 0) ? -10.0 : 10.0)'
      outdoor_temperature_half:
        doc: Outdoor temperature in °C with 0.5°C precision
        value: '(outdoor_temp - 50) / 2.0'
      outdoor_temperature:
        doc: Outdoor temperature in °C with 0.1°C precision
        value: 'outdoor_temperature_half + ((outdoor_temp_decimal >= 5) ? (outdoor_temp_decimal - 5) : outdoor_temp_decimal) / ((outdoor_temperature_half < 0) ? -10.0 : 10.0)'

  midea_timers:
    doc: Midea ON/OFF timers
    seq:
      - id: ton_high
        type: u1
        valid:
          min: 0x7F
      - id: tof_high
        type: u1
        valid:
          min: 0x7F
      - id: tof_low
        type: b4
        valid:
          max: 14
      - id: ton_low
        type: b4
        valid:
          max: 14
    instances:
      ton_minutes:
        doc: Minutes remaining before ON
        value: '(ton_high - 0x7F) * 15 - ton_low'
      tof_minutes:
        doc: Minutes remaining before OFF
        value: '(tof_high - 0x7F) * 15 - tof_low'
