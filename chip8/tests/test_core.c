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