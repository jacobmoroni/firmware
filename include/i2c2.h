/*
 * Copyright (c) 2017, James Jackson
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <stdint.h>

#include "revo_f4.h"

#include "gpio.h"

namespace i2c2
{

class I2C
{
  typedef enum : uint8_t
  {
    START,
    WRITE_MODE,
    READ_MODE,
    WRITE,
    READ,
    STOP,
  } task_type_t;

  enum : int8_t
  {
    RESULT_ERROR = 0,
    RESULT_SUCCESS = 1,
    RESULT_BUSY = -1
  };

  enum
    {
    SB = 0x0001,
    ADDR = 0x0002,
    BTF = 0x0004,
    ADD10 = 0x0008,
    STOPF = 0x0010,
    RES1 = 0x0020,
    RXNE = 0x0040,
    TXE = 0x0080,
    BERR = 0x0100,
    ARLO = 0x0200,
    AF = 0x0400,
    OVR = 0x0800,
    PEC_ERR = 0x1000,
    RES2 = 0x2000,
    TIMEOUT = 0x4000,
    SMB_ALERT = 0x8000,
    MSL = 0x1 << 16,
    BUSY = 0x2 << 16,
    TRA = 0x4 << 16,
    RES3 = 0x8 << 16,
    GEN_CALL = 0x10 << 16,
    SMBDE_FAULT = 0x20 << 16,
    DUALF = 0x40 << 16,
  };


  I2C();

  struct Task
  {
    task_type_t task;
    uint8_t addr;
    uint8_t* data;
    size_t len;
    void (*cb)(uint8_t);
  };

  static constexpr size_t TASK_BUFFER_SIZE = 25;
  Task tasks_[TASK_BUFFER_SIZE];
  size_t task_head_ = 0;
  size_t task_tail_ = 0;
  size_t task_idx_ = 0;

  static constexpr size_t WRITE_BUFFER_SIZE = 50;
  uint8_t write_buffer_[WRITE_BUFFER_SIZE];
  size_t wb_head_ = 0;

  uint32_t expected_event_;
  volatile bool busy_;
  volatile size_t idx_;
  size_t num_errors_;
  int8_t return_code_;

public:
  void init(const i2c_hardware_struct_t *c);
  void addJob(task_type_t type, uint8_t addr=0, uint8_t* data=nullptr, size_t len=0, void(*cb)(uint8_t)=nullptr);
  inline bool checkBusy() { return busy_; }

  Task& currentTask();
  Task& nextTask();
  Task& prevTask();
  void advanceTask();

  void write(uint8_t addr, uint8_t reg, uint8_t data);
  void read(uint8_t addr, uint8_t reg, uint8_t data);
  void read(uint8_t addr, uint8_t reg, uint8_t* data, size_t len, bool blocking=false, void(*cb)(uint8_t));

  uint8_t* copyToWriteBuf(uint8_t byte);


  void handleError();
  void handleEvent();

private:
  bool handleJob();

  DMA_InitTypeDef  DMA_InitStructure_;
  const i2c_hardware_struct_t* c_;
};

}
