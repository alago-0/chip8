#include <criterion/criterion.h>
#include <stdio.h>
#include "core.h"

Test(core, core_fetch_is_instruction)
{
  // Arrange
  Core core = core_init();
  core.ram[core.pc] = 0x12;
  core.ram[core.pc + 1] = 0x89;

  // Act
  uint16_t instruction = core_fetch(&core);
  
  // Assert
  //printf("instruction: %X\n", instruction);
  cr_assert_eq(instruction, 0x1289);
}

Test(core, core_fetch_pc_is_increased_by_two)
{
  // Arrange
  Core core = core_init();
  uint16_t pc_start = core.pc;

  // Act
  core_fetch(&core);
  
  // Assert
  cr_assert_eq(pc_start + 2, core.pc);
}


Test(core, core_fetch_pc_ram_overflow_exits_1, .exit_code = 1)
{
  // Arrange
  Core core = core_init();
  core.pc = RAM_MAX;

  // Act
  core_fetch(&core);
  
  // Assert
}


Test(core, core_fetch_at_max_valid_pc_does_not_exit, .exit_code = 0)
{
  // Arrange
  Core core = core_init();
  core.pc = RAM_MAX - 1;

  // Act
  core_fetch(&core);
  
  // Assert
  cr_assert_eq(core.pc, RAM_SIZE);
}


Test(core, core_instruction_get_first_nibble_is_correct)
{
  // Arrange
  uint16_t instruction = 0x1234;

  // Act
  uint8_t first_nibble = core_instruction_get_first_nibble(instruction);

  // Assert
  cr_assert_eq(first_nibble, 0x1);
}


Test(core, core_instruction_get_second_nibble_is_correct)
{
  // Arrange
  uint16_t instruction = 0x1234;

  // Act
  uint8_t second_nibble = core_instruction_get_second_nibble(instruction);

  // Assert
  //printf("second_nibble: %X\n", second_nibble);
  cr_assert_eq(second_nibble, 0x2);
}


Test(core, core_instruction_get_third_nibble_is_correct)
{
  // Arrange
  uint16_t instruction = 0x1234;

  // Act
  uint8_t third_nibble = core_instruction_get_third_nibble(instruction);

  // Assert
  cr_assert_eq(third_nibble, 0x3);
}


Test(core, core_instruction_get_fourth_nibble_is_correct)
{
  // Arrange
  uint16_t instruction = 0x1234;

  // Act
  uint8_t fourth_nibble = core_instruction_get_fourth_nibble(instruction);

  // Assert
  cr_assert_eq(fourth_nibble, 0x4);
}


Test(core, core_instruction_get_second_bytes_is_correct)
{
  // Arrange
  uint16_t instruction = 0x1234;

  // Act
  uint8_t second_byte = core_instruction_get_second_byte(instruction);

  // Assert
  cr_assert_eq(second_byte, 0x34);
}

Test(core, core_instruction_get_second_third_fourth_nibbles_is_correct)
{
  // Arrange
  uint16_t instruction = 0x1234;

  // Act
  uint16_t second_third_fourth_nibbles = core_instruction_get_second_third_fourth_nibbles(instruction);

  // Assert
  cr_assert_eq(second_third_fourth_nibbles, 0x234);
}


Test(core, core_jump_to_address_is_correct)
{
  // Arrange
  Core core = core_init();
  core.ram[core.pc] = 0x12;
  core.ram[core.pc + 1] = 0x34;

  // Act
  core_decode_execute(&core);

  // Assert
  //printf("pc: %X\n", core.pc);
  cr_assert_eq(core.pc, 0x234);
}


Test(core, core_set_register_v_is_correct)
{
  // Arrange
  Core core = core_init();
  core.ram[core.pc] = 0x62;
  core.ram[core.pc + 1] = 0x34;

  // Act
  core_decode_execute(&core);

  // Assert
  cr_assert_eq(core.v[0x2], 0x34);
}


Test(core, core_add_to_register_v_is_correct)
{
  // Arrange
  Core core = core_init();
  core.ram[core.pc] = 0x72;
  core.ram[core.pc + 1] = 0x34;
  core.v[0x2] = 0x30;

  // Act
  core_decode_execute(&core);

  // Assert
  cr_assert_eq(core.v[0x2], 0x30 + 0x34);
}


Test(core, core_set_index_is_correct)
{
  // Arrange
  Core core = core_init();
  core.ram[core.pc] = 0xA2;
  core.ram[core.pc + 1] = 0x34;

  // Act
  core_decode_execute(&core);

  // Assert
  cr_assert_eq(core.i, 0x234);
}