/**
 * @file        rfid_mock.h
 * @brief       Mock for rfid driver.
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-03
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

#include <gmock/gmock.h>
#include "rfid_driver_interface.h"

class MockRfidWrapper : public RfidDriverInterface {
    public:
        MOCK_METHOD0(close_device, int());
        MOCK_METHOD0(get_uid_label, std::string());
        MOCK_METHOD0(get_tags, std::string*());
        MOCK_METHOD0(get_type_reader, std::string());
        MOCK_METHOD0(get_num_labels, int());
        MOCK_METHOD0(inventory, int());
        
        MOCK_METHOD1(open_device, int(long num_device));
        MOCK_METHOD1(set_num_labels, void(int num_labels));
        
        MOCK_METHOD2(read_hf, void(unsigned char data[BLOCK_SIZE_HF], unsigned char uc_db_address));
        
        MOCK_METHOD3(write_hf, void(unsigned char data[BLOCK_SIZE_HF], unsigned char uc_db_address, std::string uid));
        
        MOCK_METHOD5(write_tag_epc, int(unsigned char* data, std::string epc, unsigned int mem_bank, unsigned int address, unsigned int num_blocks));
    };
