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

void parseFileBuffer(char *file, size_t size, IIList* code) {
  int bufferSize = size;
  char* buffer = (char*)malloc(bufferSize);
  Definition defines[DEFINE_MAX];
  int defineIndex = 0;
  for (int i = 0, bIndex = 0, charsFromNewline = 0; i < size; i++, bIndex++, charsFromNewline++) {
    char c = file[i];
    if (c == CHAR(MACRO_PREFIX)) {
      if (charsFromNewline == 0) {
        Definition def;
        int j = 0;
        while (c != ' ' && j < size && j < DEFINE_NAME_MAX) {
          def.name[j++] = c;
          c = file[++i];
        }
        def.name[j] = 0;
        def.define = &(file[i]);
        def.size = 0;
        while (c != '\n') {
          def.size++;
          c = file[++i];
        }
        defines[defineIndex++] = def;
        buffer[bIndex] = '\n';
        charsFromNewline = -1;
      } else {
        char name[DEFINE_NAME_MAX];
        int j = 0;
        while (c != ' ' && c != '\n' && j < size && j < DEFINE_NAME_MAX) {
          name[j++] = c;
          c = file[++i];
        }
        name[j] = 0;
        if (c == '\n')
          i--;

        for (int k = 0; k < defineIndex; k++) {
          if (strcmp(name, defines[k].name) == 0) {
            bufferSize += defines[k].size;
            buffer = realloc(buffer, bufferSize);
            for (int z = 0; z < defines[k].size; z++) {
              char dc = defines[k].define[z];
              if (dc == CHAR(MACRO_NEWLINE))
                buffer[bIndex++] = '\n';
              else
                buffer[bIndex++] = dc;
            }
            break;
          }
        }
        bIndex--;
      }
    } else { 
      if (c == '\n')
        charsFromNewline = -1;
      buffer[bIndex] = c;
    }
  }
  
  string_list* splitted = split(buffer, bufferSize, '\n');
  
  listReset(code);
  size_t label_amount = 0;
  for (int i = 0; i < splitted->size; i++) {
    char* line = trim(splitted->buf[i]);
    if (line[0] != CHAR(LABEL_PREFIX)) continue;
    label_amount++;
  }

  char* keys[label_amount];
  int values[label_amount];
  int map_index = 0;
  for (int i = 0, effective_i = 0; i < splitted->size; i++, effective_i++) {
    char* line = trim(splitted->buf[i]);
    if (strcmp(trim(line), "") == 0 || line[0] == CHAR(COMMENT_PREFIX)) {
      effective_i--;
      continue;
    }
    if (line[0] != CHAR(LABEL_PREFIX)) continue;
    char* labelName = line + 1;
    keys[map_index] = labelName;
    values[map_index] = effective_i;
    map_index++;
    effective_i--;
  }

  for (int i = 0; i < splitted->size; i++) {
    char* line = trim(splitted->buf[i]);
    if (strcmp(trim(line), "") == 0 || line[0] == CHAR(LABEL_PREFIX) || line[0] == CHAR(COMMENT_PREFIX)) continue;
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
      if (string2Param(lparam, &instance.lparam, keys, values, label_amount))
        return;
    }
    if (params->size > 1) {
      char* rparam = params->buf[1];
      rparam = trim(rparam);
      if (string2Param(rparam, &instance.rparam, keys, values, label_amount))
        return;
    }
    if (params->size > 2)
      return;
    listPush(code, instance);
  }
  free(buffer);
}

bool substrEqual(char *s, size_t start, size_t end, char *s2) {
  size_t size = (end-start+1)*sizeof(char);
  char* substr = (char*)malloc(size);
  memcpy(substr, &(s[start]), size);
  substr[size] = 0;
  return strcmp(substr, s2) == 0;
}

int string2Param(char *s, InstructionParam *param, char** keys, int* values, size_t size) {
  if (s[0] == CHAR(LABEL_PREFIX)) {
    char* label = s+1;
    int index = -1;
    for (int i = 0; i < size; i++) {
      if (strcmp(label, keys[i]) == 0) {
        index = i;
        break;
      }
    }
    if (index < 0) return 1;
    param->val_type = INSP_LITERAL;
    param->value.literal = values[index];
    return 0;
  }
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
