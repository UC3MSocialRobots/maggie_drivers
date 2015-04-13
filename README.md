# maggie_drivers

GitHub repository for the drivers for the Social Robot Maggie.

## CONTENT

- `maggie_ir_drivers`: a ROS package with the interface and the wrapper for the IR controller device. Also contains a mock (see gmock) to simulate the behavior of the physical device.
- `maggie_labjack_drivers`: a ROS package with the interface and the wrapper for the labjack device. Also contains a mock (see gmock) to simulate the behavior of the physical device.
- `maggie_drivers`: a ROS metapackage.
- `maggie_motor_drivers`: a ROS package with the interface and the drivers for the motors device.
- `maggie_rfid_drivers`: a ROS package with the interface and the wrapper for the RFID device.
- `maggie_serial_comm_drivers`: a ROS package with the interface and the drivers for the servomotors device, implemented for the serial communication. Also contains a mock (see gmock) to simulate the behavior of the physical device.

## LICENSE

The license of the packages is custom LASR-UC3M (Licencia Académica Social Robotics Lab - UC3M), an open, non-commercial license which enables you to download, modify and distribute the code as long as you distribute the sources.  
