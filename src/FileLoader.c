#include <FileLoader.h>

size_t allocateFileBuffer(FILE* f, char** buf) {
  fseek(f, 0, SEEK_END);
  int size = ftell(f);
  fseek(f, 0, SEEK_SET);
  *buf = (char*)malloc(size*sizeof(char));
  return size;
}

void initializeFileBuffer(FILE *f, char *buf) {
  if (f == NULL) return;
  const int BUF_SIZE = 4096;
  char temp[BUF_SIZE];
  int bytes_read;
  int base = 0;
  while ((bytes_read = fread(temp, sizeof(char), BUF_SIZE, f)) > 0) {
    memcpy(&(buf[base]), temp, bytes_read);
    base += bytes_read;
  }
  buf[base] = 0;
}

void parseFileBuffer(char *buf, size_t size, List* code) {
  string_list* splitted = split(buf, size, '\n');
  
  listReset(code);
  for (int i = 0; i < splitted->size; i++) {
    char* line = splitted->buf[i];
    InstructionInstance instance;
    instance.lparam.val_type = INSP_NULL;
    instance.rparam.val_type = INSP_NULL;
    bool found = false;
    for (int j = 0; j < sizeof(INSTRUCTIONS)/sizeof(char*); j++) {
      if (substrEqual(line, 0, 2, (char*)INSTRUCTIONS[j])) { //? Check first 3 chars
        instance.ins = getInstruction((InsType) j);
        found = true;
        break;
      }
    }
    if (!found) return;
    line += 3;
    if (*line != ' ' && parametersNeeded(instance.ins->type) != 0) return;

    string_list* params = split(line, strlen(line)+1, ',');
    if (params->size >= 1 && strcmp(params->buf[0], "") != 0) {
      char* lparam = params->buf[0];
      lparam = trim(lparam);
      if (string2Param(lparam, &instance.lparam))
        return;
    }
    if (params->size > 1) {
      char* rparam = params->buf[1];
      rparam = trim(rparam);
      if (string2Param(rparam, &instance.rparam))
        return;
    }
    if (params->size > 2)
      return;
    listPush(code, instance);
  }
}

bool substrEqual(char *s, size_t start, size_t end, char *s2) {
  size_t size = (end-start+1)*sizeof(char);
  char* substr = (char*)malloc(size);
  memcpy(substr, &(s[start]), size);
  substr[size] = 0;
  return strcmp(substr, s2) == 0;
}

int string2Param(char *s, InstructionParam *param) {
  for (int i = 0; i < __RG_COUNT; i++) {
    if (strcmp(s, REGISTERS_LOWER[i]) == 0) {
      param->val_type = INSP_REGISTER;
      param->value.reg = (Reg) i;
      return 0;
    }
  }
  size_t temp = 0;
  while (*s != 0) {
    if (!isdigit(*s)) return 1;
    temp++;
    s++;
  }
  s -= temp;
  param->val_type = INSP_LITERAL;
  param->value.literal = atoi(s);
  return 0;
}

bool fileExists(char *fname) {
  return access(fname, F_OK) == 0;
}
