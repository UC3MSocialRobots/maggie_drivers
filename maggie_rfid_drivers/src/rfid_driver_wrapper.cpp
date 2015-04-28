/**
 * @file        rfid_driver_wrapper.cpp
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

#include "maggie_rfid_drivers/rfid_driver_wrapper.h"

//////////////////////////////////////////////////

RfidDriverWrapper::RfidDriverWrapper()
{
    _snr = "0";
    _DSFID = "0";
    _num_devices = 0;
    _device_handle = 0;
    _reader_handle = 0;
    _lenght = 0;
    _id_device = 0;
    _num_max_labels = 0;

    // clean previous list of devices, if exists
    FEUSB_ClearScanList();
}

//////////////////////////////////////////////////

RfidDriverWrapper::~RfidDriverWrapper()
{
}

//////////////////////////////////////////////////

int RfidDriverWrapper::open_device(long num_device)
{
    char cDeviceID[16];
    char cError[256];
    char* pEnd;
    bool encontrado = false;
    char cPara[16] = "Device-ID";
    int error = 0;

    // scan everything and return the rumber of devices found
    int index = FEUSB_Scan(FEUSB_SCAN_ALL, NULL);
    if (index < 0) {
        FEUSB_GetErrorText(index, cError);
        ROS_ERROR("[RFID_DRIVER] Error: %s", cError);
    }
    else {
        _num_devices = FEUSB_GetScanListSize();

        if (_num_devices < 0) {
            ROS_ERROR("[RFID_DRIVER] Error: opening USB devices");
        }
        else {
            for (int i = 0; i < _num_devices; i++) {
                if (FEUSB_GetScanListPara(i, cPara, cDeviceID) == 0) {
                    // convert the ID from decimal to hexadecimal
                    _id_device = strtol(cDeviceID, &pEnd, 16);

                    if (num_device == _id_device) {
                        encontrado = true;
                        _device_handle = FEUSB_OpenDevice(_id_device);

                        if (_device_handle < 0) {
                            ROS_ERROR("[RFID_DRIVER] Error: opening USB devices");
                            error = -1;
                        }
                        else {
                            set_type_reader(i);

                            _reader_handle = FEISC_NewReader(_device_handle);

                            if (_reader_handle < 0) {
                                ROS_ERROR("[RFID_DRIVER] Error: opening USB devices");
                                error = -1;
                            }
                            else {
                                set_port_hand(num_device);
                                _rfid_reader.SetReaderHnd(_reader_handle);
                                ROS_DEBUG("[RFID_DRIVER] Device %ld enabled", num_device);
                                i = _num_devices;
                            }
                        }
                    }
                }
            }
        }
    }

    if (!encontrado) {
        error = -1;
    }

    return error;
}

//////////////////////////////////////////////////

int RfidDriverWrapper::close_device()
{
    int error = 0;

    if (_reader_handle < 0) {
        error = -1;
    }
    else {
        FEISC_DeleteReader(_reader_handle);
        if (_device_handle < 0) {
            error = -1;
        }
        else {
            int iErr = FEUSB_CloseDevice(_device_handle);
            if (iErr == 0) {
                ROS_DEBUG("[RFID_DRIVER] USB device %d closed", iErr);
            }
            else {
                ROS_ERROR("[RFID_DRIVER] Error: closing the device");
                error = -1;
            }
        }
    }

    return error;
}

//////////////////////////////////////////////////

std::string RfidDriverWrapper::get_uid_label()
{
    return _snr;
}

//////////////////////////////////////////////////

std::string* RfidDriverWrapper::get_tags()
{
    std::string* detected = &_labels[0];

    return detected;
}

//////////////////////////////////////////////////

std::string RfidDriverWrapper::get_type_reader()
{
    std::string result = "";

    if (strcmp(_device_name.c_str(), HF_NAME) == 0) {
        result = HF_NAME;
    }

    return result;
}

//////////////////////////////////////////////////

int RfidDriverWrapper::get_num_labels()
{
    return _lenght;
}

//////////////////////////////////////////////////

void RfidDriverWrapper::set_num_labels(int num_labels)
{
    _num_max_labels = num_labels;
}

//////////////////////////////////////////////////

void RfidDriverWrapper::read_hf(unsigned char data[BLOCK_SIZE_HF], unsigned char uc_db_address)
{
    // buffer for a block of data (block size: 4 bytes)
    unsigned char ucDB[BLOCK_SIZE_HF];

    unsigned char ucBlockSize;
    int iErr, iIdx, i;
    char cError[256];

    // fix the serial number (UID) of the label
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_REQ_UID, _snr);
    // command to write data in the label
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char) 0x23);
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char) 0x00);
    // directional mode
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char) 0x01);
    // fix the direction of the block
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, uc_db_address);
    // write 1 block of data (size 4 bytes)
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_REQ_DBN, (unsigned char) 0x01);

    // communication
    iErr = _rfid_reader.SendProtocol(0xB0);
    _rfid_reader.GetErrorText(cError, iErr);

    // find the position in the table of the label with UID = _snr
    iIdx = _rfid_reader.FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, _snr);

    // get the size of the block of data, for HF_NAME is 4 bytes
    _rfid_reader.GetTableData(iIdx, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_BLOCK_SIZE, &ucBlockSize);

    // get a block of data
    _rfid_reader.GetTableData(iIdx, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_RxDB, uc_db_address, ucDB, ucBlockSize);

    // get a block of data
    for (i = 0; i < BLOCK_SIZE_HF; i++) {
        data[i] = ucDB[i];
    }
}

//////////////////////////////////////////////////

void RfidDriverWrapper::write_hf(unsigned char data[BLOCK_SIZE_HF], unsigned char uc_db_address, std::string uid)
{
    _snr = uid;
    char cError[256];

    // find the position in the table of the label with UID = _snr
    int iIdx = _rfid_reader.FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, _snr);

    // fix the serial number (UID) of the label
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_REQ_UID, _snr);
    // command to write data in the label
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char) 0x24);
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char) 0x00);
    // directional mode
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char) 0x01);
    // write 6 blocks of data (size 2 bytes) fix the direction of the block
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, uc_db_address);
    // write 6 blocks of data (size 4 bytes)
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_REQ_DBN, (unsigned char) 0x01);
    // size of the block for the tags HF_NAME: 4
    _rfid_reader.SetTableData(iIdx, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_BLOCK_SIZE, (unsigned char) BLOCK_SIZE_HF);
    // write the blocks of data in the table
    _rfid_reader.SetTableData(iIdx, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_TxDB, uc_db_address, data, BLOCK_SIZE_HF);

    // communication
    int iErr = _rfid_reader.SendProtocol(0xB0);
    _rfid_reader.GetErrorText(cError, iErr);
}

//////////////////////////////////////////////////

int RfidDriverWrapper::write_tag_epc(unsigned char* data, std::string epc, unsigned int mem_bank, unsigned int address,
                                     unsigned int num_blocks)
{
    _snr = epc;
    FedmIscTagHandler* pTagHandler = NULL;

    pTagHandler = _rfid_reader.GetTagHandler(_snr);
    FedmIscTagHandler_EPC_Class1_Gen2* pEPC = (FedmIscTagHandler_EPC_Class1_Gen2*) pTagHandler;
    int iErr = pEPC->WriteMultipleBlocks(mem_bank, address, num_blocks, "", data);
    ROS_ERROR("[RFID_DRIVER] iErr %d", iErr);

    return iErr;
}

//////////////////////////////////////////////////

int RfidDriverWrapper::inventory()
{
    char cError[256];
    int i;
    unsigned char ucTrType = 0;

    // clean the vector of labels
    _labels.clear();

    // indicate the maximum number of labels to detect
    _rfid_reader.SetTableSize(FEDM_ISC_ISO_TABLE, _num_max_labels);

    _rfid_reader.ResetTable(FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_ALL);

    // inventory
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char) 0x01);
    _rfid_reader.SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char) 0x00);
    int iErr = _rfid_reader.SendProtocol(0xB0);

    if (iErr < 0) {
        _rfid_reader.GetErrorText(cError, iErr);
        ROS_ERROR("[RFID_DRIVER] Error: %d in inventory. %s", iErr, cError);
        return iErr;
    }

    _lenght = _rfid_reader.GetTableLength(FEDM_ISC_ISO_TABLE);
    if (_lenght == 0) {
        // reader: no transponder in reader field
        FEISC_GetLastState(_rfid_reader.GetReaderHnd(), cError);

        return _lenght;
    }

    for (i = 0; i < _lenght; i++) {
        _rfid_reader.GetTableData(i, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_TRTYPE, &ucTrType);

        // 0x03:  ISO15693
        // 0x84:  EPC Class1 Gen2
        _rfid_reader.GetTableData(i, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, _snr);
        _labels.push_back(_snr);
    }

    return _lenght;
}

//////////////////////////////////////////////////

int RfidDriverWrapper::set_type_reader(int index)
{
    std::string name = "DeviceName";
    int error = 0;

    if (FEUSB_GetScanListPara(index, (char*) name.c_str(), (char*) _device_name.c_str()) == 0) {
        if (strcmp(_device_name.c_str(), HF_NAME) == 0) {
            _rfid_reader.SetReaderType(FEDM_ISC_TYPE_ISCMR101_U);
        }
        else {
            ROS_ERROR("[RFID_DRIVER] Error: unknown RFID reader type");
            error = -1;
        }
    }
    else {
        error = -1;
    }

    return error;
}

//////////////////////////////////////////////////

int RfidDriverWrapper::set_port_hand(long num_device)
{
    int error = 0;

    if (num_device == HEAD_READER_ID) {
        _rfid_reader.SetPortHnd(HEAD_READER_PORT);
    }
    else if (num_device == BASE_READER_ID) {
        _rfid_reader.SetPortHnd(BASE_READER_PORT);
    }
    else {
        ROS_DEBUG("[RFID_DRIVER] Port by default");
        error = -1;
    }

    return error;
}

//////////////////////////////////////////////////
