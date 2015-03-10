#ifndef __RFID_DRIVER_INTERFACE_H__
#define __RFID_DRIVER_INTERFACE_H__

/**
 * @file        rfid_driver_interface.h
 * @brief       Interface for the rfid driver.
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-02
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

#include <iostream>
#include "rfid_driver_data.h"

class RfidDriverInterface {
    public:
        /**
         * @brief Empty constructor.
         */
        RfidDriverInterface()
        {
        }

        /**
         * @brief Destructor.
         */
        virtual ~RfidDriverInterface()
        {
        }

        /**
         * @brief Method to open and enable the USB RFID reader.
         * @param num_device, number of device.
         * @return error: -1 or success: 0.
         */
        virtual int open_device(long num_device) = 0;

        /**
         * @brief Method to close the USB RFID reader.
         * @param
         * @return error: -1 or success: 0.
         */
        virtual int close_device() = 0;

        /**
         * @brief Method to get the UID label detected.
         * @param
         * @return The UID detected.
         */
        virtual std::string get_uid_label() = 0;

        /**
         * @brief Method to get the UIDs of the labels detected.
         * @param
         * @return Labels detected in the inventory function.
         */
        virtual std::string* get_tags() = 0;

        /**
         * @brief Method to get the type of RFID reader used.
         * @return a string with the name or empty if error.
         */
        virtual std::string get_type_reader() = 0;

        /**
         * @brief Method to get the number of labels detected.
         * @param
         * @return The number of labels detected.
         */
        virtual int get_num_labels() = 0;

        /**
         * @brief Method to set the maximum number of labels to detect in each inventory.
         * @param num_labels, the maximum number of labels to detect in each execution:
         * @return
         */
        virtual void set_num_labels(int num_labels) = 0;

        /**
         * @brief Method to read a matrix data block of the label (HF RFID readers)
         * @param data, where read data is stored.
         * @param uc_db_address, data block address to read.
         * @return
         */
        virtual void read_hf(unsigned char data[BLOCK_SIZE_HF], unsigned char uc_db_address) = 0;

        /**
         * @brief Method to write the address (HF RFID reader)
         * @param data, where read data is stored.
         * @param uid, label id to read.
         * @param uc_db_address, data block address to read.
         * @return
         */
        virtual void write_hf(unsigned char data[BLOCK_SIZE_HF], unsigned char uc_db_address, std::string uid) = 0;

        /**
         * @brief Method to write EPC labels writing multiple blocks.
         * @param data, where read data is stored.
         * @param epc, label id to read.
         * @param mem_bank, memory bank to read: 0x01 EPC , 0x03 USER.
         * @param address, data block address to read.
         * @param num_blocks, number of blocks to read.
         * @return error: -1  or success: 0.
         */
        virtual int write_tag_epc(unsigned char* data, std::string epc, unsigned int mem_bank, unsigned int address,
                                  unsigned int num_blocks) = 0;

        /**
         * @brief Method to read the UID labels within the range of the wire.
         * @param
         * @return The table data size of the tags detected.
         */
        virtual int inventory() = 0;
};

#endif
