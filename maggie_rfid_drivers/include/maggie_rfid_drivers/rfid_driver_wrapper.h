#ifndef __RFID_DRIVER_WRAPPER_H__
#define __RFID_DRIVER_WRAPPER_H__

/**
 * @file        rfid_driver_wrapper.h
 * @brief       Wrapper for the rfid driver.
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-02
 * @author      Ana Corrales <>
 * @date        2011-07
 *
 * @copyright   Copyright (C) 2015 University Carlos III of Madrid.
 *              All rights reserved.
 * @license     LEUC3M v1.0, see LICENSE.txt
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Licencia Educativa UC3M as published by
 * the University Carlos III of Madrid, either version 1.0, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY. See the Licencia Educativa UC3M
 * version 1.0 or any later version for more details.
 *
 * A copy of the Licencia Educativa UC3M is in the LICENSE file.
 */

#include <ros/ros.h>
#include "rfid_driver_interface.h"

// third party driver headers
#include <feusb.h>
#include <feisc.h>
#include <FedmIscCore.h>
#include <impl/core/i_scan/tag_handler/FedmIscTagHandler_EPC_Class1_Gen2.h>

class RfidDriverWrapper : public RfidDriverInterface {
    public:
        /**
         * @brief Empty constructor.
         */
        RfidDriverWrapper();

        /**
         * @brief Destructor.
         */
        ~RfidDriverWrapper();

        /**
         * @brief Method to open and enable the USB RFID reader.
         * @param num_device, number of device.
         * @return error: -1 or success: 0.
         */
        int open_device(long num_device);

        /**
         * @brief Method to close the USB RFID reader.
         * @param
         * @return error: -1 or success: 0.
         */
        int close_device();

        /**
         * @brief Method to get the UID label detected.
         * @param
         * @return The UID detected.
         */
        std::string get_uid_label();

        /**
         * @brief Method to get the UIDs of the labels detected.
         * @param
         * @return Labels detected in the inventory function.
         */
        std::string* get_tags();

        /**
         * @brief Method to get the type of RFID reader used.
         * @return a string with the name or empty if error.
         */
        std::string get_type_reader();

        /**
         * @brief Method to get the number of labels detected.
         * @param
         * @return The number of labels detected.
         */
        int get_num_labels();

        /**
         * @brief Method to set the maximum number of labels to detect in each inventory.
         * @param num_labels, the maximum number of labels to detect in each execution:
         * @return
         */
        void set_num_labels(int num_labels);

        /**
         * @brief Method to read a matrix data block of the label (HF RFID readers)
         * @param data, where read data is stored.
         * @param uc_db_address, data block address to read.
         * @return
         */
        void read_hf(unsigned char data[BLOCK_SIZE_HF], unsigned char uc_db_address);

        /**
         * @brief Method to write the address (HF RFID reader)
         * @param data, where read data is stored.
         * @param uid, label id to read.
         * @param uc_db_address, data block address to read.
         * @return
         */
        void write_hf(unsigned char data[BLOCK_SIZE_HF], unsigned char uc_db_address, std::string uid);

        /**
         * @brief Method to write EPC labels writing multiple blocks.
         * @param data, where read data is stored.
         * @param epc, label id to read.
         * @param mem_bank, memory bank to read: 0x01 EPC , 0x03 USER.
         * @param address, data block address to read.
         * @param num_blocks, number of blocks to read.
         * @return error: -1  or success: 0.
         */
        int write_tag_epc(unsigned char* data, std::string epc, unsigned int mem_bank, unsigned int address,
                          unsigned int num_blocks);

        /**
         * @brief Method to read the UID labels within the range of the wire.
         * @param
         * @return The table data size of the tags detected.
         */
        int inventory();

    private:
        /**
         * @brief Method to set the type of RFID reader used.
         * @param index, number of device (index).
         * @return error: -1 or success: 0.
         */
        int set_type_reader(int index);

        /**
         * @brief Method to set the handle USB port of the lower protocol layer (FEUSB)
         *        to the upper protocol layer (FEISC).
         * @param num_device, number of device.
         * @return error: -1 or success: 0.
         */
        int set_port_hand(long num_device);

        int _device_handle, _reader_handle, _lenght, _num_max_labels, _num_devices;
        long _id_device;

        std::string _device_name;
        std::string _snr;
        std::string _DSFID;

        std::vector<std::string> _labels;

        FEDM_ISCReaderModule _rfid_reader;
};
#endif
