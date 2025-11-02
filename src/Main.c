#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <ConsoleUtils/ConsoleUtils.h>
#include <InstructionRegistry.h>
#include <utils.h>
#include <FileLoader.h>
#include <TE_Constants.h>
#include <windows.h>

const int STACK_MAX_SHOWN = 16;
const int CODE_MAX_SHOWN = 16;
const int MEMORY_SIDE_LEN = 16;
const int BETWEEN_INS_SLEEP = 100;
int stackRenderIndex = STACK_END - STACK_MAX_SHOWN;
int codeRenderIndex = 0;

int getNumberDigits(int n) {
  return (n == 0) ? 1 : (floor(log10l(n)) + 1);
}

void cls() {
  clearScreen();
}

int getStackMaxDigits(CPU* cpu) {
  int maxDigits = -1;
  for (int i = STACK_START; i < STACK_END; ++i) {
    int k = cpu->memory[i];
    int digits = getNumberDigits(k);
    if (digits > maxDigits)
      maxDigits = digits;
  }
  return maxDigits;
}

int drawStack(Event* event, CPU* cpu) {
  int maxDigits = getStackMaxDigits(cpu);
  int bp = cpu->registers[RG_BP];
  int sp = cpu->registers[RG_SP];
  for (int i = stackRenderIndex; i < stackRenderIndex+STACK_MAX_SHOWN; i++) {
    putchar('|');
    int digits = getNumberDigits(cpu->memory[i]);
    int spaces = maxDigits - digits;
    if (spaces != 0) {
      printSpaces(spaces);
    }
    printf("%u|", cpu->memory[i]);
    if (i == bp && bp == sp)
      printf("<-");
    else if (i == bp)
      printf(" -");
    else if (i == sp)
      printf(" <");
    printf("\n");
  }
  return maxDigits+4;
}

int drawCode(Event* event, CPU* cpu, IIList* code, int padding) {
  int y = 0;
  int new_padding = -1;
  int index = codeRenderIndex;
  for (int i = index; i < index+CODE_MAX_SHOWN; i++) {
    if (i >= code->size) break;
    setCursorPos(padding+1, y);
    int chars = printf("| %u ", i);
    chars += printII(&(code->head[i]));
    if (cpu->registers[RG_IP] == i) {
      chars += printf(" < ");
    }
    if (chars > new_padding)
      new_padding = chars;
    y++;
  }
  return (new_padding < 0) ? padding : new_padding + padding;
}

int drawMemory(Event* event, CPU* cpu, int padding) {
  int y = 0;
  for (int i = 0; i < MEMORY_SIZE; i++) {
    if (i % MEMORY_SIDE_LEN == 0)
      setCursorPos(padding + 1, y++);

    printf("%02X:", cpu->memory[i]);
  }
  return padding + 3*MEMORY_SIDE_LEN;
}

int drawRegisters(Event* event, CPU* cpu, int padding) {
  int y = 0;
  int new_padding = -1;
  setCursorPos(padding + 2, y);
  for (int i = 0; i < __RG_COUNT; i++) {
    int chars = printf("|  %s: %hu", REGISTERS[i], cpu->registers[i]);
    moveCursor(-chars, 1);
    if (chars > new_padding)
      new_padding = chars;
  }
  return padding + new_padding;
}

int drawFlags(Event* event, CPU* cpu, int padding) {
  int y = 0;
  int new_padding = -1;
  setCursorPos(padding + 5, y);
  
  bool flags[] = {cpu->FLAG_Z, cpu->FLAG_G, cpu->FLAG_L, cpu->FLAG_ERR, cpu->FLAG_HLT};
  for (int i = 0; i < sizeof(FLAGS) / sizeof(char*); i++) {
    int chars = printf("%s: %u", FLAGS[i], flags[i]);
    moveCursor(-chars, 1);
    if (chars > new_padding)
      new_padding = chars;
  }
  return (new_padding < 0) ? padding : new_padding + padding;
}

void handleRendering(Event* event, CPU* cpu, IIList* code, bool* drawn) {
  cls();
  int padding = drawStack(event, cpu);
  padding = drawCode(event, cpu, code, padding) + 5;
  padding = drawMemory(event, cpu, padding);
  padding = drawRegisters(event, cpu, padding);
  padding = drawFlags(event, cpu, padding);
  *drawn = true;
}

Menu mainMenu;

IIList code;
bool running = true;
bool needsIndex = false;

Menu instructionSelect;
Menu paramTypeSelect;
Menu registerSelect;
Menu literalGet;
Menu indexGet;
Menu pathGet;
Menu helpMenu;
char givenLiteral[6];
char givenIndex[10];
char givenPath[260];
FILE* selectedFile;
char* selectedFileMode;
int selectedIndex = -1;
int cursor = 0;
int pageIndex = 0;
InstructionParam parameter;

