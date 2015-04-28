#ifndef __RFID_DRIVER_DATA_H__
#define __RFID_DRIVER_DATA_H__

/**
 * @file        rfid_driver_data.h
 * @brief       Global data.
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

//////////////////////////////////////////////////
// maximum data size for label for HF
#define BLOCK_SIZE_HF   4

//////////////////////////////////////////////////

// number of readers in the robot
#define NUM_READERS     1

//////////////////////////////////////////////////

// reader id
#define BASE_READER_ID  216135657
#define HEAD_READER_ID  216132099

//////////////////////////////////////////////////

// reader bus direction
#define BUS_ADDRESS_HEAD_READER     0xFF
#define BUS_ADDRESS_BASE_READER     0xFE

//////////////////////////////////////////////////

// reader port
#define HEAD_READER_PORT    268435458
#define BASE_READER_PORT    268435459

//////////////////////////////////////////////////

// RFID reader names
#define HF_NAME  "ID ISC.MR101-U"

//////////////////////////////////////////////////

// data from the card is a matrix
#define NUM_COLUMNS 4
#define NUM_ROWS    255

//////////////////////////////////////////////////

// maximum length of the data
#define MAX_LENGTH  (NUM_COLUMNS * NUM_ROWS)

//////////////////////////////////////////////////

// length of the UID
#define UID_LENGTH  24

//////////////////////////////////////////////////

/**
 * card_data struct
 * UID[UIDLENGTH] array to keep the UID label
 * data struct
 */
typedef struct {
        char UID[UID_LENGTH];

        struct {
                unsigned int length;
                unsigned char value[MAX_LENGTH];
        } data;
} card_data;

//////////////////////////////////////////////////

#endif