bool inputMenuEvent(Event* event) {
  if (event->eventType != KEY_DOWN_EVENT) return false;
  KeyCode code = event->params.keyCode;
  if (code.key == VK_RETURN) {
    parameter.value.literal = atoi(givenLiteral);
    memset(givenLiteral, 0, sizeof(givenLiteral) / sizeof(givenLiteral[0]));
    cursor = 0;
    return true;
  }
  
  if (code.key == VK_BACK && cursor != 0)
    givenLiteral[--cursor] = '\0';

  if (isChar(code) && cursor != sizeof(givenLiteral) / sizeof(char) - 2 && (code.asChar >= 48 && code.asChar <= 57))
    givenLiteral[cursor++] = code.asChar;

  return false;
}

bool indexMenuEvent(Event* event) {
  if (event->eventType != KEY_DOWN_EVENT) return false;
  KeyCode code = event->params.keyCode;
  if (code.key == VK_RETURN) {
    selectedIndex = atoi(givenIndex);
    memset(givenIndex, 0, sizeof(givenIndex) / sizeof(givenIndex[0]));
    cursor = 0;
    return true;
  }
  
  if (code.key == VK_BACK && cursor != 0)
    givenIndex[--cursor] = '\0';

  if (isChar(code) && cursor != sizeof(givenIndex) / sizeof(char) - 2 && (code.asChar >= '0' && code.asChar <= '9'))
    givenIndex[cursor++] = code.asChar;

  return false;
}
bool pathMenuEvent(Event* event) {
  if (event->eventType != KEY_DOWN_EVENT) return false;
  KeyCode code = event->params.keyCode;
  if (code.key == VK_RETURN) {
    size_t size = strlen(givenPath)+1;
    char cpy[size];
    memcpy(cpy, givenPath, size);
    string_list* temp = split(cpy, size, '.');
    if (temp->size < 2 || temp->size > 3) return false;
    if (strcmp(temp->buf[temp->size-1], "tne") != 0) return false;
    if (!fileExists(givenPath)) return false;
    selectedFile = fopen(givenPath, selectedFileMode);
    memset(givenPath, 0, sizeof(givenPath) / sizeof(givenPath[0]));
    cursor = 0;
    return true;
  }
  
  if (code.key == VK_BACK && cursor != 0)
    givenPath[--cursor] = '\0';

  if (isChar(code) && cursor != sizeof(givenPath) / sizeof(char) - 2)
    givenPath[cursor++] = code.asChar;

  return false;
}

bool registerSelected(int selection, const MenuOption option) {
  if (selection == __RG_COUNT) return true;

  parameter.value.reg = (Reg) selection;

  return true;
}

bool paramTypeSelected(int selection, const MenuOption option) {
  if (selection == INSP_NULL) return true;
  parameter.val_type = (selection == 0) ? INSP_REGISTER : INSP_LITERAL;

  if (selection == 0) {
    menuLoop(&registerSelect, cls, registerSelected, NULL);
    return true;
  }

  menuLoop(&literalGet, cls, NULL, inputMenuEvent);

  return true;
}

bool instructionSelected(int selection, const MenuOption option) {
  if ((InsType) selection == __INS_COUNT) return true;
  InstructionInstance instance = {.ins = getInstruction(selection)};
  instance.lparam.val_type = INSP_NULL;
  instance.rparam.val_type = INSP_NULL;
  int paramsNeeded = parametersNeeded(instance.ins->type);

  if (paramsNeeded >= 1) {
    menuLoop(&paramTypeSelect, cls, paramTypeSelected, NULL);
    instance.lparam = parameter;
  }
  if (paramsNeeded > 1) {
    menuLoop(&paramTypeSelect, cls, paramTypeSelected, NULL);
    instance.rparam = parameter;
  }

  if (!needsIndex) {
    listPush(&code, instance);
    return true;
  }
  menuLoop(&indexGet, cls, NULL, indexMenuEvent);
  listInsert(&code, instance, selectedIndex);

  return true;
}

enum {
  ADD, INSERT, POP, REMOVE, LOAD, UNDO, EXIT
};

bool mainMenuInput(int selection, const MenuOption option) {
  if (selection == UNDO) return true;
  if (selection == EXIT) {
    running = false;
    return true;
  }
  if (selection == ADD) {
    menuLoop(&instructionSelect, cls, instructionSelected, NULL);
    return true;
  } else if (selection == INSERT) {
    needsIndex = true;
    menuLoop(&instructionSelect, cls, instructionSelected, NULL);
    return true;
  } else if (selection == POP) {
    listPop(&code);
    return true;
  } else if (selection == REMOVE) { 
    menuLoop(&indexGet, cls, NULL, inputMenuEvent);
    listRemove(&code, selectedIndex);
    return true;
  } else if (selection == LOAD) { 
    selectedFileMode = "r";
    menuLoop(&pathGet, cls, NULL, pathMenuEvent);
    char* content;
    size_t fileSize = allocateFileBuffer(selectedFile, &content);
    initializeFileBuffer(selectedFile, content);
    parseFileBuffer(content, fileSize, &code);
    return true;
  }
  return false;
}

int main(int argc, char** argv) {
  Originals originals;
  setRawMode(&originals);

  CPU cpu = createCPU();
  code = newList();
  hideCursor();
  
  bool drawn = false;
  
  MenuOption literalGetOptions[] = {givenLiteral};
  MenuOption indexGetOptions[] = {givenIndex};
  MenuOption pathGetOptions[] = {givenPath};
  MenuOption helpMenuOptions[] = {};

  createMenuWithTitle(mainMenu, "Main Menu", mainMenuOptions);
  createMenuWithTitle(instructionSelect, "Select Instruction", instructionSelectOptions);
  createMenuWithTitle(paramTypeSelect, "Select ParamType", paramTypeSelectOptions);
  createMenuWithTitle(registerSelect, "Select Register", registerSelectOptions);
  createMenuWithTitle(literalGet, "Insert Literal", literalGetOptions);
  createMenuWithTitle(indexGet, "Insert Index (Zero Based)", indexGetOptions);
  createMenuWithTitle(pathGet, "Insert Path", pathGetOptions);
  createMenu(helpMenu, helpMenuOptions);

  Event event;
  while (running) {
    getInput(&event);
    if (handleCtrlQ(&event)) {
      running = false;
    }

    if (event.eventType == KEY_DOWN_EVENT && isChar(event.params.keyCode)) {
      KeyCode keycode = event.params.keyCode;
      if (keycode.asChar == 'e') {
        menuLoop(&mainMenu, cls, mainMenuInput, NULL);
        cls();
        drawn = false;
      } else if (keycode.ctrl && keycode.key == VK_SPACE) {
        if (cpu.registers[RG_IP] < 0 || cpu.registers[RG_IP] >= code.size || cpu.FLAG_ERR || cpu.FLAG_HLT) continue; 
        InstructionInstance instance = code.head[cpu.registers[RG_IP]];
        instance.ins->callback(&cpu, &instance.lparam, &instance.rparam);
        cpu.registers[RG_IP] = (cpu.registers[RG_IP] + 1) % code.size;
        drawn = false;
      } else if (!keycode.ctrl && keycode.key == VK_SPACE) {
        for (cpu.registers[RG_IP] = 0; (cpu.registers[RG_IP] < code.size) && (!cpu.FLAG_ERR && !cpu.FLAG_HLT);) {
          InstructionInstance instance = code.head[cpu.registers[RG_IP]];
          instance.ins->callback(&cpu, &instance.lparam, &instance.rparam);
          cpu.registers[RG_IP]++;
          handleRendering(&event, &cpu, &code, &drawn);
          Sleep(BETWEEN_INS_SLEEP);
        }
        cpu.registers[RG_IP] = 0;
        drawn = false;
      } else if (keycode.ctrl && keycode.key == VK_R) {
        resetCPU(&cpu);
        drawn = false;
      } else if (!keycode.alt && keycode.key == VK_W) {
        if (codeRenderIndex > 0) {
          codeRenderIndex--;
          drawn = false;
        }
      } else if (!keycode.alt && keycode.key == VK_S) {
        if (code.size - codeRenderIndex > CODE_MAX_SHOWN && codeRenderIndex < code.size-1) {
          codeRenderIndex++;
          drawn = false;
        }
      } else if (keycode.alt && keycode.key == VK_W) {
        if (stackRenderIndex > STACK_START) {
          stackRenderIndex--;
          drawn = false;
        }
      } else if (keycode.alt && keycode.key == VK_S) {
        int index = stackRenderIndex;
        if (index < STACK_END - STACK_MAX_SHOWN) {
          stackRenderIndex++;
          drawn = false;
        }
      } else if (keycode.key == VK_H) {
        // menuLoop(&helpMenu, helpDisplay, NULL, helpEvent);
      }
    }

    if (!drawn)
      handleRendering(&event, &cpu, &code, &drawn);
  };

  resetRawMode(&originals);

	return 0;
}
